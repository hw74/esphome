#pragma once

#include "esphome/core/component.h"
#include "esphome/components/tuya/tuya.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace tuya {

class TuyaSensor : public sensor::Sensor, public Component {
 public:
  void set_datapoint(uint8_t datapoint_id) { datapoint_id_ = datapoint_id; }
  void set_tuya_parent(Tuya *parent) { this->parent_ = parent; }

  void setup() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void dump_config() override;

 protected:

  Tuya *parent_;
  uint8_t datapoint_id_{};
};

}  // namespace tuya
}  // namespace esphome
