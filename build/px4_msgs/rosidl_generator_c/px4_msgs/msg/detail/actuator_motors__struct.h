// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from px4_msgs:msg/ActuatorMotors.idl
// generated code does not contain a copyright notice

#ifndef PX4_MSGS__MSG__DETAIL__ACTUATOR_MOTORS__STRUCT_H_
#define PX4_MSGS__MSG__DETAIL__ACTUATOR_MOTORS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Constant 'NUM_CONTROLS'.
enum
{
  px4_msgs__msg__ActuatorMotors__NUM_CONTROLS = 8
};

// Struct defined in msg/ActuatorMotors in the package px4_msgs.
typedef struct px4_msgs__msg__ActuatorMotors
{
  uint64_t timestamp;
  uint64_t timestamp_sample;
  uint16_t reversible_flags;
  float control[8];
} px4_msgs__msg__ActuatorMotors;

// Struct for a sequence of px4_msgs__msg__ActuatorMotors.
typedef struct px4_msgs__msg__ActuatorMotors__Sequence
{
  px4_msgs__msg__ActuatorMotors * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} px4_msgs__msg__ActuatorMotors__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // PX4_MSGS__MSG__DETAIL__ACTUATOR_MOTORS__STRUCT_H_
