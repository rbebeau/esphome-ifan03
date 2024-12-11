import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome import automation
from esphome.automation import maybe_simple_id

from esphome.const import (
    CONF_OUTPUT_ID,
    CONF_ID,
)
from esphome.core import  coroutine_with_priority

BUZZER_ENABLE = "buzzer_enable"

ifan03_ns = cg.esphome_ns.namespace("ifan03")
IFan03 = ifan03_ns.class_("IFan03", cg.Component, fan.Fan)
CycleSpeedAction = ifan03_ns.class_("CycleSpeedAction", automation.Action)

CONFIG_SCHEMA = fan.FAN_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(IFan03),
        cv.Optional(BUZZER_ENABLE, default=True): cv.boolean,
    }
).extend(cv.COMPONENT_SCHEMA)
FAN_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(IFan03),
    }
)
@automation.register_action("ifan03.cycle_speed", CycleSpeedAction, FAN_ACTION_SCHEMA)
async def fan_cycle_speed_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

@coroutine_with_priority(100.0)

async def to_code(config):
    cg.add_define("USE_FAN")
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    cg.add(var.set_buzzer_enable(config[BUZZER_ENABLE]))
    await cg.register_component(var, config)

    await fan.register_fan(var, config)

    
    cg.add_global(ifan03_ns.using)
