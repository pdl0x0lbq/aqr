// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from armquadrover_msgs:msg/DDSM115.idl
// generated code does not contain a copyright notice

#ifndef ARMQUADROVER_MSGS__MSG__DETAIL__DDSM115__FUNCTIONS_H_
#define ARMQUADROVER_MSGS__MSG__DETAIL__DDSM115__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "armquadrover_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "armquadrover_msgs/msg/detail/ddsm115__struct.h"

/// Initialize msg/DDSM115 message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * armquadrover_msgs__msg__DDSM115
 * )) before or use
 * armquadrover_msgs__msg__DDSM115__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
bool
armquadrover_msgs__msg__DDSM115__init(armquadrover_msgs__msg__DDSM115 * msg);

/// Finalize msg/DDSM115 message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
void
armquadrover_msgs__msg__DDSM115__fini(armquadrover_msgs__msg__DDSM115 * msg);

/// Create msg/DDSM115 message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * armquadrover_msgs__msg__DDSM115__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
armquadrover_msgs__msg__DDSM115 *
armquadrover_msgs__msg__DDSM115__create();

/// Destroy msg/DDSM115 message.
/**
 * It calls
 * armquadrover_msgs__msg__DDSM115__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
void
armquadrover_msgs__msg__DDSM115__destroy(armquadrover_msgs__msg__DDSM115 * msg);

/// Check for msg/DDSM115 message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
bool
armquadrover_msgs__msg__DDSM115__are_equal(const armquadrover_msgs__msg__DDSM115 * lhs, const armquadrover_msgs__msg__DDSM115 * rhs);

/// Copy a msg/DDSM115 message.
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
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
bool
armquadrover_msgs__msg__DDSM115__copy(
  const armquadrover_msgs__msg__DDSM115 * input,
  armquadrover_msgs__msg__DDSM115 * output);

/// Initialize array of msg/DDSM115 messages.
/**
 * It allocates the memory for the number of elements and calls
 * armquadrover_msgs__msg__DDSM115__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
bool
armquadrover_msgs__msg__DDSM115__Sequence__init(armquadrover_msgs__msg__DDSM115__Sequence * array, size_t size);

/// Finalize array of msg/DDSM115 messages.
/**
 * It calls
 * armquadrover_msgs__msg__DDSM115__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
void
armquadrover_msgs__msg__DDSM115__Sequence__fini(armquadrover_msgs__msg__DDSM115__Sequence * array);

/// Create array of msg/DDSM115 messages.
/**
 * It allocates the memory for the array and calls
 * armquadrover_msgs__msg__DDSM115__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
armquadrover_msgs__msg__DDSM115__Sequence *
armquadrover_msgs__msg__DDSM115__Sequence__create(size_t size);

/// Destroy array of msg/DDSM115 messages.
/**
 * It calls
 * armquadrover_msgs__msg__DDSM115__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
void
armquadrover_msgs__msg__DDSM115__Sequence__destroy(armquadrover_msgs__msg__DDSM115__Sequence * array);

/// Check for msg/DDSM115 message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
bool
armquadrover_msgs__msg__DDSM115__Sequence__are_equal(const armquadrover_msgs__msg__DDSM115__Sequence * lhs, const armquadrover_msgs__msg__DDSM115__Sequence * rhs);

/// Copy an array of msg/DDSM115 messages.
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
ROSIDL_GENERATOR_C_PUBLIC_armquadrover_msgs
bool
armquadrover_msgs__msg__DDSM115__Sequence__copy(
  const armquadrover_msgs__msg__DDSM115__Sequence * input,
  armquadrover_msgs__msg__DDSM115__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // ARMQUADROVER_MSGS__MSG__DETAIL__DDSM115__FUNCTIONS_H_
