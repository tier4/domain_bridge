// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from domain_bridge:msg/CompressedMsg.idl
// generated code does not contain a copyright notice
#include "domain_bridge/msg/detail/compressed_msg__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `data`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
domain_bridge__msg__CompressedMsg__init(domain_bridge__msg__CompressedMsg * msg)
{
  if (!msg) {
    return false;
  }
  // data
  if (!rosidl_runtime_c__octet__Sequence__init(&msg->data, 0)) {
    domain_bridge__msg__CompressedMsg__fini(msg);
    return false;
  }
  return true;
}

void
domain_bridge__msg__CompressedMsg__fini(domain_bridge__msg__CompressedMsg * msg)
{
  if (!msg) {
    return;
  }
  // data
  rosidl_runtime_c__octet__Sequence__fini(&msg->data);
}

bool
domain_bridge__msg__CompressedMsg__are_equal(const domain_bridge__msg__CompressedMsg * lhs, const domain_bridge__msg__CompressedMsg * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // data
  if (!rosidl_runtime_c__octet__Sequence__are_equal(
      &(lhs->data), &(rhs->data)))
  {
    return false;
  }
  return true;
}

bool
domain_bridge__msg__CompressedMsg__copy(
  const domain_bridge__msg__CompressedMsg * input,
  domain_bridge__msg__CompressedMsg * output)
{
  if (!input || !output) {
    return false;
  }
  // data
  if (!rosidl_runtime_c__octet__Sequence__copy(
      &(input->data), &(output->data)))
  {
    return false;
  }
  return true;
}

domain_bridge__msg__CompressedMsg *
domain_bridge__msg__CompressedMsg__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  domain_bridge__msg__CompressedMsg * msg = (domain_bridge__msg__CompressedMsg *)allocator.allocate(sizeof(domain_bridge__msg__CompressedMsg), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(domain_bridge__msg__CompressedMsg));
  bool success = domain_bridge__msg__CompressedMsg__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
domain_bridge__msg__CompressedMsg__destroy(domain_bridge__msg__CompressedMsg * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    domain_bridge__msg__CompressedMsg__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
domain_bridge__msg__CompressedMsg__Sequence__init(domain_bridge__msg__CompressedMsg__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  domain_bridge__msg__CompressedMsg * data = NULL;

  if (size) {
    data = (domain_bridge__msg__CompressedMsg *)allocator.zero_allocate(size, sizeof(domain_bridge__msg__CompressedMsg), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = domain_bridge__msg__CompressedMsg__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        domain_bridge__msg__CompressedMsg__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
domain_bridge__msg__CompressedMsg__Sequence__fini(domain_bridge__msg__CompressedMsg__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      domain_bridge__msg__CompressedMsg__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

domain_bridge__msg__CompressedMsg__Sequence *
domain_bridge__msg__CompressedMsg__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  domain_bridge__msg__CompressedMsg__Sequence * array = (domain_bridge__msg__CompressedMsg__Sequence *)allocator.allocate(sizeof(domain_bridge__msg__CompressedMsg__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = domain_bridge__msg__CompressedMsg__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
domain_bridge__msg__CompressedMsg__Sequence__destroy(domain_bridge__msg__CompressedMsg__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    domain_bridge__msg__CompressedMsg__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
domain_bridge__msg__CompressedMsg__Sequence__are_equal(const domain_bridge__msg__CompressedMsg__Sequence * lhs, const domain_bridge__msg__CompressedMsg__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!domain_bridge__msg__CompressedMsg__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
domain_bridge__msg__CompressedMsg__Sequence__copy(
  const domain_bridge__msg__CompressedMsg__Sequence * input,
  domain_bridge__msg__CompressedMsg__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(domain_bridge__msg__CompressedMsg);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    domain_bridge__msg__CompressedMsg * data =
      (domain_bridge__msg__CompressedMsg *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!domain_bridge__msg__CompressedMsg__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          domain_bridge__msg__CompressedMsg__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!domain_bridge__msg__CompressedMsg__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
