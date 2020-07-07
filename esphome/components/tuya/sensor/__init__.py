import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID
from .. import tuya_ns, CONF_TUYA_ID, Tuya

DEPENDENCIES = ['tuya']

CONF_DATAPOINT = "datapoint"

TuyaSensor = tuya_ns.class_('TuyaSensor', sensor.Sensor, cg.Component)

CONFIG_SCHEMA = sensor.SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(TuyaSensor),
    cv.GenerateID(CONF_TUYA_ID): cv.use_id(Tuya),
    cv.Required(CONF_DATAPOINT): cv.uint8_t,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    cg.add(var.set_datapoint(config[CONF_DATAPOINT]))

    paren = yield cg.get_variable(config[CONF_TUYA_ID])
    cg.add(var.set_tuya_parent(paren))
