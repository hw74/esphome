#include "tuya_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace tuya {

static const char *TAG = "tuya.switch";

void TuyaSwitch::dump_config() { LOG_SWITCH("", "Tuya Switch", this); }
void TuyaSwitch::setup() {
  this->parent_->register_listener(this->datapoint_id_, [this](TuyaDatapoint datapoint) {
    ESP_LOGD(TAG, "MCU reported switch at %u has value %s", datapoint.id, ONOFF(datapoint.value_bool));
    this->publish_state(datapoint.value_bool);
  });
}
void TuyaSwitch::write_state(bool state) {
  TuyaDatapoint datapoint{};
  datapoint.id = this->datapoint_id_;
  datapoint.type = TuyaDatapointType::BOOLEAN;
  datapoint.value_bool = state;

  ESP_LOGD(TAG, "sending value %s to datapoint %u", ONOFF(datapoint.value_bool), datapoint.id);
  parent_->set_datapoint_value(datapoint);
}

}  // namespace tuya
}  // namespace esphome
