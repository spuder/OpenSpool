class SetColorEffect : public esphome::light::LightEffect {
public:
  SetColorEffect(const std::string &name, const std::string &hex_color) 
    : LightEffect(name), hex_color_(hex_color) {}

  void init() override {
    // This method is called when the effect is registered with the light
    // We don't need to do anything special here for this effect
  }

  void apply() override {
    auto *light = static_cast<esphome::light::LightState *>(this->state_);
    esphome::Color color = hex2rgb(hex_color_);
    
    auto call = light->make_call();
    call.set_state(true);
    call.set_red(color.r);
    call.set_green(color.g);
    call.set_blue(color.b);
    call.perform();

    // Update the internal state
    light->current_values.set_red(color.r);
    light->current_values.set_green(color.g);
    light->current_values.set_blue(color.b);
    light->remote_values = light->current_values;
    light->publish_state();
  }

protected:
  std::string hex_color_;

  esphome::Color hex2rgb(const std::string &hex) {
    int r, g, b;
    sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
    return esphome::Color(r / 255.0f, g / 255.0f, b / 255.0f);
  }

};