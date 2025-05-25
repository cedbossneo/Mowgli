#ifndef _ROS_mower_msgs_Status_h
#define _ROS_mower_msgs_Status_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"

namespace mower_msgs
{

  class Status : public ros::Msg
  {
    public:
      typedef ros::Time _stamp_type;
      _stamp_type stamp;
      typedef uint8_t _mower_status_type;
      _mower_status_type mower_status;
      typedef uint8_t _raspberry_pi_power_type;
      _raspberry_pi_power_type raspberry_pi_power;
      typedef uint8_t _gps_power_type;
      _gps_power_type gps_power;
      typedef uint8_t _esc_power_type;
      _esc_power_type esc_power;
      typedef uint8_t _rain_detected_type;
      _rain_detected_type rain_detected;
      typedef uint8_t _sound_module_available_type;
      _sound_module_available_type sound_module_available;
      typedef uint8_t _sound_module_busy_type;
      _sound_module_busy_type sound_module_busy;
      typedef uint8_t _ui_board_available_type;
      _ui_board_available_type ui_board_available;
      typedef uint8_t _mow_enabled_type;
      _mow_enabled_type mow_enabled;
      typedef uint8_t _mower_esc_status_type;
      _mower_esc_status_type mower_esc_status;
       typedef float _mower_esc_temperature_type;
      _mower_esc_temperature_type mower_esc_temperature;
      typedef float _mower_esc_current_type;
      _mower_esc_current_type mower_esc_current;
      typedef float _mower_motor_temperature_type;
      _mower_motor_temperature_type mower_motor_temperature;
      typedef float _mower_motor_rpm_type;
      _mower_motor_rpm_type mower_motor_rpm;

      enum {
         MOWER_STATUS_INITIALIZING = 0,
         MOWER_STATUS_OK = 255
      };

    Status():
      stamp(),
      mower_status(0),
      raspberry_pi_power(0),
      gps_power(0),
      esc_power(0),
      rain_detected(0),
      sound_module_available(0),
      sound_module_busy(0),
      ui_board_available(0),
      mow_enabled(0),
      mower_esc_status(0),
      mower_esc_temperature(0),
      mower_esc_current(0),
      mower_motor_temperature(0),
      mower_motor_rpm(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset=serializeTime(outbuffer,this->stamp);
      offset+=serializeUint8(outbuffer+offset,this->mower_status);
      offset+=serializeUint8(outbuffer+offset,this->raspberry_pi_power);
      offset+=serializeUint8(outbuffer+offset,this->gps_power);
      offset+=serializeUint8(outbuffer+offset,this->esc_power);
      offset+=serializeUint8(outbuffer+offset,this->rain_detected);
      offset+=serializeUint8(outbuffer+offset,this->sound_module_available);
      offset+=serializeUint8(outbuffer+offset,this->sound_module_busy);
      offset+=serializeUint8(outbuffer+offset,this->ui_board_available);
      offset+=serializeUint8(outbuffer+offset,this->mow_enabled);
      offset+=serializeUint8(outbuffer+offset,this->mower_esc_status);
      offset+=serializeFloat(outbuffer+offset,this->mower_esc_temperature);
      offset+=serializeFloat(outbuffer+offset,this->mower_esc_current);
      offset+=serializeFloat(outbuffer+offset,this->mower_motor_temperature);
      offset+=serializeFloat(outbuffer+offset,this->mower_motor_rpm);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset=deserializeTime(inbuffer,&this->stamp);
      offset+=deserializeUint8(inbuffer+offset,&this->mower_status);
      offset+=deserializeUint8(inbuffer+offset,&this->raspberry_pi_power);
      offset+=deserializeUint8(inbuffer+offset,&this->gps_power);
      offset+=deserializeUint8(inbuffer+offset,&this->esc_power);
      offset+=deserializeUint8(inbuffer+offset,&this->rain_detected);
      offset+=deserializeUint8(inbuffer+offset,&this->sound_module_available);
      offset+=deserializeUint8(inbuffer+offset,&this->sound_module_busy);
      offset+=deserializeUint8(inbuffer+offset,&this->ui_board_available);
      offset+=deserializeUint8(inbuffer+offset,&this->mow_enabled);
      offset+=deserializeUint8(inbuffer+offset,&this->mower_esc_status);
      offset+=deserializeFloat(inbuffer+offset,&this->mower_esc_temperature);
      offset+=deserializeFloat(inbuffer+offset,&this->mower_esc_current);
      offset+=deserializeFloat(inbuffer+offset,&this->mower_motor_temperature);
      offset+=deserializeFloat(inbuffer+offset,&this->mower_motor_rpm);
      return offset;
    }

    virtual const char * getType() override { return "mower_msgs/Status"; };
    virtual const char * getMD5() override { return "f0436eea58ffe66343cd2a3715ac406d"; };

  };

}
#endif
