// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from domain_bridge:msg/CompressedMsg.idl
// generated code does not contain a copyright notice

#ifndef DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__FUNCTIONS_H_
#define DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "domain_bridge/msg/rosidl_generator_c__visibility_control.h"

#include "domain_bridge/msg/detail/compressed_msg__struct.h"

/// Initialize msg/CompressedMsg message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * domain_bridge__msg__CompressedMsg
 * )) before or use
 * domain_bridge__msg__CompressedMsg__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
bool
domain_bridge__msg__CompressedMsg__init(domain_bridge__msg__CompressedMsg * msg);

/// Finalize msg/CompressedMsg message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
void
domain_bridge__msg__CompressedMsg__fini(domain_bridge__msg__CompressedMsg * msg);

/// Create msg/CompressedMsg message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * domain_bridge__msg__CompressedMsg__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
domain_bridge__msg__CompressedMsg *
domain_bridge__msg__CompressedMsg__create();

/// Destroy msg/CompressedMsg message.
/**
 * It calls
 * domain_bridge__msg__CompressedMsg__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
void
domain_bridge__msg__CompressedMsg__destroy(domain_bridge__msg__CompressedMsg * msg);

/// Check for msg/CompressedMsg message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
bool
domain_bridge__msg__CompressedMsg__are_equal(const domain_bridge__msg__CompressedMsg * lhs, const domain_bridge__msg__CompressedMsg * rhs);

/// Copy a msg/CompressedMsg message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
bool
domain_bridge__msg__CompressedMsg__copy(
  const domain_bridge__msg__CompressedMsg * input,
  domain_bridge__msg__CompressedMsg * output);

/// Initialize array of msg/CompressedMsg messages.
/**
 * It allocates the memory for the number of elements and calls
 * domain_bridge__msg__CompressedMsg__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
bool
domain_bridge__msg__CompressedMsg__Sequence__init(domain_bridge__msg__CompressedMsg__Sequence * array, size_t size);

/// Finalize array of msg/CompressedMsg messages.
/**
 * It calls
 * domain_bridge__msg__CompressedMsg__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
void
domain_bridge__msg__CompressedMsg__Sequence__fini(domain_bridge__msg__CompressedMsg__Sequence * array);

/// Create array of msg/CompressedMsg messages.
/**
 * It allocates the memory for the array and calls
 * domain_bridge__msg__CompressedMsg__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
domain_bridge__msg__CompressedMsg__Sequence *
domain_bridge__msg__CompressedMsg__Sequence__create(size_t size);

/// Destroy array of msg/CompressedMsg messages.
/**
 * It calls
 * domain_bridge__msg__CompressedMsg__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
void
domain_bridge__msg__CompressedMsg__Sequence__destroy(domain_bridge__msg__CompressedMsg__Sequence * array);

/// Check for msg/CompressedMsg message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
bool
domain_bridge__msg__CompressedMsg__Sequence__are_equal(const domain_bridge__msg__CompressedMsg__Sequence * lhs, const domain_bridge__msg__CompressedMsg__Sequence * rhs);

/// Copy an array of msg/CompressedMsg messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_domain_bridge
bool
domain_bridge__msg__CompressedMsg__Sequence__copy(
  const domain_bridge__msg__CompressedMsg__Sequence * input,
  domain_bridge__msg__CompressedMsg__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // DOMAIN_BRIDGE__MSG__DETAIL__COMPRESSED_MSG__FUNCTIONS_H_
