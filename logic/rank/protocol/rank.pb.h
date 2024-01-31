// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rank.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_rank_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_rank_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_rank_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_rank_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[5]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_rank_2eproto;
namespace RankProtocol {
class C2S_RankLoad;
class C2S_RankLoadDefaultTypeInternal;
extern C2S_RankLoadDefaultTypeInternal _C2S_RankLoad_default_instance_;
class C2S_RankUpdate;
class C2S_RankUpdateDefaultTypeInternal;
extern C2S_RankUpdateDefaultTypeInternal _C2S_RankUpdate_default_instance_;
class RankRoleInfo;
class RankRoleInfoDefaultTypeInternal;
extern RankRoleInfoDefaultTypeInternal _RankRoleInfo_default_instance_;
class S2C_RankLoad;
class S2C_RankLoadDefaultTypeInternal;
extern S2C_RankLoadDefaultTypeInternal _S2C_RankLoad_default_instance_;
class S2C_RankUpdate;
class S2C_RankUpdateDefaultTypeInternal;
extern S2C_RankUpdateDefaultTypeInternal _S2C_RankUpdate_default_instance_;
}  // namespace RankProtocol
PROTOBUF_NAMESPACE_OPEN
template<> ::RankProtocol::C2S_RankLoad* Arena::CreateMaybeMessage<::RankProtocol::C2S_RankLoad>(Arena*);
template<> ::RankProtocol::C2S_RankUpdate* Arena::CreateMaybeMessage<::RankProtocol::C2S_RankUpdate>(Arena*);
template<> ::RankProtocol::RankRoleInfo* Arena::CreateMaybeMessage<::RankProtocol::RankRoleInfo>(Arena*);
template<> ::RankProtocol::S2C_RankLoad* Arena::CreateMaybeMessage<::RankProtocol::S2C_RankLoad>(Arena*);
template<> ::RankProtocol::S2C_RankUpdate* Arena::CreateMaybeMessage<::RankProtocol::S2C_RankUpdate>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace RankProtocol {

// ===================================================================

class RankRoleInfo :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RankProtocol.RankRoleInfo) */ {
 public:
  RankRoleInfo();
  virtual ~RankRoleInfo();

  RankRoleInfo(const RankRoleInfo& from);
  RankRoleInfo(RankRoleInfo&& from) noexcept
    : RankRoleInfo() {
    *this = ::std::move(from);
  }

  inline RankRoleInfo& operator=(const RankRoleInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline RankRoleInfo& operator=(RankRoleInfo&& from) noexcept {
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
  static const RankRoleInfo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const RankRoleInfo* internal_default_instance() {
    return reinterpret_cast<const RankRoleInfo*>(
               &_RankRoleInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(RankRoleInfo& a, RankRoleInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(RankRoleInfo* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline RankRoleInfo* New() const final {
    return CreateMaybeMessage<RankRoleInfo>(nullptr);
  }

  RankRoleInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<RankRoleInfo>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const RankRoleInfo& from);
  void MergeFrom(const RankRoleInfo& from);
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
  void InternalSwap(RankRoleInfo* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RankProtocol.RankRoleInfo";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_rank_2eproto);
    return ::descriptor_table_rank_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:RankProtocol.RankRoleInfo)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_rank_2eproto;
};
// -------------------------------------------------------------------

class C2S_RankLoad :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RankProtocol.C2S_RankLoad) */ {
 public:
  C2S_RankLoad();
  virtual ~C2S_RankLoad();

  C2S_RankLoad(const C2S_RankLoad& from);
  C2S_RankLoad(C2S_RankLoad&& from) noexcept
    : C2S_RankLoad() {
    *this = ::std::move(from);
  }

  inline C2S_RankLoad& operator=(const C2S_RankLoad& from) {
    CopyFrom(from);
    return *this;
  }
  inline C2S_RankLoad& operator=(C2S_RankLoad&& from) noexcept {
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
  static const C2S_RankLoad& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const C2S_RankLoad* internal_default_instance() {
    return reinterpret_cast<const C2S_RankLoad*>(
               &_C2S_RankLoad_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(C2S_RankLoad& a, C2S_RankLoad& b) {
    a.Swap(&b);
  }
  inline void Swap(C2S_RankLoad* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline C2S_RankLoad* New() const final {
    return CreateMaybeMessage<C2S_RankLoad>(nullptr);
  }

  C2S_RankLoad* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<C2S_RankLoad>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const C2S_RankLoad& from);
  void MergeFrom(const C2S_RankLoad& from);
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
  void InternalSwap(C2S_RankLoad* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RankProtocol.C2S_RankLoad";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_rank_2eproto);
    return ::descriptor_table_rank_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:RankProtocol.C2S_RankLoad)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_rank_2eproto;
};
// -------------------------------------------------------------------

class S2C_RankLoad :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RankProtocol.S2C_RankLoad) */ {
 public:
  S2C_RankLoad();
  virtual ~S2C_RankLoad();

  S2C_RankLoad(const S2C_RankLoad& from);
  S2C_RankLoad(S2C_RankLoad&& from) noexcept
    : S2C_RankLoad() {
    *this = ::std::move(from);
  }

  inline S2C_RankLoad& operator=(const S2C_RankLoad& from) {
    CopyFrom(from);
    return *this;
  }
  inline S2C_RankLoad& operator=(S2C_RankLoad&& from) noexcept {
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
  static const S2C_RankLoad& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const S2C_RankLoad* internal_default_instance() {
    return reinterpret_cast<const S2C_RankLoad*>(
               &_S2C_RankLoad_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(S2C_RankLoad& a, S2C_RankLoad& b) {
    a.Swap(&b);
  }
  inline void Swap(S2C_RankLoad* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline S2C_RankLoad* New() const final {
    return CreateMaybeMessage<S2C_RankLoad>(nullptr);
  }

  S2C_RankLoad* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<S2C_RankLoad>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const S2C_RankLoad& from);
  void MergeFrom(const S2C_RankLoad& from);
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
  void InternalSwap(S2C_RankLoad* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RankProtocol.S2C_RankLoad";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_rank_2eproto);
    return ::descriptor_table_rank_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRrisFieldNumber = 3,
    kMyselfFieldNumber = 2,
    kRetFieldNumber = 1,
  };
  // repeated .RankProtocol.RankRoleInfo rris = 3;
  int rris_size() const;
  private:
  int _internal_rris_size() const;
  public:
  void clear_rris();
  ::RankProtocol::RankRoleInfo* mutable_rris(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::RankProtocol::RankRoleInfo >*
      mutable_rris();
  private:
  const ::RankProtocol::RankRoleInfo& _internal_rris(int index) const;
  ::RankProtocol::RankRoleInfo* _internal_add_rris();
  public:
  const ::RankProtocol::RankRoleInfo& rris(int index) const;
  ::RankProtocol::RankRoleInfo* add_rris();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::RankProtocol::RankRoleInfo >&
      rris() const;

  // .RankProtocol.RankRoleInfo myself = 2;
  bool has_myself() const;
  private:
  bool _internal_has_myself() const;
  public:
  void clear_myself();
  const ::RankProtocol::RankRoleInfo& myself() const;
  ::RankProtocol::RankRoleInfo* release_myself();
  ::RankProtocol::RankRoleInfo* mutable_myself();
  void set_allocated_myself(::RankProtocol::RankRoleInfo* myself);
  private:
  const ::RankProtocol::RankRoleInfo& _internal_myself() const;
  ::RankProtocol::RankRoleInfo* _internal_mutable_myself();
  public:

  // int32 ret = 1;
  void clear_ret();
  ::PROTOBUF_NAMESPACE_ID::int32 ret() const;
  void set_ret(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_ret() const;
  void _internal_set_ret(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:RankProtocol.S2C_RankLoad)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::RankProtocol::RankRoleInfo > rris_;
  ::RankProtocol::RankRoleInfo* myself_;
  ::PROTOBUF_NAMESPACE_ID::int32 ret_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_rank_2eproto;
};
// -------------------------------------------------------------------

class C2S_RankUpdate :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RankProtocol.C2S_RankUpdate) */ {
 public:
  C2S_RankUpdate();
  virtual ~C2S_RankUpdate();

  C2S_RankUpdate(const C2S_RankUpdate& from);
  C2S_RankUpdate(C2S_RankUpdate&& from) noexcept
    : C2S_RankUpdate() {
    *this = ::std::move(from);
  }

  inline C2S_RankUpdate& operator=(const C2S_RankUpdate& from) {
    CopyFrom(from);
    return *this;
  }
  inline C2S_RankUpdate& operator=(C2S_RankUpdate&& from) noexcept {
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
  static const C2S_RankUpdate& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const C2S_RankUpdate* internal_default_instance() {
    return reinterpret_cast<const C2S_RankUpdate*>(
               &_C2S_RankUpdate_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  friend void swap(C2S_RankUpdate& a, C2S_RankUpdate& b) {
    a.Swap(&b);
  }
  inline void Swap(C2S_RankUpdate* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline C2S_RankUpdate* New() const final {
    return CreateMaybeMessage<C2S_RankUpdate>(nullptr);
  }

  C2S_RankUpdate* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<C2S_RankUpdate>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const C2S_RankUpdate& from);
  void MergeFrom(const C2S_RankUpdate& from);
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
  void InternalSwap(C2S_RankUpdate* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RankProtocol.C2S_RankUpdate";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_rank_2eproto);
    return ::descriptor_table_rank_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kScoreFieldNumber = 1,
  };
  // int32 score = 1;
  void clear_score();
  ::PROTOBUF_NAMESPACE_ID::int32 score() const;
  void set_score(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_score() const;
  void _internal_set_score(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:RankProtocol.C2S_RankUpdate)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::int32 score_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_rank_2eproto;
};
// -------------------------------------------------------------------

class S2C_RankUpdate :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RankProtocol.S2C_RankUpdate) */ {
 public:
  S2C_RankUpdate();
  virtual ~S2C_RankUpdate();

  S2C_RankUpdate(const S2C_RankUpdate& from);
  S2C_RankUpdate(S2C_RankUpdate&& from) noexcept
    : S2C_RankUpdate() {
    *this = ::std::move(from);
  }

  inline S2C_RankUpdate& operator=(const S2C_RankUpdate& from) {
    CopyFrom(from);
    return *this;
  }
  inline S2C_RankUpdate& operator=(S2C_RankUpdate&& from) noexcept {
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
  static const S2C_RankUpdate& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const S2C_RankUpdate* internal_default_instance() {
    return reinterpret_cast<const S2C_RankUpdate*>(
               &_S2C_RankUpdate_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    4;

  friend void swap(S2C_RankUpdate& a, S2C_RankUpdate& b) {
    a.Swap(&b);
  }
  inline void Swap(S2C_RankUpdate* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline S2C_RankUpdate* New() const final {
    return CreateMaybeMessage<S2C_RankUpdate>(nullptr);
  }

  S2C_RankUpdate* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<S2C_RankUpdate>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const S2C_RankUpdate& from);
  void MergeFrom(const S2C_RankUpdate& from);
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
  void InternalSwap(S2C_RankUpdate* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RankProtocol.S2C_RankUpdate";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_rank_2eproto);
    return ::descriptor_table_rank_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRetFieldNumber = 1,
  };
  // int32 ret = 1;
  void clear_ret();
  ::PROTOBUF_NAMESPACE_ID::int32 ret() const;
  void set_ret(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_ret() const;
  void _internal_set_ret(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:RankProtocol.S2C_RankUpdate)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::int32 ret_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_rank_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RankRoleInfo

// -------------------------------------------------------------------

// C2S_RankLoad

// -------------------------------------------------------------------

// S2C_RankLoad

// int32 ret = 1;
inline void S2C_RankLoad::clear_ret() {
  ret_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 S2C_RankLoad::_internal_ret() const {
  return ret_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 S2C_RankLoad::ret() const {
  // @@protoc_insertion_point(field_get:RankProtocol.S2C_RankLoad.ret)
  return _internal_ret();
}
inline void S2C_RankLoad::_internal_set_ret(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  ret_ = value;
}
inline void S2C_RankLoad::set_ret(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_ret(value);
  // @@protoc_insertion_point(field_set:RankProtocol.S2C_RankLoad.ret)
}

// .RankProtocol.RankRoleInfo myself = 2;
inline bool S2C_RankLoad::_internal_has_myself() const {
  return this != internal_default_instance() && myself_ != nullptr;
}
inline bool S2C_RankLoad::has_myself() const {
  return _internal_has_myself();
}
inline void S2C_RankLoad::clear_myself() {
  if (GetArenaNoVirtual() == nullptr && myself_ != nullptr) {
    delete myself_;
  }
  myself_ = nullptr;
}
inline const ::RankProtocol::RankRoleInfo& S2C_RankLoad::_internal_myself() const {
  const ::RankProtocol::RankRoleInfo* p = myself_;
  return p != nullptr ? *p : *reinterpret_cast<const ::RankProtocol::RankRoleInfo*>(
      &::RankProtocol::_RankRoleInfo_default_instance_);
}
inline const ::RankProtocol::RankRoleInfo& S2C_RankLoad::myself() const {
  // @@protoc_insertion_point(field_get:RankProtocol.S2C_RankLoad.myself)
  return _internal_myself();
}
inline ::RankProtocol::RankRoleInfo* S2C_RankLoad::release_myself() {
  // @@protoc_insertion_point(field_release:RankProtocol.S2C_RankLoad.myself)
  
  ::RankProtocol::RankRoleInfo* temp = myself_;
  myself_ = nullptr;
  return temp;
}
inline ::RankProtocol::RankRoleInfo* S2C_RankLoad::_internal_mutable_myself() {
  
  if (myself_ == nullptr) {
    auto* p = CreateMaybeMessage<::RankProtocol::RankRoleInfo>(GetArenaNoVirtual());
    myself_ = p;
  }
  return myself_;
}
inline ::RankProtocol::RankRoleInfo* S2C_RankLoad::mutable_myself() {
  // @@protoc_insertion_point(field_mutable:RankProtocol.S2C_RankLoad.myself)
  return _internal_mutable_myself();
}
inline void S2C_RankLoad::set_allocated_myself(::RankProtocol::RankRoleInfo* myself) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete myself_;
  }
  if (myself) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      myself = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, myself, submessage_arena);
    }
    
  } else {
    
  }
  myself_ = myself;
  // @@protoc_insertion_point(field_set_allocated:RankProtocol.S2C_RankLoad.myself)
}

// repeated .RankProtocol.RankRoleInfo rris = 3;
inline int S2C_RankLoad::_internal_rris_size() const {
  return rris_.size();
}
inline int S2C_RankLoad::rris_size() const {
  return _internal_rris_size();
}
inline void S2C_RankLoad::clear_rris() {
  rris_.Clear();
}
inline ::RankProtocol::RankRoleInfo* S2C_RankLoad::mutable_rris(int index) {
  // @@protoc_insertion_point(field_mutable:RankProtocol.S2C_RankLoad.rris)
  return rris_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::RankProtocol::RankRoleInfo >*
S2C_RankLoad::mutable_rris() {
  // @@protoc_insertion_point(field_mutable_list:RankProtocol.S2C_RankLoad.rris)
  return &rris_;
}
inline const ::RankProtocol::RankRoleInfo& S2C_RankLoad::_internal_rris(int index) const {
  return rris_.Get(index);
}
inline const ::RankProtocol::RankRoleInfo& S2C_RankLoad::rris(int index) const {
  // @@protoc_insertion_point(field_get:RankProtocol.S2C_RankLoad.rris)
  return _internal_rris(index);
}
inline ::RankProtocol::RankRoleInfo* S2C_RankLoad::_internal_add_rris() {
  return rris_.Add();
}
inline ::RankProtocol::RankRoleInfo* S2C_RankLoad::add_rris() {
  // @@protoc_insertion_point(field_add:RankProtocol.S2C_RankLoad.rris)
  return _internal_add_rris();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::RankProtocol::RankRoleInfo >&
S2C_RankLoad::rris() const {
  // @@protoc_insertion_point(field_list:RankProtocol.S2C_RankLoad.rris)
  return rris_;
}

// -------------------------------------------------------------------

// C2S_RankUpdate

// int32 score = 1;
inline void C2S_RankUpdate::clear_score() {
  score_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 C2S_RankUpdate::_internal_score() const {
  return score_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 C2S_RankUpdate::score() const {
  // @@protoc_insertion_point(field_get:RankProtocol.C2S_RankUpdate.score)
  return _internal_score();
}
inline void C2S_RankUpdate::_internal_set_score(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  score_ = value;
}
inline void C2S_RankUpdate::set_score(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_score(value);
  // @@protoc_insertion_point(field_set:RankProtocol.C2S_RankUpdate.score)
}

// -------------------------------------------------------------------

// S2C_RankUpdate

// int32 ret = 1;
inline void S2C_RankUpdate::clear_ret() {
  ret_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 S2C_RankUpdate::_internal_ret() const {
  return ret_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 S2C_RankUpdate::ret() const {
  // @@protoc_insertion_point(field_get:RankProtocol.S2C_RankUpdate.ret)
  return _internal_ret();
}
inline void S2C_RankUpdate::_internal_set_ret(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  ret_ = value;
}
inline void S2C_RankUpdate::set_ret(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_ret(value);
  // @@protoc_insertion_point(field_set:RankProtocol.S2C_RankUpdate.ret)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace RankProtocol

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_rank_2eproto
