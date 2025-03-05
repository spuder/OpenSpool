/* Copyright (C) 2025 Drew Green (@agreenbhm)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "rp2040_mqtt.h"

#include "esphome/core/log.h"
#include "esphome/core/util.h"
#include "esphome/components/network/util.h"

namespace esphome
{
    namespace rp2040_mqtt
    {
        float RP2040MQTT::get_setup_priority() const { return setup_priority::AFTER_WIFI; }
        static const char *TAG = "rp2040_mqtt";

        void RP2040MQTT::setup()
        {
            if (!this->enabled_)
            {
                ESP_LOGE(TAG, "RP2040MQTT is disabled");
                return;
            }
            ESP_LOGCONFIG(TAG, "Setting up RP2040MQTT...");

            this->ssl_client_.setInsecure();

            this->ssl_client_.setBufferSizes(16384, 512);
            this->ssl_client_.setDebugLevel(0);
            this->ssl_client_.setClient(&this->wifiClient_, this->secure_);

            this->ssl_client_.setSessionTimeout(150);

            this->mqttClient_.stop();

            if (this->authenticate_)
            {
                this->mqttClient_.setUsernamePassword(this->mqtt_user_, this->mqtt_password_.c_str());
            }

            ESP_LOGD(TAG, "Connecting MQTT to %s:%d", this->mqtt_host_.c_str(), this->mqtt_port_);
            if (!(this->mqttClient_.connect(this->mqtt_host_.c_str(), this->mqtt_port_)))
            {
                ESP_LOGE(TAG, "Failed to connect to MQTT server");
                return;
            }
            else
            {
                ESP_LOGD(TAG, "Connected to MQTT server");
                this->connected_ = true;
            }
        }

        void RP2040MQTT::loop()
        {
            if (!this->enabled_)
            {
                return;
            }

            if (!this->mqttClient_.connected())
            {
                if (!this->waiting_)
                {
                    this->mqttClient_.stop();
                    ESP_LOGE(TAG, "MQTT client disconnected");
                    this->connected_ = false;
                    this->startTime_ = millis();
                    this->waiting_ = true;
                }
                else
                {
                    if ((millis() - this->startTime_) > 15000)
                    {
                        this->waiting_ = false;
                        ESP_LOGE(TAG, "Reconnecting to MQTT server");
                        this->setup();
                        return;
                    }
                }

            } else{
                this->mqttClient_.poll();
                //delay(10);
            }
        }

        void RP2040MQTT::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Broker: %s:%u", this->mqtt_host_, this->mqtt_port_);
        }

        void RP2040MQTT::publish(const std::string &topic, const std::string &payload, uint8_t qos, bool retain)
        {

            this->mqttClient_.beginMessage(topic.c_str());
            this->mqttClient_.print(payload.c_str());
            this->mqttClient_.endMessage();
        }

    } // namespace rp2040_mqtt
} // namespace esphome