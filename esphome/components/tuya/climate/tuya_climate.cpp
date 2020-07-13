#include "esphome/core/log.h"
#include "tuya_climate.h"

namespace esphome {
namespace tuya {

static const char *TAG = "tuya.climate";

void TuyaClimate::setup() {
  if (this->switch_id_.has_value()) {
    this->parent_->register_listener(*this->switch_id_, [this](TuyaDatapoint datapoint) {
      if (datapoint.value_bool) {
        this->mode = climate::CLIMATE_MODE_HEAT;
      } else {
        this->mode = climate::CLIMATE_MODE_OFF;
      }
      this->compute_state_();
      this->publish_state();
      ESP_LOGD(TAG, "MCU reported switch is: %s", ONOFF(datapoint.value_bool));
    });
  }
  if (this->target_temperature_id_.has_value()) {
    this->parent_->register_listener(*this->target_temperature_id_, [this](TuyaDatapoint datapoint) {
      this->target_temperature = datapoint.value_int;
      this->compute_state_();
      this->publish_state();
      ESP_LOGD(TAG, "MCU reported target temperature is: %d", datapoint.value_int);
    });
  }
  if (this->current_temperature_id_.has_value()) {
    this->parent_->register_listener(*this->current_temperature_id_, [this](TuyaDatapoint datapoint) {
      this->current_temperature = datapoint.value_int;
      this->compute_state_();
      this->publish_state();
      ESP_LOGD(TAG, "MCU reported current temperature is: %d", datapoint.value_int);
    });
  }
  // if (this->eco_mode_id_.has_value()) {
  //   this->parent_->register_listener(*this->eco_mode_id_, [this](TuyaDatapoint datapoint) {
  //     this->eco_mode = datapoint.value_bool;
  //     this->compute_state_();
  //     this->publish_state();
  //     ESP_LOGD(TAG, "MCU reported eco mode of: %s", ONOFF(datapoint.value_bool));
  //   });
  // }
  if (this->power_level_id_.has_value()) {
    this->parent_->register_listener(*this->power_level_id_, [this](TuyaDatapoint datapoint) {
      if (power_level_enum_low_.has_value() && datapoint.value_enum == power_level_enum_low_.value())
        this->set_mcu_reported_power_level(TuyaPowerLevel::POWER_LEVEL_LOW);
      else if (power_level_enum_medium_.has_value() && datapoint.value_enum == power_level_enum_medium_.value())
        this->set_mcu_reported_power_level(TuyaPowerLevel::POWER_LEVEL_MEDIUM);
      else if (power_level_enum_high_.has_value() && datapoint.value_enum == power_level_enum_high_.value())
        this->set_mcu_reported_power_level(TuyaPowerLevel::POWER_LEVEL_HIGH);
      else if (power_level_enum_off_.has_value() && datapoint.value_enum == power_level_enum_off_.value())
        this->set_mcu_reported_power_level(TuyaPowerLevel::POWER_LEVEL_OFF);
      else {
        this->set_mcu_reported_power_level(TuyaPowerLevel::POWER_LEVEL_UNKNOWN);
        ESP_LOGD(TAG, "MCU reported unconfigured or unknown power level enum value: %u", datapoint.value_enum);
      }

      ESP_LOGV(TAG, "MCU reported power level: %s (enum %u)",
               tuya_power_level_to_string(this->power_level_.value()), datapoint.value_enum);
    });
  }
  if (this->operation_mode_id_.has_value()) {
    this->parent_->register_listener(*this->operation_mode_id_, [this](TuyaDatapoint datapoint) {
      if (datapoint.value_enum == 0)
        this->set_mcu_reported_operation_mode(TuyaOperationMode::OPERATION_MODE_MANUAL);
      else if (datapoint.value_enum == 1)
        this->set_mcu_reported_operation_mode(TuyaOperationMode::OPERATION_MODE_PROGRAM);
      else {
        this->set_mcu_reported_operation_mode(TuyaOperationMode::OPERATION_MODE_UNKNOWN);
        ESP_LOGD(TAG, "MCU reported unconfigured or unknown  operation mode enum value: %u", datapoint.value_enum);
      }
      ESP_LOGV(TAG, "MCU reported operation mode: %s (enum %u)",
               tuya_operation_mode_to_string(this->operation_mode_.value()), datapoint.value_enum);
    });
  }
}

void TuyaClimate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    this->mode = *call.get_mode();

    TuyaDatapoint datapoint{};
    datapoint.id = *this->switch_id_;
    datapoint.type = TuyaDatapointType::BOOLEAN;
    datapoint.value_bool = this->mode != climate::CLIMATE_MODE_OFF;
    this->parent_->set_datapoint_value(datapoint);
    ESP_LOGD(TAG, "Setting switch: %s", ONOFF(datapoint.value_bool));
  }
  if (call.get_target_temperature_low().has_value())
    this->target_temperature_low = *call.get_target_temperature_low();
  if (call.get_target_temperature_high().has_value())
    this->target_temperature_high = *call.get_target_temperature_high();
  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();

    TuyaDatapoint datapoint{};
    datapoint.id = *this->target_temperature_id_;
    datapoint.type = TuyaDatapointType::INTEGER;
    datapoint.value_int = (int) this->target_temperature;
    this->parent_->set_datapoint_value(datapoint);
    ESP_LOGD(TAG, "Setting target temperature: %d", datapoint.value_int);
  }
  // if (call.get_eco_mode().has_value()) {
  //   this->eco_mode = *call.get_eco_mode();

  //   TuyaDatapoint datapoint{};
  //   datapoint.id = *this->eco_mode_id_;
  //   datapoint.type = TuyaDatapointType::BOOLEAN;
  //   datapoint.value_bool = this->eco_mode;
  //   this->parent_->set_datapoint_value(datapoint);
  //   ESP_LOGD(TAG, "Setting eco mode: %s", ONOFF(datapoint.value_bool));
  // }

  this->compute_state_();
  this->publish_state();
}

climate::ClimateTraits TuyaClimate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(this->current_temperature_id_.has_value());
  traits.set_supports_heat_mode(true);
  // traits.set_supports_eco_mode(this->eco_mode_id_.has_value());
  traits.set_supports_action(true);
  return traits;
}

void TuyaClimate::dump_config() {
  LOG_CLIMATE("", "Tuya Climate", this);
  if (this->switch_id_.has_value())
    ESP_LOGCONFIG(TAG, "  Switch has datapoint ID %u", *this->switch_id_);
  if (this->target_temperature_id_.has_value())
    ESP_LOGCONFIG(TAG, "  Target Temperature has datapoint ID %u", *this->target_temperature_id_);
  if (this->current_temperature_id_.has_value())
    ESP_LOGCONFIG(TAG, "  Current Temperature has datapoint ID %u", *this->current_temperature_id_);
  // if (this->eco_mode_id_.has_value())
  //   ESP_LOGCONFIG(TAG, "  Eco Mode has datapoint ID %u", *this->mode_id_);
}

void TuyaClimate::compute_state_() {
  if (supports_POWER_LEVEL_()) {
    return; // power level monitor will set the correct state
  }

  if (isnan(this->current_temperature) || isnan(this->target_temperature)) {
    // if any control parameters are nan, go to OFF action (not IDLE!)
    this->switch_to_action_(climate::CLIMATE_ACTION_OFF);
    return;
  }

  if (this->mode == climate::CLIMATE_MODE_OFF) {
    this->switch_to_action_(climate::CLIMATE_ACTION_OFF);
    return;
  }

  const bool too_cold = this->current_temperature < this->target_temperature - 1;
  const bool too_hot = this->current_temperature > this->target_temperature + 1;
  const bool on_target = this->current_temperature == this->target_temperature;

  climate::ClimateAction target_action;
  if (too_cold) {
    // too cold -> show as heating if possible, else idle
    if (this->traits().supports_mode(climate::CLIMATE_MODE_HEAT)) {
      target_action = climate::CLIMATE_ACTION_HEATING;
    } else {
      target_action = climate::CLIMATE_ACTION_IDLE;
    }
  } else if (too_hot) {
    // too hot -> show as cooling if possible, else idle
    if (this->traits().supports_mode(climate::CLIMATE_MODE_COOL)) {
      target_action = climate::CLIMATE_ACTION_COOLING;
    } else {
      target_action = climate::CLIMATE_ACTION_IDLE;
    }
  } else if (on_target) {
    target_action = climate::CLIMATE_ACTION_IDLE;
  } else {
    target_action = this->action;
  }
  this->switch_to_action_(target_action);
}

void TuyaClimate::switch_to_action_(climate::ClimateAction action) {
  // For now this just sets the current action but could include triggers later
  this->action = action;
}

bool TuyaClimate::supports_POWER_LEVEL_() {
  return this->power_level_id_.has_value();
}

void TuyaClimate::set_mcu_reported_power_level(TuyaPowerLevel level) {
   
   this->power_level_ = level;
   switch (level) {
      case TuyaPowerLevel::POWER_LEVEL_OFF:
        switch_to_action_(climate::CLIMATE_ACTION_IDLE);
        break;
      case TuyaPowerLevel::POWER_LEVEL_LOW:
      case TuyaPowerLevel::POWER_LEVEL_MEDIUM:
      case TuyaPowerLevel::POWER_LEVEL_HIGH:
        switch_to_action_(climate::CLIMATE_ACTION_HEATING);
        break;
      default: // do nothing
        break;
    }
    this->power_level_reported_callback_.call();
    this->publish_state();
}

void TuyaClimate::control_power_level(TuyaPowerLevel level) {
  ESP_LOGV(TAG, "Received power level control request: %s", tuya_power_level_to_string(level));
  if (supports_POWER_LEVEL_()) {
    uint8_t value;
    if (level == TuyaPowerLevel::POWER_LEVEL_LOW && power_level_enum_low_.has_value())
      value = power_level_enum_low_.value();
    else if (level == TuyaPowerLevel::POWER_LEVEL_MEDIUM && power_level_enum_medium_.has_value())
      value = power_level_enum_medium_.value();
    else if (level == TuyaPowerLevel::POWER_LEVEL_HIGH && power_level_enum_high_.has_value())
      value = power_level_enum_high_.value();
    else if (level == TuyaPowerLevel::POWER_LEVEL_OFF && power_level_enum_off_.has_value())
      value = power_level_enum_off_.value();
    else {
      ESP_LOGD(TAG, "Setting power level: %s not supported or not configured", tuya_power_level_to_string(level));
      return;
    }

    ESP_LOGD(TAG, "Sending power level: %s (enum value=%u) to MCU", tuya_power_level_to_string(level), value);
    TuyaDatapoint datapoint{};
    datapoint.id = *this->power_level_id_;
    datapoint.type = TuyaDatapointType::ENUM;
    datapoint.value_enum = value;
    this->parent_->set_datapoint_value(datapoint);
  }
  ESP_LOGD(TAG, "Control power level not supported");
}

bool TuyaClimate::supports_operation_mode_() { return this->operation_mode_id_.has_value(); }

void TuyaClimate::set_mcu_reported_operation_mode(TuyaOperationMode mode) {
  this->operation_mode_ = mode;
  // may also want to reset traits to avoid unexpected/undesired behavior (like setting power level not allowed/does nothing when in program mode)
  this->operation_mode_reported_callback_.call();
  this->publish_state();
}

void TuyaClimate::control_operation_mode(TuyaOperationMode mode) {
  ESP_LOGV(TAG, "Received operation mode control request: %s", tuya_operation_mode_to_string(mode));
  if (supports_operation_mode_()) {
    uint8_t value;
    if (mode == TuyaOperationMode::OPERATION_MODE_MANUAL && operation_mode_enum_manual_.has_value())
      value = operation_mode_enum_manual_.value();
    else if (mode == TuyaOperationMode::OPERATION_MODE_PROGRAM && operation_mode_enum_program_.has_value())
      value = operation_mode_enum_program_.value();
    else {
      ESP_LOGD(TAG, "Setting operation mode %s not supported or not configured", tuya_operation_mode_to_string(mode));
      return;
    }

    ESP_LOGD(TAG, "Sending operation mode: %s (enum value=%u) to MCU", tuya_operation_mode_to_string(mode), value);
    TuyaDatapoint datapoint{};
    datapoint.id = *this->operation_mode_id_;
    datapoint.type = TuyaDatapointType::ENUM;
    datapoint.value_enum = value;
    this->parent_->set_datapoint_value(datapoint);
  }
  ESP_LOGD(TAG, "Control operation mode not configured");
}

}  // namespace tuya
}  // namespace esphome
