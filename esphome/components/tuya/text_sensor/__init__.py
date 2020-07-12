import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID, CONF_TYPE
from .. import tuya_ns
from ..climate import TuyaClimate

TuyaClimateTextSensor = tuya_ns.class_('TuyaClimateTextSensor',
                                       text_sensor.TextSensor, cg.Component)
TuyaClimateTextSensorType = tuya_ns.enum('TuyaClimateTextSensorType')

TUYA_CLIMATE_TEXT_SENSOR_TYPES = {
    'climate_power_level': TuyaClimateTextSensorType.POWER_LEVEL,
    'climate_operation_mode': TuyaClimateTextSensorType.OPERATION_MODE,
}
CONF_CLIMATE_ID = 'climate_id'
CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(TuyaClimateTextSensor),
    cv.GenerateID(CONF_CLIMATE_ID): cv.use_id(TuyaClimate),

    cv.Required(CONF_TYPE): cv.enum(TUYA_CLIMATE_TEXT_SENSOR_TYPES, lower=True),
})


def to_code(config):
    parent = yield cg.get_variable(config[CONF_CLIMATE_ID])
    var = cg.new_Pvariable(config[CONF_ID])
    yield text_sensor.register_text_sensor(var, config)
    yield cg.register_component(var, config)

    cg.add(var.set_parent(parent))
    cg.add(var.set_type(config[CONF_TYPE]))
