// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from domain_bridge:msg/CompressedMsg.idl
// generated code does not contain a copyright notice

#ifndef DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__STRUCT_HPP_
#define DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__domain_bridge__msg__CompressedMsg __attribute__((deprecated))
#else
# define DEPRECATED__domain_bridge__msg__CompressedMsg __declspec(deprecated)
#endif

namespace domain_bridge
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct CompressedMsg_
{
  using Type = CompressedMsg_<ContainerAllocator>;

  explicit CompressedMsg_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
  }

  explicit CompressedMsg_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_init;
    (void)_alloc;
  }

  // field types and members
  using _data_type =
    std::vector<unsigned char, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<unsigned char>>;
  _data_type data;

  // setters for named parameter idiom
  Type & set__data(
    const std::vector<unsigned char, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<unsigned char>> & _arg)
  {
    this->data = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    domain_bridge::msg::CompressedMsg_<ContainerAllocator> *;
  using ConstRawPtr =
    const domain_bridge::msg::CompressedMsg_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<domain_bridge::msg::CompressedMsg_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<domain_bridge::msg::CompressedMsg_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      domain_bridge::msg::CompressedMsg_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<domain_bridge::msg::CompressedMsg_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      domain_bridge::msg::CompressedMsg_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<domain_bridge::msg::CompressedMsg_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<domain_bridge::msg::CompressedMsg_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<domain_bridge::msg::CompressedMsg_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__domain_bridge__msg__CompressedMsg
    std::shared_ptr<domain_bridge::msg::CompressedMsg_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__domain_bridge__msg__CompressedMsg
    std::shared_ptr<domain_bridge::msg::CompressedMsg_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const CompressedMsg_ & other) const
  {
    if (this->data != other.data) {
      return false;
    }
    return true;
  }
  bool operator!=(const CompressedMsg_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct CompressedMsg_

// alias to use template instance with default allocator
using CompressedMsg =
  domain_bridge::msg::CompressedMsg_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace domain_bridge

#endif  // DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__STRUCT_HPP_
