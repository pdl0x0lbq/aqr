// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from armquadrover_msgs:msg/DDSM115.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "armquadrover_msgs/msg/detail/ddsm115__rosidl_typesupport_introspection_c.h"
#include "armquadrover_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "armquadrover_msgs/msg/detail/ddsm115__functions.h"
#include "armquadrover_msgs/msg/detail/ddsm115__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  armquadrover_msgs__msg__DDSM115__init(message_memory);
}

void armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_fini_function(void * message_memory)
{
  armquadrover_msgs__msg__DDSM115__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_message_member_array[8] = {
  {
    "front_left",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armquadrover_msgs__msg__DDSM115, front_left),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "front_right",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armquadrover_msgs__msg__DDSM115, front_right),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "rear_left",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armquadrover_msgs__msg__DDSM115, rear_left),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "rear_right",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT16,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armquadrover_msgs__msg__DDSM115, rear_right),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "front_left_servo",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armquadrover_msgs__msg__DDSM115, front_left_servo),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "front_right_servo",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armquadrover_msgs__msg__DDSM115, front_right_servo),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "rear_left_servo",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armquadrover_msgs__msg__DDSM115, rear_left_servo),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "rear_right_servo",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(armquadrover_msgs__msg__DDSM115, rear_right_servo),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_message_members = {
  "armquadrover_msgs__msg",  // message namespace
  "DDSM115",  // message name
  8,  // number of fields
  sizeof(armquadrover_msgs__msg__DDSM115),
  armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_message_member_array,  // message members
  armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_init_function,  // function to initialize message memory (memory has to be allocated)
  armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_message_type_support_handle = {
  0,
  &armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_armquadrover_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, armquadrover_msgs, msg, DDSM115)() {
  if (!armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_message_type_support_handle.typesupport_identifier) {
    armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &armquadrover_msgs__msg__DDSM115__rosidl_typesupport_introspection_c__DDSM115_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
