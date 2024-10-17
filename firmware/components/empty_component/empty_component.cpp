#include "esphome/core/log.h"
#include "empty_component.h"

namespace esphome {
namespace empty_component {

static const char *TAG = "empty_component.component";

void EmptyComponent::setup() {
  // You can set the initial value here if needed
  // this->my_attribute_ = 42;
  this->color_ = "black";
}

void EmptyComponent::loop() {
  // You can use the attribute here if needed
  // ESP_LOGD(TAG, "My attribute value: %d", this->my_attribute_);
}

void EmptyComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Empty component");
  ESP_LOGCONFIG(TAG, "  My attribute: %d", this->my_attribute_);
  ESP_LOGCONFIG(TAG, "  Color: %s", this->color_.c_str());
}

void EmptyComponent::set_color(const std::string &color) {
  this->color_ = color;
}

std::string EmptyComponent::get_color() const {
  return this->color_;
}


}  // namespace empty_component
}  // namespace esphome