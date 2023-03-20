#ifndef _ROS_blower_control_BlowerCmd_h
#define _ROS_blower_control_BlowerCmd_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace blower_control
{

  class BlowerCmd : public ros::Msg
  {
    public:
      typedef int16_t _motor_ctrl_type;
      _motor_ctrl_type motor_ctrl;

    BlowerCmd():
      motor_ctrl(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_motor_ctrl;
      u_motor_ctrl.real = this->motor_ctrl;
      *(outbuffer + offset + 0) = (u_motor_ctrl.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_motor_ctrl.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->motor_ctrl);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_motor_ctrl;
      u_motor_ctrl.base = 0;
      u_motor_ctrl.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_motor_ctrl.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->motor_ctrl = u_motor_ctrl.real;
      offset += sizeof(this->motor_ctrl);
     return offset;
    }

    virtual const char * getType() override { return "blower_control/BlowerCmd"; };
    virtual const char * getMD5() override { return "3d088bb83def02151b8f3249aceced88"; };

  };

}
#endif
