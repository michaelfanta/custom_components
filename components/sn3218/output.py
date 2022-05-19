import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_CHANNEL, CONF_ID
from . import SN3218Component, sn3218_ns

DEPENDENCIES = ['sn3218']

PCA9634Channel = sn3218_ns.class_('SN3218Channel', output.FloatOutput)
CONF_SN3218_ID = 'sn3218_id'

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend({
    cv.Required(CONF_ID): cv.declare_id(PCA9634Channel),
    cv.GenerateID(CONF_SN3218_ID): cv.use_id(SN3218Component),
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=17)
})

async def to_code(config):
    paren = await cg.get_variable(config[CONF_SN3218_ID])
    rhs = paren.create_channel(config[CONF_CHANNEL])
    var = cg.Pvariable(config[CONF_ID], rhs)
    await output.register_output(var, config)
