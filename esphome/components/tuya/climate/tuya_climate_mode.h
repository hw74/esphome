#pragma once

#include <cstdint>

namespace esphome {
namespace tuya {

/// Enum for all power levels a climate device can be set at. Much like fan low/mid/high, but more generic. Used in Tuya
/// heaters
enum TuyaPowerLevel : uint8_t {
  // the power level is set to Off (not the same as CLIMATE_MODE_OFF)
  POWER_LEVEL_OFF = 0,
  // the power level is set to Low
  POWER_LEVEL_LOW = 1,
  // the power level is set to Medium
  POWER_LEVEL_MEDIUM = 2,
  // the power level is set to High
  POWER_LEVEL_HIGH = 3,
  POWER_LEVEL_UNKNOWN = 4,
};

/// Enum for all levels a climate device can be set at. Much like fan low/mid/high, but more generic. Used in Tuya
/// heaters
enum TuyaOperationMode : uint8_t {
  // manual operation; heater will use user-specified power level for constant heating
  OPERATION_MODE_MANUAL = 0,
  // program / thermostat mode; heater will use internal thermostat to reach and maintain target temperature
  OPERATION_MODE_PROGRAM = 1,
  OPERATION_MODE_UNKNOWN = 2,
};

/// Convert the given TuyaPowerLevel to a human-readable string.
const char *tuya_power_level_to_string(TuyaPowerLevel level);
/// Convert the given TuyaPowerLevel to a human-readable string.
const char *tuya_operation_mode_to_string(TuyaOperationMode mode);

}  // namespace tuya
}  // namespace esphome