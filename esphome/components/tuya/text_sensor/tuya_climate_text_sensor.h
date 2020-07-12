#pragma once

#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "../climate/tuya_climate.h"

namespace esphome {
namespace tuya {

enum TuyaClimateTextSensorType {
  POWER_LEVEL,
  OPERATION_MODE,
};

class TuyaClimateTextSensor : public text_sensor::TextSensor, public Component {
 public:
  void setup() override;
  void set_parent(TuyaClimate *parent) { parent_ = parent; }
  void set_type(TuyaClimateTextSensorType type) { type_ = type; }

  void dump_config() override;

 protected:
  void update_power_level_from_parent_();
  void update_operation_mode_from_parent_();
  TuyaClimate *parent_;
  TuyaClimateTextSensorType type_;
};

}  // namespace tuya
}  // namespace esphome
