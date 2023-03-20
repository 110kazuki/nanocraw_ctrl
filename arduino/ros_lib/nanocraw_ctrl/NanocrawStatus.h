#ifndef _ROS_nanocraw_ctrl_NanocrawStatus_h
#define _ROS_nanocraw_ctrl_NanocrawStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "nanocraw_ctrl/CrawlerUnit.h"

namespace nanocraw_ctrl
{

  class NanocrawStatus : public ros::Msg
  {
    public:
      typedef nanocraw_ctrl::CrawlerUnit _current_type;
      _current_type current;
      typedef nanocraw_ctrl::CrawlerUnit _volt_type;
      _volt_type volt;

    NanocrawStatus():
      current(),
      volt()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->current.serialize(outbuffer + offset);
      offset += this->volt.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->current.deserialize(inbuffer + offset);
      offset += this->volt.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "nanocraw_ctrl/NanocrawStatus"; };
    virtual const char * getMD5() override { return "94b09343201507a0785b0caad11cf848"; };

  };

}
#endif
