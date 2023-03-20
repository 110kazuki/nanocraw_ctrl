#ifndef _ROS_cugo_control_MotorCmd_h
#define _ROS_cugo_control_MotorCmd_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace cugo_control
{

  class MotorCmd : public ros::Msg
  {
    public:
      typedef int16_t _left_motor_type;
      _left_motor_type left_motor;
      typedef int16_t _right_motor_type;
      _right_motor_type right_motor;

    MotorCmd():
      left_motor(0),
      right_motor(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_left_motor;
      u_left_motor.real = this->left_motor;
      *(outbuffer + offset + 0) = (u_left_motor.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_left_motor.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->left_motor);
      union {
        int16_t real;
        uint16_t base;
      } u_right_motor;
      u_right_motor.real = this->right_motor;
      *(outbuffer + offset + 0) = (u_right_motor.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_right_motor.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->right_motor);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_left_motor;
      u_left_motor.base = 0;
      u_left_motor.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_left_motor.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->left_motor = u_left_motor.real;
      offset += sizeof(this->left_motor);
      union {
        int16_t real;
        uint16_t base;
      } u_right_motor;
      u_right_motor.base = 0;
      u_right_motor.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_right_motor.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->right_motor = u_right_motor.real;
      offset += sizeof(this->right_motor);
     return offset;
    }

    virtual const char * getType() override { return "cugo_control/MotorCmd"; };
    virtual const char * getMD5() override { return "c9620e3cdcbc4957c5312462ef3f2bf5"; };

  };

}
#endif
