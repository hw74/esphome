#include "tuya_climate_text_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace tuya {

static const char *TAG = "tuya.climate.text_sensor";

void TuyaClimateTextSensor::setup() {
  if (this->type_ == TuyaClimateTextSensorType::POWER_LEVEL)
    this->parent_->add_on_power_level_reported_callback([this]() { this->update_power_level_from_parent_(); });
  else if (this->type_ == TuyaClimateTextSensorType::OPERATION_MODE)
    this->parent_->add_on_operation_mode_reported_callback([this]() { this->update_operation_mode_from_parent_(); });
}

void TuyaClimateTextSensor::update_power_level_from_parent_() {
  std::string value;
  auto level = this->parent_->get_power_level();
  if (level.has_value())
    value = tuya_power_level_to_string(level.value());
  else
    value = "Not available";
  this->publish_state(value);
}

void TuyaClimateTextSensor::update_operation_mode_from_parent_() {
  std::string value;
  auto mode = this->parent_->get_operation_mode();
  if (mode.has_value())
    value = tuya_operation_mode_to_string(mode.value());
  else
    value = "Not available";
  this->publish_state(value);
}

void TuyaClimateTextSensor::dump_config() { LOG_TEXT_SENSOR("", "Tuya Climate Text Sensor", this); }

}  // namespace tuya
}  // namespace esphome
