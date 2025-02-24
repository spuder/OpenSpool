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

#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/automation.h"

#include <Arduino.h>
//#include <WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP_SSLClient.h>
#include <ArduinoMqttClient.h>


namespace esphome
{
    namespace rp2040_mqtt
    {
        class RP2040MQTT : public Component
        {
        public:
            RP2040MQTT() = default;

            void setup() override;
            void loop() override;
            void dump_config() override;

            float get_setup_priority() const override;

            void set_mqtt_host(const char *mqtt_host) { this->mqtt_host_ = std::string(mqtt_host); }

            void set_broker_address(const char *mqtt_host) { this->set_mqtt_host(mqtt_host); }

            void set_mqtt_port(uint16_t mqtt_port) { this->mqtt_port_ = mqtt_port; }

            void set_secure(bool secure) { this->secure_ = secure; }

            void set_mqtt_user(const char *mqtt_user) { this->mqtt_user_ = mqtt_user; }

            void set_mqtt_password(const char *mqtt_password) { this->mqtt_password_ = std::string(mqtt_password); }

            void set_password(const char *password) { this->set_mqtt_password(password); }

            void set_authenticate(bool authenticate) { this->authenticate_ = authenticate; }

            void set_enabled(bool enabled) { this->enabled_ = enabled; }

            void set_connected(bool connected) { this->connected_ = connected; }

            bool is_connected() { return this->connected_; }

            void publish(const std::string &topic, const std::string &payload, uint8_t qos = 0, bool retain = false);

            void stop() { this->mqttClient_.stop(); }
       
        protected:
            bool secure_{true};
            std::string mqtt_host_{};
            uint16_t mqtt_port_{0};
            const char *mqtt_user_{nullptr};
            std::string mqtt_password_{};
            bool authenticate_{false};
            bool enabled_{true};
            bool connected_{false};
            ESP_SSLClient ssl_client_{};
            MqttClient mqttClient_{ssl_client_};
            WiFiClient wifiClient_{};
            unsigned long startTime_{0};
            bool waiting_{false};

        };

        template <typename... Ts> class RP2040MQTTEnableAction : public Action<Ts...>
        {
        public:
            RP2040MQTTEnableAction(RP2040MQTT *parent) : parent_(parent) {}
            void play(Ts... x) override { 
                this->parent_->set_enabled(true);
                this->parent_->setup();
                }

        protected:
            RP2040MQTT *parent_;
        };

        template <typename... Ts> class RP2040MQTTDisableAction : public Action<Ts...>
        {
        public:
            RP2040MQTTDisableAction(RP2040MQTT *parent) : parent_(parent) {}
            void play(Ts... x) override { 
                this->parent_->set_enabled(false);
                this->parent_->stop();
                this->parent_->set_connected(false);
                }

        protected:
            RP2040MQTT *parent_;
        };

        template<typename... Ts> class RP2040MQTTPublishAction : public Action<Ts...> {
            public:
             RP2040MQTTPublishAction(RP2040MQTT *parent) : parent_(parent) {}
             TEMPLATABLE_VALUE(std::string, topic)
             TEMPLATABLE_VALUE(std::string, payload)
             TEMPLATABLE_VALUE(uint8_t, qos)
             TEMPLATABLE_VALUE(bool, retain)
           
             void play(Ts... x) override {
               this->parent_->publish(this->topic_.value(x...), this->payload_.value(x...), this->qos_.value(x...),
                                      this->retain_.value(x...));
             }
           
            protected:
            RP2040MQTT *parent_;
           };

        template<typename... Ts> class RP2040MQTTConnectedCondition : public Condition<Ts...> {
            public:
             RP2040MQTTConnectedCondition(RP2040MQTT *parent) : parent_(parent) {}
             bool check(Ts... x) override { return this->parent_->is_connected(); }
           
            protected:
             RP2040MQTT *parent_;
           };

    } // namespace rp2040_mqtt
} // namespace esphome