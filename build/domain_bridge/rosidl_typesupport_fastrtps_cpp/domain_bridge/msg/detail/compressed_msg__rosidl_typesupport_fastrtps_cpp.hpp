// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from domain_bridge:msg/CompressedMsg.idl
// generated code does not contain a copyright notice

#ifndef DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "domain_bridge/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "domain_bridge/msg/detail/compressed_msg__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace domain_bridge
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_domain_bridge
cdr_serialize(
  const domain_bridge::msg::CompressedMsg & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_domain_bridge
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  domain_bridge::msg::CompressedMsg & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_domain_bridge
get_serialized_size(
  const domain_bridge::msg::CompressedMsg & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_domain_bridge
max_serialized_size_CompressedMsg(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace domain_bridge

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_domain_bridge
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, domain_bridge, msg, CompressedMsg)();

#ifdef __cplusplus
}
#endif

#endif  // DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
