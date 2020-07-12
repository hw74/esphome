#include "tuya_climate_mode.h"

namespace esphome {
namespace tuya {

const char *tuya_power_level_to_string(TuyaPowerLevel level) {
  switch (level) {
    case tuya::POWER_LEVEL_OFF:   
      return "OFF";
    case tuya::POWER_LEVEL_LOW:
      return "LOW";
    case tuya::POWER_LEVEL_MEDIUM:
      return "MEDIUM";
    case tuya::POWER_LEVEL_HIGH:
      return "HIGH";
    case tuya::POWER_LEVEL_UNKNOWN:
      return "UNKNOWN";
    default:
      return "UNDEFINED";
  }
}

const char *tuya_operation_mode_to_string(TuyaOperationMode mode) {
  switch (mode) {
    case tuya::OPERATION_MODE_MANUAL:
      return "M";
    case tuya::OPERATION_MODE_PROGRAM:
      return "P";
    case tuya::OPERATION_MODE_UNKNOWN:
      return "UNKNOWN";
    default:
      return "UNDEFINED";
  }
}
}  // namespace tuya
}  // namespace esphome