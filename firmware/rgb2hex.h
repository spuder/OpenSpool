#pragma once

#ifndef RGB2HEX_H
#define RGB2HEX_H

#include "esphome/core/color.h"

namespace esphome {

Color rgb2hex(int red, int green, int blue) {
    // Normalize RGB values from 0-255 to 0-1 range
    float r = red / 255.0f;
    float g = green / 255.0f;
    float b = blue / 255.0f;
    return Color(r, g, b);
}

esphome::Color hex2color(int hex) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8) & 0xFF) / 255.0f;
    float b = (hex & 0xFF) / 255.0f;
    return esphome::Color(r, g, b);
}

esphome::Color hex2color(const char* hex) {
    uint32_t color = strtoul(hex, NULL, 16);
    return hex2color(color);
}

//TODO: This isn't returning correctly, white should be FFFFFF but returns FF2600

std::string float2hex(float r, float g, float b) {
    int red = static_cast<int>(std::round(r * 255));
    int green = static_cast<int>(std::round(g * 255));
    int blue = static_cast<int>(std::round(b * 255));
    char hex_string[7];
    snprintf(hex_string, sizeof(hex_string), "%02X%02X%02X", red, green, blue);
    return std::string(hex_string);
}

} // namespace esphome

#endif // RGB2HEX_H
