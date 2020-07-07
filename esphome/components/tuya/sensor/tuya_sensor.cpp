#include "tuya_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace tuya {

static const char *TAG = "tuya.sensor";

void TuyaSensor::dump_config() { LOG_SENSOR("", "Tuya Sensor", this); }
void TuyaSensor::setup() {
  this->parent_->register_listener(this->datapoint_id_, [this](TuyaDatapoint datapoint) {
    ESP_LOGD(TAG, "MCU reported sensor at datapoint %u, value %d", datapoint.id, datapoint.value_int);
    this->publish_state(datapoint.value_int);
  });
}

}  // namespace tuya
}  // namespace esphome
