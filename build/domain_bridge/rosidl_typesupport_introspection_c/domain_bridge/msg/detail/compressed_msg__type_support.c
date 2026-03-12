// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from domain_bridge:msg/CompressedMsg.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "domain_bridge/msg/detail/compressed_msg__rosidl_typesupport_introspection_c.h"
#include "domain_bridge/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "domain_bridge/msg/detail/compressed_msg__functions.h"
#include "domain_bridge/msg/detail/compressed_msg__struct.h"


// Include directives for member types
// Member `data`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  domain_bridge__msg__CompressedMsg__init(message_memory);
}

void domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_fini_function(void * message_memory)
{
  domain_bridge__msg__CompressedMsg__fini(message_memory);
}

size_t domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__size_function__CompressedMsg__data(
  const void * untyped_member)
{
  const rosidl_runtime_c__octet__Sequence * member =
    (const rosidl_runtime_c__octet__Sequence *)(untyped_member);
  return member->size;
}

const void * domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__get_const_function__CompressedMsg__data(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__octet__Sequence * member =
    (const rosidl_runtime_c__octet__Sequence *)(untyped_member);
  return &member->data[index];
}

void * domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__get_function__CompressedMsg__data(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__octet__Sequence * member =
    (rosidl_runtime_c__octet__Sequence *)(untyped_member);
  return &member->data[index];
}

void domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__fetch_function__CompressedMsg__data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__get_const_function__CompressedMsg__data(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__assign_function__CompressedMsg__data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__get_function__CompressedMsg__data(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

bool domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__resize_function__CompressedMsg__data(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__octet__Sequence * member =
    (rosidl_runtime_c__octet__Sequence *)(untyped_member);
  rosidl_runtime_c__octet__Sequence__fini(member);
  return rosidl_runtime_c__octet__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_message_member_array[1] = {
  {
    "data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_OCTET,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(domain_bridge__msg__CompressedMsg, data),  // bytes offset in struct
    NULL,  // default value
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__size_function__CompressedMsg__data,  // size() function pointer
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__get_const_function__CompressedMsg__data,  // get_const(index) function pointer
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__get_function__CompressedMsg__data,  // get(index) function pointer
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__fetch_function__CompressedMsg__data,  // fetch(index, &value) function pointer
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__assign_function__CompressedMsg__data,  // assign(index, value) function pointer
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__resize_function__CompressedMsg__data  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_message_members = {
  "domain_bridge__msg",  // message namespace
  "CompressedMsg",  // message name
  1,  // number of fields
  sizeof(domain_bridge__msg__CompressedMsg),
  domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_message_member_array,  // message members
  domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_init_function,  // function to initialize message memory (memory has to be allocated)
  domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_message_type_support_handle = {
  0,
  &domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_domain_bridge
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, domain_bridge, msg, CompressedMsg)() {
  if (!domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_message_type_support_handle.typesupport_identifier) {
    domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &domain_bridge__msg__CompressedMsg__rosidl_typesupport_introspection_c__CompressedMsg_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
