#pragma once

class MyCustomLightOutput : public Component, public LightOutput {
 public:
  void setup() override {
    // This will be called by App.setup()
    // pinMode(5, INPUT);
  }
  LightTraits get_traits() override {
    // return the traits this light supports
    auto traits = LightTraits();
    // traits.set_supported_color_modes({ColorMode::RGB, ColorMode::BRIGHTNESS});
    traits.set_supported_color_modes({ColorMode::RGB});
    return traits;
  }

  void write_state(LightState *state) override {
    // This will be called by the light to get a new state to be written.
    float red, green, blue;
    // use any of the provided current_values methods
    state->current_values_as_rgb(&red, &green, &blue);
    // Write red, green and blue to HW
    // ...
  }
};


// #include "esphome.h"
// #include "esphome/core/component.h"
// #include "esphome/components/light/light_output.h"


// class RGBOnlyLight : public Component, public light::LightOutput {
//  public:
//   void setup() override {
//     // Initialization if needed
//   }

//   light::LightTraits get_traits() override {
//     auto traits = light::LightTraits();
//     traits.supports_color_capability({light::ColorCapability::RGB});

//     return traits;
//   }

//   void write_state(light::LightState *state) override {
//     float red, green, blue;
//     state->current_values_as_rgb(&red, &green, &blue);
    
//     // Convert to 0-255 range, always at full intensity
//     uint8_t r = red * 255;
//     uint8_t g = green * 255;
//     uint8_t b = blue * 255;

//     // Your RGB handling code here
//     // For example:
//     // set_rgb(r, g, b);
//   }

//  protected:
//   void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
//     // Implement your RGB output logic here
//   }
// };