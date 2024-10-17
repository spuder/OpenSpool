import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CONF_MY_ATTRIBUTE = "my_attribute"

empty_component_ns = cg.esphome_ns.namespace('empty_component')
EmptyComponent = empty_component_ns.class_('EmptyComponent', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(EmptyComponent),
    cv.Optional(CONF_MY_ATTRIBUTE, default=0): cv.int_
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    if CONF_MY_ATTRIBUTE in config:
        cg.add(var.set_my_attribute(config[CONF_MY_ATTRIBUTE]))