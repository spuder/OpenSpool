#include "filament.h"
#include "esphome/core/log.h"

namespace esphome {
namespace filament {

static const char *TAG = "filament.component";

void Filament::setup() {
  ESP_LOGD(TAG, "Setting up Filament component...");
  // Add your setup code here
}

void Filament::loop() {
  // Add your loop code here
}

void Filament::dump_config() {
  ESP_LOGCONFIG(TAG, "Filament:");
  ESP_LOGCONFIG(TAG, "  Type: %s", this->type_.c_str());
  ESP_LOGCONFIG(TAG, "  Color: %s", this->color_.c_str());
  ESP_LOGCONFIG(TAG, "  Color Hex: %s", this->get_color_hex().c_str());
}

std::string Filament::get_color_hex() const {
  auto it = color_lookup_.find(color_);
  if (it != color_lookup_.end()) {
    return it->second;
  }
  return "FFFFFFFF";  // Default to white if color not found
}

}  // namespace filament
}  // namespace esphome