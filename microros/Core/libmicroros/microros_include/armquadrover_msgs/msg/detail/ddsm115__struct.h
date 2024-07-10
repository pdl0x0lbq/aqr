// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from armquadrover_msgs:msg/DDSM115.idl
// generated code does not contain a copyright notice

#ifndef ARMQUADROVER_MSGS__MSG__DETAIL__DDSM115__STRUCT_H_
#define ARMQUADROVER_MSGS__MSG__DETAIL__DDSM115__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/DDSM115 in the package armquadrover_msgs.
typedef struct armquadrover_msgs__msg__DDSM115
{
  int16_t front_left;
  int16_t front_right;
  int16_t rear_left;
  int16_t rear_right;
  float front_left_servo;
  float front_right_servo;
  float rear_left_servo;
  float rear_right_servo;
} armquadrover_msgs__msg__DDSM115;

// Struct for a sequence of armquadrover_msgs__msg__DDSM115.
typedef struct armquadrover_msgs__msg__DDSM115__Sequence
{
  armquadrover_msgs__msg__DDSM115 * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} armquadrover_msgs__msg__DDSM115__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ARMQUADROVER_MSGS__MSG__DETAIL__DDSM115__STRUCT_H_