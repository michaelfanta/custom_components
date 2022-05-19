import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import CONF_OUTPUT_ID, CONF_OUTPUT
from . import SN3218Component, sn3218_ns

DEPENDENCIES = ['sn3218']

SN3218Light = sn3218_ns.class_('SN3218Light', light.LightOutput)
CONF_SN3218_ID = 'sn3218_id'

CONFIG_SCHEMA = light.BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(SN3218Light),
    cv.GenerateID(CONF_SN3218_ID): cv.use_id(SN3218Component),
    cv.Required(CONF_OUTPUT): cv.use_id(output.FloatOutput),
})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    yield light.register_light(var, config)
    out = yield cg.get_variable(config[CONF_OUTPUT])
    cg.add(var.set_output(out))
