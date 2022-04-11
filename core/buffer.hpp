#ifndef __CORE_BUFFER_HPP__
#define __CORE_BUFFER_HPP__

#include <memory>

#include "log/glog.h"

namespace Eayew {

template<typename ValueType>
class buffer_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using self_type = buffer_iterator;
    using value_type = ValueType;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = std::ptrdiff_t;

    explicit buffer_iterator(pointer p)
        :m_ptr(p) {
    }

    reference operator*() const {
        return *m_ptr;
    }

    buffer_iterator& operator++() {
        ++m_ptr;
        return *this;
    }

    buffer_iterator& operator--() {
        --m_ptr;
        return *this;
    }

    reference operator[](const difference_type off) const {
        return (*(*this + off));
    }

private:
    pointer m_ptr;
};


template<class Alloc>
class base_buffer {
public:
    using allocator_type = Alloc;
    using value_type = typename allocator_type::value_type;
    using iterator = buffer_iterator<value_type>;
    using const_iterator = buffer_iterator<const value_type>;
    using pointer = typename iterator::pointer;
    using const_pointer = typename const_iterator::pointer;

    constexpr static size_t DEFALUT_CAPACITY = 128;

    enum class seek_origin {
        Begin,
        Current,
        End
    };

    base_buffer(size_t capacity = DEFALUT_CAPACITY, uint32_t head_reserved = 0, const allocator_type& al = allocator_type())
        : m_allocator(al)
        , m_flag(0)
        , m_headRerved(head_reserved)
        , m_capacity(0)
        , m_readPos(0)
        , m_writePos(0) {
        capacity = (capacity > 0 ? capacity : DEFALUT_CAPACITY);
        prepare(capacity + head_reserved);
        m_readPos = m_writePos = m_headRerved;
    }

    base_buffer(const base_buffer&) = delete;

    base_buffer& operator=(const base_buffer&) = delete;

    base_buffer(base_buffer&& other) noexcept
        : m_allocator(std::move(other.m_allocator))
        , m_flag(std::exchange(other.m_flag, 0))
        , m_headRerved(std::exchange(other.m_headRerved, 0))
        , m_capacity(std::exchange(other.m_capacity, 0))
        , m_readPos(std::exchange(other.m_readPos, 0))
        , m_writePos(std::exchange(other.m_writePos, 0))
        , m_data(std::exchange(other.m_data, nullptr)) {
    }

    base_buffer& operator=(base_buffer&& other) noexcept {
        if (this != std::addressof(*other)) {
            m_allocator.deallocate(m_data, m_capacity);
            m_flag = std::exchange(other.m_flag, 0);
            m_headRerved = std::exchange(other.m_headRerved, 0);
            m_capacity = std::exchange(other.m_capacity, 0);
            m_readPos = std::exchange(other.m_readPos, 0);
            m_writePos = std::exchange(other.m_writePos, 0);
            m_data = std::exchange(other.m_data, nullptr);
            m_allocator = std::move(other.m_allocator);
        }
        return *this;
    }

    ~base_buffer() {
        m_allocator.deallocate(m_data, m_capacity);
    }

    template<typename T>
    void writeBack(const T* data, size_t count) {
        // static_assert(std::is_trivially_copyable<T>::value, "type T must be trivially copyable");
        if (nullptr == data || 0 == count) {
            return;
        }
        size_t n = sizeof(T) * count;
        auto* buffer = reinterpret_cast<T*>(prepare(n));
        memcpy(buffer, data, n);
        m_writePos += n;
    }

    void writeBack(char c) {
        *prepare(1) = c;
        ++m_writePos;
    }

    template<typename T>
    bool writeFront(const T* data, size_t count) noexcept {
        static_assert(std::is_trivially_copyable<T>::value, "type T must be trivially copyable");
        if (nullptr == data || 0 == count) {
            return false;
        }
        size_t n = sizeof(T) * count;
        if (n > m_readPos) {
            return false;
        }
        m_readPos -= n;
        auto* buffer = reinterpret_cast<T*>(std::addressof(*begin()));
        memcpy(buffer, data, n);
        return true;
    }

    template<typename T>
    bool read(T* data, size_t count) noexcept {
        static_assert(std::is_trivially_copyable<T>::value, "type T must be trivially copyable");
        if (nullptr == data || 0 == count) {
            return false;
        }
        size_t n = sizeof(T) * count;
        if (m_readPos + n > m_writePos) {
            return false;
        }
        auto* buff = std::addressof(*begin());
        memcpy(data, buff, n);
        m_readPos += n;
        return true;
    }

    void consume(size_t n) {
        seek(static_cast<int>(n));
    }

    size_t seek(int offset, seek_origin s =seek_origin::Current) noexcept {
        switch (s) {
            case seek_origin::Begin:
                m_readPos = offset;
                break;
            case seek_origin::Current:
                m_readPos += offset;
                if (m_readPos > m_writePos) {
                    m_readPos = m_writePos;
                }
                break;
            default:
                break;
        }
        return m_readPos;
    }

    void clear() noexcept {
        m_flag = 0;
        m_writePos = m_readPos = m_headRerved;
    }

    pointer prepare(size_t need) {
        if (writeAbleSize() >= need) {
            return m_data + m_writePos;
        }
        if (writeAbleSize() + m_readPos < need + m_headRerved) {
            auto required_size = m_writePos + need;
            required_size = nextPow2(required_size);
            auto tmp = m_allocator.allocate(required_size);
            if (nullptr != m_data) {
                std::memcpy(tmp, m_data, m_writePos);
                m_allocator.deallocate(m_data, m_capacity);
            }
            m_data = tmp;
            m_capacity = required_size;
        } else {
            size_t readable = size();
            if (0 != readable) {
                std::memmove(m_data + m_headRerved, m_data + m_readPos, readable);
            }
            m_readPos = m_headRerved;
            m_writePos = m_readPos + readable;
        }
        return (m_data + m_writePos);
    }

    void commit(size_t n) noexcept {
        m_writePos += n;
        if (m_writePos >= m_capacity) {
            m_writePos = m_capacity;
        }
    }

    iterator begin() noexcept {
        return iterator{ m_data + m_readPos };
    }

    iterator end() noexcept {
        return iterator{ m_data + m_writePos };
    }

    pointer data() noexcept {
        return std::addressof(*begin());
    }

    pointer wdata() noexcept {
        return std::addressof(*end());
    }

    size_t size() const noexcept {
        return m_writePos - m_readPos;
    }

    size_t capacity() const noexcept {
        return m_capacity;
    }

    size_t writeAbleSize() const noexcept {
        return m_capacity - m_writePos;
    }

private:
    size_t nextPow2(size_t x) {
        if (!(x & (x - 1))) {
            return x;
        }
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return x + 1;
    }

private:
    allocator_type m_allocator;

    uint32_t m_flag;

    uint32_t m_headRerved;

    size_t m_capacity;
    size_t m_readPos;
    size_t m_writePos;

    value_type* m_data = nullptr;
};

};

#ifdef EAYEW_ENABlE_MIMALLOC
#include "mimalloc.h"
namespace Eayew {
    using buffer = base_buffer<mi_stl_allocator<char>>;
}
#else
namespace Eayew {
    using buffer = base_buffer<std::allocator<char>>;
    using buffer_ptr = std::shared_ptr<buffer>;
}
#endif

#endif