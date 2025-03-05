# Copyright (C) 2025 Drew Green (@agreenbhm)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.automation import Condition
from esphome.const import CONF_ID
from esphome.core import CORE, coroutine_with_priority
from esphome import automation
from esphome.const import (
	CONF_TOPIC,
    CONF_PAYLOAD,
	CONF_QOS,
	CONF_RETAIN
)

rp2040_mqtt_ns = cg.esphome_ns.namespace("rp2040_mqtt")
RP2040MQTTComponent = rp2040_mqtt_ns.class_("RP2040MQTT", cg.Component)
RP2040MQTTConnectedCondition = rp2040_mqtt_ns.class_("RP2040MQTTConnectedCondition", Condition)
RP2040MQTTEnableAction = rp2040_mqtt_ns.class_("RP2040MQTTEnableAction", automation.Action)
RP2040MQTTDisableAction = rp2040_mqtt_ns.class_("RP2040MQTTDisableAction", automation.Action)
RP2040MQTTPublishAction = rp2040_mqtt_ns.class_("RP2040MQTTPublishAction", automation.Action)

MULTI_CONF = True

CONF_SECURE = "secure"
CONF_MQTT_HOST = "mqtt_host"
CONF_MQTT_PORT = "mqtt_port"
CONF_AUTH = "auth"
CONF_MQTT_USER = "mqtt_user"
CONF_MQTT_PASS = "mqtt_pass"


CONFIG_SCHEMA = (
	cv.Schema(
		{
			cv.GenerateID(): cv.declare_id(RP2040MQTTComponent),
			cv.Optional(CONF_MQTT_HOST): cv.string,
			cv.Optional(CONF_MQTT_PORT): cv.port,
			cv.Optional(CONF_SECURE): cv.boolean,
			cv.Optional(CONF_AUTH): cv.boolean,
			cv.Optional(CONF_MQTT_USER): cv.string,
			cv.Optional(CONF_MQTT_PASS): cv.string

		}
	)
		.extend(cv.COMPONENT_SCHEMA)
)

@automation.register_action(
    "rp2040_mqtt.enable",
    RP2040MQTTEnableAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(RP2040MQTTComponent),
        }
    ),
)

async def rp2040_mqtt_enable_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

@automation.register_action(
    "rp2040_mqtt.disable",
    RP2040MQTTDisableAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(RP2040MQTTComponent),
        }
    ),
)

async def rp2040_mqtt_disable_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

MQTT_PUBLISH_ACTION_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(RP2040MQTTComponent),
        cv.Required(CONF_TOPIC): cv.templatable(cv.publish_topic),
        cv.Required(CONF_PAYLOAD): cv.templatable(cv.mqtt_payload),
        cv.Optional(CONF_QOS, default=0): cv.templatable(cv.mqtt_qos),
        cv.Optional(CONF_RETAIN, default=False): cv.templatable(cv.boolean),
    }
)


@automation.register_action(
    "rp2040_mqtt.publish", RP2040MQTTPublishAction, MQTT_PUBLISH_ACTION_SCHEMA
)
async def rp2040_mqtt_publish_action_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_TOPIC], args, cg.std_string)
    cg.add(var.set_topic(template_))

    template_ = await cg.templatable(config[CONF_PAYLOAD], args, cg.std_string)
    cg.add(var.set_payload(template_))
    template_ = await cg.templatable(config[CONF_QOS], args, cg.uint8)
    cg.add(var.set_qos(template_))
    template_ = await cg.templatable(config[CONF_RETAIN], args, bool)
    cg.add(var.set_retain(template_))
    return var



@automation.register_condition(
    "rp2040_mqtt.connected",
    RP2040MQTTConnectedCondition,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(RP2040MQTTComponent),
        }
    ),
)
async def rp2040_mqtt_connected_to_code(config, condition_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(condition_id, template_arg, paren)

def to_code(config):
	var = cg.new_Pvariable(config[CONF_ID])
	if CONF_MQTT_HOST in config:
		cg.add(var.set_mqtt_host(config[CONF_MQTT_HOST]))
	if CONF_MQTT_PORT in config:
		cg.add(var.set_mqtt_port(config[CONF_MQTT_PORT]))
	if CONF_SECURE in config:
		cg.add(var.set_secure(config[CONF_SECURE]))
	if CONF_AUTH in config:
		cg.add(var.set_authenticate(config[CONF_AUTH]))
		if CONF_MQTT_USER in config:
			cg.add(var.set_mqtt_user(config[CONF_MQTT_USER]))
		if CONF_MQTT_PASS in config:
			cg.add(var.set_mqtt_password(config[CONF_MQTT_PASS]))
	# if CORE.is_rp2040:
	#	cg.add_library("mobizt/ESP_SSLClient", None)
	cg.add_library("mobizt/ESP_SSLClient", None)
	cg.add_library("arduino-libraries/ArduinoMqttClient", None)


	yield cg.register_component(var, config)