from esphome.components import climate
from esphome import automation
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID, CONF_SWITCH_DATAPOINT, CONF_TYPE
from .. import tuya_ns, CONF_TUYA_ID, Tuya

DEPENDENCIES = ['tuya']

CONF_TARGET_TEMPERATURE_DATAPOINT = "target_temperature_datapoint"
CONF_CURRENT_TEMPERATURE_DATAPOINT = "current_temperature_datapoint"
# CONF_ECO_MODE_DATAPOINT = "eco_mode_datapoint"
CONF_POWER_LEVEL = 'power_level'
CONF_PL_DATAPOINT = 'datapoint_id'
CONF_PL_LOW = 'enum_low'
CONF_PL_MEDIUM = 'enum_medium'
CONF_PL_HIGH = 'enum_high'
CONF_PL_OFF = 'enum_off'

CONF_OPERATION_MODE = 'operation_mode'
CONF_OM_DATAPOINT = 'datapoint_id'
CONF_OM_MANUAL = 'enum_manual'
CONF_OM_PROGRAM = 'enum_program'

CONF_ACTION_POWER_LEVEL = 'power_level'
CONF_ACTION_OPERATION_MODE = 'operation_mode'

TuyaClimate = tuya_ns.class_('TuyaClimate', climate.Climate, cg.Component)

TuyaPowerLevel = tuya_ns.enum('TuyaPowerLevel')
HLEnumDefault = tuya_ns.enum('TuyaPowerLevelDataPointEnumDefault')

TuyaOperationMode = tuya_ns.enum('TuyaOperationMode')
OMEnumDefault = tuya_ns.enum('TuyaOperationModeDataPointEnumDefault')

TuyaClimateSetAction = tuya_ns.class_('TuyaClimateSetAction', automation.Action)
POWER_LEVEL_ACTION_TYPES = {
    'off': TuyaPowerLevel.POWER_LEVEL_OFF,
    'low': TuyaPowerLevel.POWER_LEVEL_LOW,
    'medium': TuyaPowerLevel.POWER_LEVEL_MEDIUM,
    'high': TuyaPowerLevel.POWER_LEVEL_HIGH,
}
OPERATION_MODE_ACTION_TYPES = {
    'manual': TuyaOperationMode.OPERATION_MODE_MANUAL,
    'program': TuyaOperationMode.OPERATION_MODE_PROGRAM,
}

CONFIG_SCHEMA = cv.All(climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(TuyaClimate),
    cv.GenerateID(CONF_TUYA_ID): cv.use_id(Tuya),
    cv.Optional(CONF_SWITCH_DATAPOINT): cv.uint8_t,
    cv.Optional(CONF_TARGET_TEMPERATURE_DATAPOINT): cv.uint8_t,
    cv.Optional(CONF_CURRENT_TEMPERATURE_DATAPOINT): cv.uint8_t,
    # cv.Optional(CONF_ECO_MODE_DATAPOINT): cv.uint8_t,
    cv.Optional(CONF_POWER_LEVEL): cv.Schema({
        cv.Required(CONF_PL_DATAPOINT): cv.uint8_t,
        cv.Optional(CONF_PL_LOW): cv.uint8_t,
        cv.Optional(CONF_PL_MEDIUM): cv.uint8_t,
        cv.Optional(CONF_PL_HIGH): cv.uint8_t,
        cv.Optional(CONF_PL_OFF): cv.uint8_t,
    }),
    cv.Optional(CONF_OPERATION_MODE): cv.Schema({
        cv.Required(CONF_OM_DATAPOINT): cv.uint8_t,
        cv.Optional(CONF_OM_MANUAL): cv.uint8_t,
        cv.Optional(CONF_OM_PROGRAM): cv.uint8_t,
    }),
}).extend(cv.COMPONENT_SCHEMA), cv.has_at_least_one_key(
    CONF_TARGET_TEMPERATURE_DATAPOINT, CONF_SWITCH_DATAPOINT))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)

    paren = yield cg.get_variable(config[CONF_TUYA_ID])
    cg.add(var.set_tuya_parent(paren))

    if CONF_SWITCH_DATAPOINT in config:
        cg.add(var.set_switch_id(config[CONF_SWITCH_DATAPOINT]))
    if CONF_TARGET_TEMPERATURE_DATAPOINT in config:
        cg.add(var.set_target_temperature_id(config[CONF_TARGET_TEMPERATURE_DATAPOINT]))
    if CONF_CURRENT_TEMPERATURE_DATAPOINT in config:
        cg.add(var.set_current_temperature_id(config[CONF_CURRENT_TEMPERATURE_DATAPOINT]))
    # if CONF_ECO_MODE_DATAPOINT in config:
    #     cg.add(var.set_eco_mode_id(config[CONF_ECO_MODE_DATAPOINT]))
    if CONF_POWER_LEVEL in config:
        params = config[CONF_POWER_LEVEL]
        cg.add(var.set_power_level_id(params[CONF_PL_DATAPOINT]))
        if len(params) == 1:
            cg.add(var.set_power_level_enum_low(HLEnumDefault.DEFAULT_HL_ENUM_LOW))
            cg.add(var.set_power_level_enum_medium(HLEnumDefault.DEFAULT_HL_ENUM_MEDIUM))
            cg.add(var.set_power_level_enum_high(HLEnumDefault.DEFAULT_HL_ENUM_HIGH))
            cg.add(var.set_power_level_enum_off(HLEnumDefault.DEFAULT_HL_ENUM_OFF))
        if CONF_PL_LOW in params:
            cg.add(var.set_power_level_enum_low(params[CONF_PL_LOW]))
        if CONF_PL_MEDIUM in params:
            cg.add(var.set_power_level_enum_medium(params[CONF_PL_MEDIUM]))
        if CONF_PL_HIGH in params:
            cg.add(var.set_power_level_enum_high(params[CONF_PL_HIGH]))
        if CONF_PL_OFF in params:
            cg.add(var.set_power_level_enum_off(params[CONF_PL_OFF]))

    if CONF_OPERATION_MODE in config:
        params = config[CONF_OPERATION_MODE]
        cg.add(var.set_operation_mode_id(params[CONF_OM_DATAPOINT]))
        if len(params) == 1:
            cg.add(var.set_operation_mode_enum_manual(OMEnumDefault.DEFAULT_OM_ENUM_MANUAL))
            cg.add(var.set_operation_mode_enum_program(OMEnumDefault.DEFAULT_OM_ENUM_PROGRAM))
        if CONF_OM_MANUAL in params:
            cg.add(var.set_operation_mode_enum_manual(params[CONF_OM_MANUAL]))
        if CONF_OM_PROGRAM in params:
            cg.add(var.set_operation_mode_enum_program(params[CONF_OM_PROGRAM]))

@automation.register_action('climate.tuya.set', TuyaClimateSetAction, automation.maybe_simple_id({
    cv.Required(CONF_ID): cv.use_id(TuyaClimate),
    cv.Optional(CONF_ACTION_POWER_LEVEL): cv.templatable(cv.enum(POWER_LEVEL_ACTION_TYPES,
                                                                 lower=True)),
    cv.Optional(CONF_ACTION_OPERATION_MODE): cv.templatable(cv.enum(OPERATION_MODE_ACTION_TYPES,
                                                                    lower=True)),
}))
def climate_action_set_to_code(config, action_id, template_arg, args):
    paren = yield cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    if CONF_ACTION_POWER_LEVEL in config:
        template_ = yield cg.templatable(config[CONF_ACTION_POWER_LEVEL], args, TuyaPowerLevel)
        cg.add(var.set_power_level(template_))
    if CONF_ACTION_OPERATION_MODE in config:
        template_ = yield cg.templatable(config[CONF_ACTION_OPERATION_MODE], args, TuyaOperationMode)
        cg.add(var.set_operation_mode(template_))
    yield var
