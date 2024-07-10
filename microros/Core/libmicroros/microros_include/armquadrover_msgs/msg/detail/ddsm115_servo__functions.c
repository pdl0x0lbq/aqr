// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from armquadrover_msgs:msg/DDSM115Servo.idl
// generated code does not contain a copyright notice
#include "armquadrover_msgs/msg/detail/ddsm115_servo__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
armquadrover_msgs__msg__DDSM115Servo__init(armquadrover_msgs__msg__DDSM115Servo * msg)
{
  if (!msg) {
    return false;
  }
  // front_left_servo
  // front_right_servo
  // rear_left_servo
  // rear_right_servo
  return true;
}

void
armquadrover_msgs__msg__DDSM115Servo__fini(armquadrover_msgs__msg__DDSM115Servo * msg)
{
  if (!msg) {
    return;
  }
  // front_left_servo
  // front_right_servo
  // rear_left_servo
  // rear_right_servo
}

bool
armquadrover_msgs__msg__DDSM115Servo__are_equal(const armquadrover_msgs__msg__DDSM115Servo * lhs, const armquadrover_msgs__msg__DDSM115Servo * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // front_left_servo
  if (lhs->front_left_servo != rhs->front_left_servo) {
    return false;
  }
  // front_right_servo
  if (lhs->front_right_servo != rhs->front_right_servo) {
    return false;
  }
  // rear_left_servo
  if (lhs->rear_left_servo != rhs->rear_left_servo) {
    return false;
  }
  // rear_right_servo
  if (lhs->rear_right_servo != rhs->rear_right_servo) {
    return false;
  }
  return true;
}

bool
armquadrover_msgs__msg__DDSM115Servo__copy(
  const armquadrover_msgs__msg__DDSM115Servo * input,
  armquadrover_msgs__msg__DDSM115Servo * output)
{
  if (!input || !output) {
    return false;
  }
  // front_left_servo
  output->front_left_servo = input->front_left_servo;
  // front_right_servo
  output->front_right_servo = input->front_right_servo;
  // rear_left_servo
  output->rear_left_servo = input->rear_left_servo;
  // rear_right_servo
  output->rear_right_servo = input->rear_right_servo;
  return true;
}

armquadrover_msgs__msg__DDSM115Servo *
armquadrover_msgs__msg__DDSM115Servo__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  armquadrover_msgs__msg__DDSM115Servo * msg = (armquadrover_msgs__msg__DDSM115Servo *)allocator.allocate(sizeof(armquadrover_msgs__msg__DDSM115Servo), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(armquadrover_msgs__msg__DDSM115Servo));
  bool success = armquadrover_msgs__msg__DDSM115Servo__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
armquadrover_msgs__msg__DDSM115Servo__destroy(armquadrover_msgs__msg__DDSM115Servo * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    armquadrover_msgs__msg__DDSM115Servo__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
armquadrover_msgs__msg__DDSM115Servo__Sequence__init(armquadrover_msgs__msg__DDSM115Servo__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  armquadrover_msgs__msg__DDSM115Servo * data = NULL;

  if (size) {
    data = (armquadrover_msgs__msg__DDSM115Servo *)allocator.zero_allocate(size, sizeof(armquadrover_msgs__msg__DDSM115Servo), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = armquadrover_msgs__msg__DDSM115Servo__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        armquadrover_msgs__msg__DDSM115Servo__fini(&data[i - 1]);
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
armquadrover_msgs__msg__DDSM115Servo__Sequence__fini(armquadrover_msgs__msg__DDSM115Servo__Sequence * array)
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
      armquadrover_msgs__msg__DDSM115Servo__fini(&array->data[i]);
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

armquadrover_msgs__msg__DDSM115Servo__Sequence *
armquadrover_msgs__msg__DDSM115Servo__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  armquadrover_msgs__msg__DDSM115Servo__Sequence * array = (armquadrover_msgs__msg__DDSM115Servo__Sequence *)allocator.allocate(sizeof(armquadrover_msgs__msg__DDSM115Servo__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = armquadrover_msgs__msg__DDSM115Servo__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
armquadrover_msgs__msg__DDSM115Servo__Sequence__destroy(armquadrover_msgs__msg__DDSM115Servo__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    armquadrover_msgs__msg__DDSM115Servo__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
armquadrover_msgs__msg__DDSM115Servo__Sequence__are_equal(const armquadrover_msgs__msg__DDSM115Servo__Sequence * lhs, const armquadrover_msgs__msg__DDSM115Servo__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!armquadrover_msgs__msg__DDSM115Servo__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
armquadrover_msgs__msg__DDSM115Servo__Sequence__copy(
  const armquadrover_msgs__msg__DDSM115Servo__Sequence * input,
  armquadrover_msgs__msg__DDSM115Servo__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(armquadrover_msgs__msg__DDSM115Servo);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    armquadrover_msgs__msg__DDSM115Servo * data =
      (armquadrover_msgs__msg__DDSM115Servo *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!armquadrover_msgs__msg__DDSM115Servo__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          armquadrover_msgs__msg__DDSM115Servo__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!armquadrover_msgs__msg__DDSM115Servo__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
