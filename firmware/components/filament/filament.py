import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.automation import Action
from esphome import automation



CODEOWNERS = ["@YourGitHubUsername"]

CONF_FILAMENT = "filament"
CONF_TYPE = "type"
CONF_COLOR = "color"
CONF_COLOR_LOOKUP = "color_lookup"

filament_ns = cg.esphome_ns.namespace("filament")
Filament = filament_ns.class_("Filament", cg.Component)
FilamentDumpConfigAction = filament_ns.class_("FilamentDumpConfigAction", Action)

COLOR_LOOKUP_SCHEMA = cv.Schema({cv.string: cv.string})

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Filament),
    cv.Required(CONF_TYPE): cv.string,
    cv.Required(CONF_COLOR): cv.string,
    cv.Optional(CONF_COLOR_LOOKUP, default={}): COLOR_LOOKUP_SCHEMA,
}).extend(cv.COMPONENT_SCHEMA)

@automation.register_action("filament.dump_config", FilamentDumpConfigAction, cv.Schema({
    cv.GenerateID(): cv.use_id(Filament),
}))
async def filament_dump_config_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_type(config[CONF_TYPE]))
    cg.add(var.set_color(config[CONF_COLOR]))
    
    color_lookup = config[CONF_COLOR_LOOKUP]
    if color_lookup:
        for color, hex_value in color_lookup.items():
            cg.add(var.add_color_lookup(color, hex_value))