// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: login_id.proto

#include "login_id.pb.h"

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
}  // namespace LoginProtocol
static constexpr ::PROTOBUF_NAMESPACE_ID::Metadata* file_level_metadata_login_5fid_2eproto = nullptr;
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_login_5fid_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_login_5fid_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_login_5fid_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_login_5fid_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016login_id.proto\022\rLoginProtocol*\210\002\n\002ID\022\010"
  "\n\004ZERO\020\000\022\023\n\rLOGIN_ID_BASE\020\200\200\010\022\024\n\016C2S_LOG"
  "IN_BASE\020\350\207\010\022\025\n\017C2S_LOGIN_LOGIN\020\351\207\010\022\025\n\017S2"
  "C_LOGIN_LOGIN\020\352\207\010\022\026\n\020C2S_LOGIN_CREATE\020\353\207"
  "\010\022\026\n\020S2C_LOGIN_CREATE\020\354\207\010\022\024\n\016C2S_LOGIN_L"
  "OAD\020\355\207\010\022\024\n\016S2C_LOGIN_LOAD\020\356\207\010\022\026\n\020C2S_LOG"
  "IN_OPENID\020\357\207\010\022\026\n\020S2C_LOGIN_OPENID\020\360\207\010\022\023\n"
  "\rC2S_LOGIN_TOP\020\317\217\010b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_login_5fid_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_login_5fid_2eproto_sccs[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_login_5fid_2eproto_once;
static bool descriptor_table_login_5fid_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_login_5fid_2eproto = {
  &descriptor_table_login_5fid_2eproto_initialized, descriptor_table_protodef_login_5fid_2eproto, "login_id.proto", 306,
  &descriptor_table_login_5fid_2eproto_once, descriptor_table_login_5fid_2eproto_sccs, descriptor_table_login_5fid_2eproto_deps, 0, 0,
  schemas, file_default_instances, TableStruct_login_5fid_2eproto::offsets,
  file_level_metadata_login_5fid_2eproto, 0, file_level_enum_descriptors_login_5fid_2eproto, file_level_service_descriptors_login_5fid_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_login_5fid_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_login_5fid_2eproto), true);
namespace LoginProtocol {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ID_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_login_5fid_2eproto);
  return file_level_enum_descriptors_login_5fid_2eproto[0];
}
bool ID_IsValid(int value) {
  switch (value) {
    case 0:
    case 131072:
    case 132072:
    case 132073:
    case 132074:
    case 132075:
    case 132076:
    case 132077:
    case 132078:
    case 132079:
    case 132080:
    case 133071:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace LoginProtocol
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
