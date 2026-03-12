// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from domain_bridge:msg/CompressedMsg.idl
// generated code does not contain a copyright notice

#ifndef DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__BUILDER_HPP_
#define DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "domain_bridge/msg/detail/compressed_msg__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace domain_bridge
{

namespace msg
{

namespace builder
{

class Init_CompressedMsg_data
{
public:
  Init_CompressedMsg_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::domain_bridge::msg::CompressedMsg data(::domain_bridge::msg::CompressedMsg::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::domain_bridge::msg::CompressedMsg msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::domain_bridge::msg::CompressedMsg>()
{
  return domain_bridge::msg::builder::Init_CompressedMsg_data();
}

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__BUILDER_HPP_
