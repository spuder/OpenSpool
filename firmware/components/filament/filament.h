#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace filament {

class Filament : public esphome::Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_type(const std::string &type) { type_ = type; }
  void set_color(const std::string &color) { color_ = color; }

  std::string get_type() const { return type_; }
  std::string get_color() const { return color_; }
  std::string get_color_hex() const;

 protected:
  std::string type_;
  std::string color_;
  const std::unordered_map<std::string, std::string> color_lookup_ = {
    {"black", "161616FF"},
    {"yellow", "FFF144FF"},
    {"bambu_green", "0ACC38FF"},
    {"blue", "2850E0FF"},
    {"purple", "A03CF7FF"}
  };
};

}  // namespace filament
}  // namespace esphome