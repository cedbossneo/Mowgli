#ifndef _ROS_mower_msgs_Power_h
#define _ROS_mower_msgs_Power_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"

namespace mower_msgs
{

  class Power : public ros::Msg
  {
    public:
      typedef ros::Time _stamp_type;
      _stamp_type stamp;
      typedef float _v_charge_type;
      _v_charge_type v_charge;
      typedef float _v_battery_type;
      _v_battery_type v_battery;
      typedef float _charge_current_type;
      _charge_current_type charge_current;
      typedef uint8_t _charger_enabled_type;
      _charger_enabled_type charger_enabled;
      typedef const char* _charger_status_type;
      _charger_status_type charger_status;

    Power():
      stamp(),
      v_charge(0),
      v_battery(0),
      charge_current(0),
      charger_enabled(0),
      charger_status("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset=serializeTime(outbuffer,this->stamp);
      offset+=serializeFloat(outbuffer+offset,this->v_charge);
      offset+=serializeFloat(outbuffer+offset,this->v_battery);
      offset+=serializeFloat(outbuffer+offset,this->charge_current);
      offset+=serializeUint8(outbuffer+offset,this->charger_enabled);
      offset+=serializeString(outbuffer+offset,this->charger_status);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset=deserializeTime(inbuffer,&this->stamp);
      offset+=deserializeFloat(inbuffer+offset,&this->v_charge);
      offset+=deserializeFloat(inbuffer+offset,&this->v_battery);
      offset+=deserializeFloat(inbuffer+offset,&this->charge_current);
      offset+=deserializeUint8(inbuffer+offset,&this->charger_enabled);
      offset+=deserializeString(inbuffer+offset,&this->charger_status);
      return offset;
    }

    virtual const char * getType() override { return "mower_msgs/Power"; };
    virtual const char * getMD5() override { return "b4d588c7a5fc639905fc448fc85b1ca9"; };

  };

}
#endif
