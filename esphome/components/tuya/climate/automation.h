#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "tuya_climate.h"

namespace esphome {
namespace tuya {

template<typename... Ts> class TuyaClimateSetAction : public Action<Ts...> {
 public:
  TuyaClimateSetAction(TuyaClimate *climate) : climate_(climate) {}

  TEMPLATABLE_VALUE(TuyaPowerLevel, power_level)
  TEMPLATABLE_VALUE(TuyaOperationMode, operation_mode)

  void play(Ts... x) {
    // always first try to set the operation mode, since that will generally influence whether you can set the power level.
    // That way you can go from program mode to manual + selected power level at once. "on my heater" you cannot set the
    // power level while in program mode
    if (this->operation_mode_.has_value())
      this->climate_->control_operation_mode(operation_mode_.value(x...));
    if (this->power_level_.has_value())
      this->climate_->control_power_level(power_level_.value(x...));
  }

  void set_power_level(TuyaPowerLevel level) { power_level_ = level; }
  void set_operation_mode(TuyaOperationMode mode) { operation_mode_ = mode; }

 protected:
  TuyaClimate *climate_;
};

}  // namespace tuya
}  // namespace esphome
