// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from domain_bridge:msg/CompressedMsg.idl
// generated code does not contain a copyright notice

#ifndef DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__STRUCT_H_
#define DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'data'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/CompressedMsg in the package domain_bridge.
typedef struct domain_bridge__msg__CompressedMsg
{
  rosidl_runtime_c__octet__Sequence data;
} domain_bridge__msg__CompressedMsg;

// Struct for a sequence of domain_bridge__msg__CompressedMsg.
typedef struct domain_bridge__msg__CompressedMsg__Sequence
{
  domain_bridge__msg__CompressedMsg * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} domain_bridge__msg__CompressedMsg__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__STRUCT_H_
