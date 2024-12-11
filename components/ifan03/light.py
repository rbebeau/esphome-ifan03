import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import (
    CONF_OUTPUT_ID,
)

ifan03_ns = cg.esphome_ns.namespace("ifan03")

IFan03 = ifan03_ns.class_("IFanLight", cg.Component, light.LightOutput)

CONFIG_SCHEMA = light.LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(IFan03),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
