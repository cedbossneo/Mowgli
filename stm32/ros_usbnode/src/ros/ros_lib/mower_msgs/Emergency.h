#ifndef _ROS_mower_msgs_Emergency_h
#define _ROS_mower_msgs_Emergency_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"

namespace mower_msgs
{

  class Emergency : public ros::Msg
  {
    public:
      typedef ros::Time _stamp_type;
      _stamp_type stamp;
      typedef uint8_t _active_emergency_type;
      _active_emergency_type active_emergency;
      typedef uint8_t _latched_emergency_type;
      _latched_emergency_type latched_emergency;
      typedef const char* _reason_type;
      _reason_type reason;

    Emergency():
      stamp(),
      active_emergency(0),
      latched_emergency(0),
      reason("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset=serializeTime(outbuffer,this->stamp);
      offset+=serializeUint8(outbuffer+offset,this->active_emergency);
      offset+=serializeUint8(outbuffer+offset,this->latched_emergency);
      offset+=serializeString(outbuffer+offset,this->reason);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset=deserializeTime(inbuffer,&this->stamp);
      offset+=deserializeUint8(inbuffer+offset,&this->active_emergency);
      offset+=deserializeUint8(inbuffer+offset,&this->latched_emergency);
      offset+=deserializeString(inbuffer+offset,&this->reason);
      return offset;
    }

    virtual const char * getType() override { return "mower_msgs/Emergency"; };
    virtual const char * getMD5() override { return "d7eccf6a6a9c8fdeb4675252bc74b404"; };

  };

}
#endif
