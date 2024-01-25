// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: public.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_public_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_public_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3010000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3010000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_public_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_public_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_public_2eproto;
namespace PublicProtocol {
class BaseRoleInfo;
class BaseRoleInfoDefaultTypeInternal;
extern BaseRoleInfoDefaultTypeInternal _BaseRoleInfo_default_instance_;
}  // namespace PublicProtocol
PROTOBUF_NAMESPACE_OPEN
template<> ::PublicProtocol::BaseRoleInfo* Arena::CreateMaybeMessage<::PublicProtocol::BaseRoleInfo>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace PublicProtocol {

// ===================================================================

class BaseRoleInfo :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PublicProtocol.BaseRoleInfo) */ {
 public:
  BaseRoleInfo();
  virtual ~BaseRoleInfo();

  BaseRoleInfo(const BaseRoleInfo& from);
  BaseRoleInfo(BaseRoleInfo&& from) noexcept
    : BaseRoleInfo() {
    *this = ::std::move(from);
  }

  inline BaseRoleInfo& operator=(const BaseRoleInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline BaseRoleInfo& operator=(BaseRoleInfo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const BaseRoleInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const BaseRoleInfo* internal_default_instance() {
    return reinterpret_cast<const BaseRoleInfo*>(
               &_BaseRoleInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(BaseRoleInfo& a, BaseRoleInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(BaseRoleInfo* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline BaseRoleInfo* New() const final {
    return CreateMaybeMessage<BaseRoleInfo>(nullptr);
  }

  BaseRoleInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<BaseRoleInfo>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const BaseRoleInfo& from);
  void MergeFrom(const BaseRoleInfo& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(BaseRoleInfo* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PublicProtocol.BaseRoleInfo";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_public_2eproto);
    return ::descriptor_table_public_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 2,
    kAvatarurlFieldNumber = 3,
    kRoleIdFieldNumber = 1,
    kLogouttsFieldNumber = 5,
    kOnlineFieldNumber = 4,
  };
  // bytes name = 2;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const void* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // bytes avatarurl = 3;
  void clear_avatarurl();
  const std::string& avatarurl() const;
  void set_avatarurl(const std::string& value);
  void set_avatarurl(std::string&& value);
  void set_avatarurl(const char* value);
  void set_avatarurl(const void* value, size_t size);
  std::string* mutable_avatarurl();
  std::string* release_avatarurl();
  void set_allocated_avatarurl(std::string* avatarurl);
  private:
  const std::string& _internal_avatarurl() const;
  void _internal_set_avatarurl(const std::string& value);
  std::string* _internal_mutable_avatarurl();
  public:

  // uint64 role_id = 1;
  void clear_role_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 role_id() const;
  void set_role_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_role_id() const;
  void _internal_set_role_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 logoutts = 5;
  void clear_logoutts();
  ::PROTOBUF_NAMESPACE_ID::uint64 logoutts() const;
  void set_logoutts(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_logoutts() const;
  void _internal_set_logoutts(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // bool online = 4;
  void clear_online();
  bool online() const;
  void set_online(bool value);
  private:
  bool _internal_online() const;
  void _internal_set_online(bool value);
  public:

  // @@protoc_insertion_point(class_scope:PublicProtocol.BaseRoleInfo)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr avatarurl_;
  ::PROTOBUF_NAMESPACE_ID::uint64 role_id_;
  ::PROTOBUF_NAMESPACE_ID::uint64 logoutts_;
  bool online_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_public_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// BaseRoleInfo

// uint64 role_id = 1;
inline void BaseRoleInfo::clear_role_id() {
  role_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 BaseRoleInfo::_internal_role_id() const {
  return role_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 BaseRoleInfo::role_id() const {
  // @@protoc_insertion_point(field_get:PublicProtocol.BaseRoleInfo.role_id)
  return _internal_role_id();
}
inline void BaseRoleInfo::_internal_set_role_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  role_id_ = value;
}
inline void BaseRoleInfo::set_role_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_role_id(value);
  // @@protoc_insertion_point(field_set:PublicProtocol.BaseRoleInfo.role_id)
}

// bytes name = 2;
inline void BaseRoleInfo::clear_name() {
  name_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& BaseRoleInfo::name() const {
  // @@protoc_insertion_point(field_get:PublicProtocol.BaseRoleInfo.name)
  return _internal_name();
}
inline void BaseRoleInfo::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:PublicProtocol.BaseRoleInfo.name)
}
inline std::string* BaseRoleInfo::mutable_name() {
  // @@protoc_insertion_point(field_mutable:PublicProtocol.BaseRoleInfo.name)
  return _internal_mutable_name();
}
inline const std::string& BaseRoleInfo::_internal_name() const {
  return name_.GetNoArena();
}
inline void BaseRoleInfo::_internal_set_name(const std::string& value) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void BaseRoleInfo::set_name(std::string&& value) {
  
  name_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:PublicProtocol.BaseRoleInfo.name)
}
inline void BaseRoleInfo::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:PublicProtocol.BaseRoleInfo.name)
}
inline void BaseRoleInfo::set_name(const void* value, size_t size) {
  
  name_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:PublicProtocol.BaseRoleInfo.name)
}
inline std::string* BaseRoleInfo::_internal_mutable_name() {
  
  return name_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* BaseRoleInfo::release_name() {
  // @@protoc_insertion_point(field_release:PublicProtocol.BaseRoleInfo.name)
  
  return name_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void BaseRoleInfo::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:PublicProtocol.BaseRoleInfo.name)
}

// bytes avatarurl = 3;
inline void BaseRoleInfo::clear_avatarurl() {
  avatarurl_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& BaseRoleInfo::avatarurl() const {
  // @@protoc_insertion_point(field_get:PublicProtocol.BaseRoleInfo.avatarurl)
  return _internal_avatarurl();
}
inline void BaseRoleInfo::set_avatarurl(const std::string& value) {
  _internal_set_avatarurl(value);
  // @@protoc_insertion_point(field_set:PublicProtocol.BaseRoleInfo.avatarurl)
}
inline std::string* BaseRoleInfo::mutable_avatarurl() {
  // @@protoc_insertion_point(field_mutable:PublicProtocol.BaseRoleInfo.avatarurl)
  return _internal_mutable_avatarurl();
}
inline const std::string& BaseRoleInfo::_internal_avatarurl() const {
  return avatarurl_.GetNoArena();
}
inline void BaseRoleInfo::_internal_set_avatarurl(const std::string& value) {
  
  avatarurl_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void BaseRoleInfo::set_avatarurl(std::string&& value) {
  
  avatarurl_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:PublicProtocol.BaseRoleInfo.avatarurl)
}
inline void BaseRoleInfo::set_avatarurl(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  avatarurl_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:PublicProtocol.BaseRoleInfo.avatarurl)
}
inline void BaseRoleInfo::set_avatarurl(const void* value, size_t size) {
  
  avatarurl_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:PublicProtocol.BaseRoleInfo.avatarurl)
}
inline std::string* BaseRoleInfo::_internal_mutable_avatarurl() {
  
  return avatarurl_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* BaseRoleInfo::release_avatarurl() {
  // @@protoc_insertion_point(field_release:PublicProtocol.BaseRoleInfo.avatarurl)
  
  return avatarurl_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void BaseRoleInfo::set_allocated_avatarurl(std::string* avatarurl) {
  if (avatarurl != nullptr) {
    
  } else {
    
  }
  avatarurl_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), avatarurl);
  // @@protoc_insertion_point(field_set_allocated:PublicProtocol.BaseRoleInfo.avatarurl)
}

// bool online = 4;
inline void BaseRoleInfo::clear_online() {
  online_ = false;
}
inline bool BaseRoleInfo::_internal_online() const {
  return online_;
}
inline bool BaseRoleInfo::online() const {
  // @@protoc_insertion_point(field_get:PublicProtocol.BaseRoleInfo.online)
  return _internal_online();
}
inline void BaseRoleInfo::_internal_set_online(bool value) {
  
  online_ = value;
}
inline void BaseRoleInfo::set_online(bool value) {
  _internal_set_online(value);
  // @@protoc_insertion_point(field_set:PublicProtocol.BaseRoleInfo.online)
}

// uint64 logoutts = 5;
inline void BaseRoleInfo::clear_logoutts() {
  logoutts_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 BaseRoleInfo::_internal_logoutts() const {
  return logoutts_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 BaseRoleInfo::logoutts() const {
  // @@protoc_insertion_point(field_get:PublicProtocol.BaseRoleInfo.logoutts)
  return _internal_logoutts();
}
inline void BaseRoleInfo::_internal_set_logoutts(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  logoutts_ = value;
}
inline void BaseRoleInfo::set_logoutts(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_logoutts(value);
  // @@protoc_insertion_point(field_set:PublicProtocol.BaseRoleInfo.logoutts)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace PublicProtocol

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_public_2eproto
