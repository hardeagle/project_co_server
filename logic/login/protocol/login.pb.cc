// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: login.proto

#include "login.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace LoginProtocol {
class C2S_LoginLoadDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<C2S_LoginLoad> _instance;
} _C2S_LoginLoad_default_instance_;
class S2C_LoginLoadDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<S2C_LoginLoad> _instance;
} _S2C_LoginLoad_default_instance_;
}  // namespace LoginProtocol
static void InitDefaultsscc_info_C2S_LoginLoad_login_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::LoginProtocol::_C2S_LoginLoad_default_instance_;
    new (ptr) ::LoginProtocol::C2S_LoginLoad();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::LoginProtocol::C2S_LoginLoad::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_C2S_LoginLoad_login_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_C2S_LoginLoad_login_2eproto}, {}};

static void InitDefaultsscc_info_S2C_LoginLoad_login_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::LoginProtocol::_S2C_LoginLoad_default_instance_;
    new (ptr) ::LoginProtocol::S2C_LoginLoad();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::LoginProtocol::S2C_LoginLoad::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_S2C_LoginLoad_login_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_S2C_LoginLoad_login_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_login_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_login_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_login_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_login_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::LoginProtocol::C2S_LoginLoad, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::LoginProtocol::C2S_LoginLoad, loginname_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::LoginProtocol::S2C_LoginLoad, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::LoginProtocol::S2C_LoginLoad, ret_),
  PROTOBUF_FIELD_OFFSET(::LoginProtocol::S2C_LoginLoad, role_id_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::LoginProtocol::C2S_LoginLoad)},
  { 6, -1, sizeof(::LoginProtocol::S2C_LoginLoad)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::LoginProtocol::_C2S_LoginLoad_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::LoginProtocol::_S2C_LoginLoad_default_instance_),
};

const char descriptor_table_protodef_login_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013login.proto\022\rLoginProtocol\"\"\n\rC2S_Logi"
  "nLoad\022\021\n\tloginname\030\001 \001(\014\"-\n\rS2C_LoginLoa"
  "d\022\013\n\003ret\030\001 \001(\005\022\017\n\007role_id\030\002 \001(\005b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_login_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_login_2eproto_sccs[2] = {
  &scc_info_C2S_LoginLoad_login_2eproto.base,
  &scc_info_S2C_LoginLoad_login_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_login_2eproto_once;
static bool descriptor_table_login_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_login_2eproto = {
  &descriptor_table_login_2eproto_initialized, descriptor_table_protodef_login_2eproto, "login.proto", 119,
  &descriptor_table_login_2eproto_once, descriptor_table_login_2eproto_sccs, descriptor_table_login_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_login_2eproto::offsets,
  file_level_metadata_login_2eproto, 2, file_level_enum_descriptors_login_2eproto, file_level_service_descriptors_login_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_login_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_login_2eproto), true);
namespace LoginProtocol {

// ===================================================================

void C2S_LoginLoad::InitAsDefaultInstance() {
}
class C2S_LoginLoad::_Internal {
 public:
};

C2S_LoginLoad::C2S_LoginLoad()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:LoginProtocol.C2S_LoginLoad)
}
C2S_LoginLoad::C2S_LoginLoad(const C2S_LoginLoad& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  loginname_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_loginname().empty()) {
    loginname_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.loginname_);
  }
  // @@protoc_insertion_point(copy_constructor:LoginProtocol.C2S_LoginLoad)
}

void C2S_LoginLoad::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_C2S_LoginLoad_login_2eproto.base);
  loginname_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

C2S_LoginLoad::~C2S_LoginLoad() {
  // @@protoc_insertion_point(destructor:LoginProtocol.C2S_LoginLoad)
  SharedDtor();
}

void C2S_LoginLoad::SharedDtor() {
  loginname_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void C2S_LoginLoad::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const C2S_LoginLoad& C2S_LoginLoad::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_C2S_LoginLoad_login_2eproto.base);
  return *internal_default_instance();
}


void C2S_LoginLoad::Clear() {
// @@protoc_insertion_point(message_clear_start:LoginProtocol.C2S_LoginLoad)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  loginname_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  _internal_metadata_.Clear();
}

const char* C2S_LoginLoad::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // bytes loginname = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(_internal_mutable_loginname(), ptr, ctx);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* C2S_LoginLoad::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:LoginProtocol.C2S_LoginLoad)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // bytes loginname = 1;
  if (this->loginname().size() > 0) {
    target = stream->WriteBytesMaybeAliased(
        1, this->_internal_loginname(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:LoginProtocol.C2S_LoginLoad)
  return target;
}

size_t C2S_LoginLoad::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:LoginProtocol.C2S_LoginLoad)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes loginname = 1;
  if (this->loginname().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_loginname());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void C2S_LoginLoad::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:LoginProtocol.C2S_LoginLoad)
  GOOGLE_DCHECK_NE(&from, this);
  const C2S_LoginLoad* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<C2S_LoginLoad>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:LoginProtocol.C2S_LoginLoad)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:LoginProtocol.C2S_LoginLoad)
    MergeFrom(*source);
  }
}

void C2S_LoginLoad::MergeFrom(const C2S_LoginLoad& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:LoginProtocol.C2S_LoginLoad)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.loginname().size() > 0) {

    loginname_.AssignWithDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from.loginname_);
  }
}

void C2S_LoginLoad::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:LoginProtocol.C2S_LoginLoad)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void C2S_LoginLoad::CopyFrom(const C2S_LoginLoad& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:LoginProtocol.C2S_LoginLoad)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool C2S_LoginLoad::IsInitialized() const {
  return true;
}

void C2S_LoginLoad::InternalSwap(C2S_LoginLoad* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  loginname_.Swap(&other->loginname_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
}

::PROTOBUF_NAMESPACE_ID::Metadata C2S_LoginLoad::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void S2C_LoginLoad::InitAsDefaultInstance() {
}
class S2C_LoginLoad::_Internal {
 public:
};

S2C_LoginLoad::S2C_LoginLoad()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:LoginProtocol.S2C_LoginLoad)
}
S2C_LoginLoad::S2C_LoginLoad(const S2C_LoginLoad& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&ret_, &from.ret_,
    static_cast<size_t>(reinterpret_cast<char*>(&role_id_) -
    reinterpret_cast<char*>(&ret_)) + sizeof(role_id_));
  // @@protoc_insertion_point(copy_constructor:LoginProtocol.S2C_LoginLoad)
}

void S2C_LoginLoad::SharedCtor() {
  ::memset(&ret_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&role_id_) -
      reinterpret_cast<char*>(&ret_)) + sizeof(role_id_));
}

S2C_LoginLoad::~S2C_LoginLoad() {
  // @@protoc_insertion_point(destructor:LoginProtocol.S2C_LoginLoad)
  SharedDtor();
}

void S2C_LoginLoad::SharedDtor() {
}

void S2C_LoginLoad::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const S2C_LoginLoad& S2C_LoginLoad::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_S2C_LoginLoad_login_2eproto.base);
  return *internal_default_instance();
}


void S2C_LoginLoad::Clear() {
// @@protoc_insertion_point(message_clear_start:LoginProtocol.S2C_LoginLoad)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&ret_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&role_id_) -
      reinterpret_cast<char*>(&ret_)) + sizeof(role_id_));
  _internal_metadata_.Clear();
}

const char* S2C_LoginLoad::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 ret = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ret_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 role_id = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          role_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* S2C_LoginLoad::InternalSerializeWithCachedSizesToArray(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:LoginProtocol.S2C_LoginLoad)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 ret = 1;
  if (this->ret() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_ret(), target);
  }

  // int32 role_id = 2;
  if (this->role_id() != 0) {
    stream->EnsureSpace(&target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_role_id(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:LoginProtocol.S2C_LoginLoad)
  return target;
}

size_t S2C_LoginLoad::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:LoginProtocol.S2C_LoginLoad)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 ret = 1;
  if (this->ret() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_ret());
  }

  // int32 role_id = 2;
  if (this->role_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_role_id());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void S2C_LoginLoad::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:LoginProtocol.S2C_LoginLoad)
  GOOGLE_DCHECK_NE(&from, this);
  const S2C_LoginLoad* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<S2C_LoginLoad>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:LoginProtocol.S2C_LoginLoad)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:LoginProtocol.S2C_LoginLoad)
    MergeFrom(*source);
  }
}

void S2C_LoginLoad::MergeFrom(const S2C_LoginLoad& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:LoginProtocol.S2C_LoginLoad)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.ret() != 0) {
    _internal_set_ret(from._internal_ret());
  }
  if (from.role_id() != 0) {
    _internal_set_role_id(from._internal_role_id());
  }
}

void S2C_LoginLoad::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:LoginProtocol.S2C_LoginLoad)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void S2C_LoginLoad::CopyFrom(const S2C_LoginLoad& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:LoginProtocol.S2C_LoginLoad)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S2C_LoginLoad::IsInitialized() const {
  return true;
}

void S2C_LoginLoad::InternalSwap(S2C_LoginLoad* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(ret_, other->ret_);
  swap(role_id_, other->role_id_);
}

::PROTOBUF_NAMESPACE_ID::Metadata S2C_LoginLoad::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace LoginProtocol
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::LoginProtocol::C2S_LoginLoad* Arena::CreateMaybeMessage< ::LoginProtocol::C2S_LoginLoad >(Arena* arena) {
  return Arena::CreateInternal< ::LoginProtocol::C2S_LoginLoad >(arena);
}
template<> PROTOBUF_NOINLINE ::LoginProtocol::S2C_LoginLoad* Arena::CreateMaybeMessage< ::LoginProtocol::S2C_LoginLoad >(Arena* arena) {
  return Arena::CreateInternal< ::LoginProtocol::S2C_LoginLoad >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
