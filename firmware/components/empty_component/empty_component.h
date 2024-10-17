#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace empty_component {

class EmptyComponent : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  // Add getter and setter for your new attribute
  void set_my_attribute(int value) { my_attribute_ = value; }
  int get_my_attribute() const { return my_attribute_; }

  void set_color(const std::string &color);
  std::string get_color() const;

 private:
  int my_attribute_{0};  // Default value is 0
  std::string color_{"purple"};
  std::string type_{};
};

}  // namespace empty_component
}  // namespace esphome