#pragma once

#include "esphome/core/component.h"
#include "esphome/components/tuya/tuya.h"
#include "esphome/components/climate/climate.h"
#include "tuya_climate_mode.h"

namespace esphome {
namespace tuya {

// default expected enum values returned by tuya MCU power level datapoint
enum TuyaPowerLevelDataPointEnumDefault {
  DEFAULT_HL_ENUM_LOW = 0,
  DEFAULT_HL_ENUM_MEDIUM = 1,
  DEFAULT_HL_ENUM_HIGH = 2,
  DEFAULT_HL_ENUM_OFF = 3,
};

enum TuyaOperationModeDataPointEnumDefault {
  DEFAULT_OM_ENUM_MANUAL = 0,
  DEFAULT_OM_ENUM_PROGRAM = 1,
};


class TuyaClimate : public climate::Climate, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void set_switch_id(uint8_t switch_id) { this->switch_id_ = switch_id; }
  void set_target_temperature_id(uint8_t target_temperature_id) {
    this->target_temperature_id_ = target_temperature_id;
  }
  void set_current_temperature_id(uint8_t current_temperature_id) {
    this->current_temperature_id_ = current_temperature_id;
  }
  // void set_eco_mode_id(uint8_t eco_mode_id) { this->eco_mode_id_ = eco_mode_id; }
  void set_power_level_id(uint8_t POWER_LEVEL_id) { this->power_level_id_ = POWER_LEVEL_id; }
  void set_mcu_reported_power_level(TuyaPowerLevel level);
  optional<TuyaPowerLevel> get_power_level() { return this->power_level_; };
  void add_on_power_level_reported_callback(std::function<void()> &&callback) {
    power_level_reported_callback_.add(std::move(callback));
  }
  void set_power_level_enum_low(uint8_t enum_value) { this->power_level_enum_low_ = enum_value; }
  void set_power_level_enum_medium(uint8_t enum_value) { this->power_level_enum_medium_ = enum_value; }
  void set_power_level_enum_high(uint8_t enum_value) { this->power_level_enum_high_ = enum_value; }
  void set_power_level_enum_off(uint8_t enum_value) { this->power_level_enum_off_ = enum_value; }
  void control_power_level(TuyaPowerLevel level);

  void set_operation_mode_id(uint8_t operation_mode_id) { this->operation_mode_id_ = operation_mode_id; }
  void set_mcu_reported_operation_mode(TuyaOperationMode mode);
  optional<TuyaOperationMode> get_operation_mode() { return this->operation_mode_; };
  void add_on_operation_mode_reported_callback(std::function<void()> &&callback) {
    operation_mode_reported_callback_.add(std::move(callback));
  }
  void set_operation_mode_enum_manual(uint8_t enum_value) { this->operation_mode_enum_manual_ = enum_value; }
  void set_operation_mode_enum_program(uint8_t enum_value) { this->operation_mode_enum_program_ = enum_value; }
  void control_operation_mode(TuyaOperationMode mode);

  void set_tuya_parent(Tuya *parent) { this->parent_ = parent; }

 protected:
  /// Override control to change settings of the climate device.
  void control(const climate::ClimateCall &call) override;
  /// Return the traits of this controller.
  climate::ClimateTraits traits() override;

  /// Re-compute the state of this climate controller.
  void compute_state_();

  /// Switch the climate device to the given climate mode.
  void switch_to_action_(climate::ClimateAction action);

  bool supports_POWER_LEVEL_();
  bool supports_operation_mode_();

  Tuya *parent_;
  optional<uint8_t> switch_id_{};
  optional<uint8_t> target_temperature_id_{};
  optional<uint8_t> current_temperature_id_{};
  // optional<uint8_t> eco_mode_id_{};
  optional<uint8_t> power_level_id_{};
  optional<TuyaPowerLevel> power_level_{};
  CallbackManager<void()> power_level_reported_callback_;
  optional<uint8_t> power_level_enum_low_{};
  optional<uint8_t> power_level_enum_medium_{};
  optional<uint8_t> power_level_enum_high_{};
  optional<uint8_t> power_level_enum_off_{};

  optional<uint8_t> operation_mode_id_{};
  optional<TuyaOperationMode> operation_mode_{};
  CallbackManager<void()> operation_mode_reported_callback_;
  optional<uint8_t> operation_mode_enum_manual_{};
  optional<uint8_t> operation_mode_enum_program_{};
};

}  // namespace tuya
}  // namespace esphome
