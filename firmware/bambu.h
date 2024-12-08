// filament_mappings.h

#pragma once

#include <unordered_map>
#include <string>

#ifdef ESP_PLATFORM
#include "esp_idf_version.h"
#include "mbedtls/hkdf.h"
#include "mbedtls/md.h"
#else
#include <mbedtls/hkdf.h>
#include <mbedtls/md.h>
#include <ArduinoJson.h>
#endif

namespace bambulabs
{
    struct FilamentInfo {
        std::string color_code;
        std::string type;
    };
    const std::unordered_map<std::string, std::string> filament_mappings = {
        {"TPU", "GFU99"},
        {"PLA", "GFL99"},
        {"PETG", "GFG99"},
        {"PET-CF", "GFG99"},
        {"ASA", "GFB98"},
        {"ABS", "GFB99"},
        {"PC", "GFC99"},
        {"PA", "GFN99"},
        {"PA-CF", "GFN98"},
        {"PLA-CF", "GFL98"},
        {"PVA", "GFS99"},
        {"Support", "GFS00"}};

    // Special cases for brand-specific codes
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> brand_specific_codes = {
        {"PLA", {{"Bambu", "GFA00"}, {"PolyTerra", "GFL01"}, {"PolyLite", "GFL00"}}},
        {"TPU", {{"Bambu", "GFU01"}}},
        {"ABS", {{"Bambu", "GFB00"}}},
        {"PC", {{"Bambu", "GFC00"}}},
        {"PA-CF", {{"Bambu", "GFN03"}}},
        {"PET-CF", {{"Bambu", "GFT00"}}}};

    // Function with two parameters
    inline std::string get_bambu_code(const std::string &type, const std::string &brand = "")
    {
        if (!brand.empty())
        {
            auto brand_it = brand_specific_codes.find(type);
            if (brand_it != brand_specific_codes.end())
            {
                auto code_it = brand_it->second.find(brand);
                if (code_it != brand_it->second.end())
                {
                    return code_it->second;
                }
            }
        }

        auto it = filament_mappings.find(type);
        if (it != filament_mappings.end())
        {
            return it->second;
        }
        return ""; // Unknown type
    }

    // Function with three parameters (for Bambu PLA subtypes)
    inline std::string get_bambu_code(const std::string &type, const std::string &brand, const std::string &subtype)
    {
        if (type == "PLA" && brand == "Bambu")
        {
            if (subtype == "Matte")
                return "GFA01";
            if (subtype == "Metal")
                return "GFA02";
            if (subtype == "Impact")
                return "GFA03";
            return "GFA00"; // Default to Basic for unknown subtypes
        }
        return get_bambu_code(type, brand);
    }

    // uint8_t ams_id, uint8_t ams_tray
    inline std::string generate_mqtt_payload(std::string openspool_tag_json, uint16_t ams_id, uint16_t ams_tray)
    {
        StaticJsonDocument<1024> doc_in;
        StaticJsonDocument<512> doc_out;

        DeserializationError error = deserializeJson(doc_in, openspool_tag_json);
        if (error)
        {
            ESP_LOGE("bambu", "Failed to parse input JSON: %s", error.c_str());
            return {}; // skip publishing
        }

        // Check if 'version' key exists and its value is 1.0
        if (!doc_in.containsKey("version") || doc_in["version"].as<std::string>() != "1.0")
        {
            ESP_LOGE("bambu", "Invalid or missing version. Expected version '1.0'");
            return {}; // skip publishing
        }

        // Check if required fields are present
        const char *required_fields[] = {"color_hex", "min_temp", "max_temp", "brand", "type"};
        for (const char *field : required_fields)
        {
            if (!doc_in.containsKey(field))
            {
                ESP_LOGE("bambu", "Missing required field: %s", field);
                return {}; // skip publishing
            }
        }

        if (doc_in["color_hex"].as<std::string>().length() != 6) {
            ESP_LOGE("bambu", "Invalid color_hex length (expected 6 characters)");
            return {};
        }

        JsonObject print = doc_out.createNestedObject("print");
        print["sequence_id"] = "0";
        print["command"] = "ams_filament_setting";
        print["ams_id"] = ams_id;
        print["tray_id"] = ams_tray;
        print["tray_color"] = doc_in["color_hex"].as<std::string>() + "FF";
        print["nozzle_temp_min"] = uint16_t(doc_in["min_temp"]); // if not string or int, will fall back to 0
        print["nozzle_temp_max"] = uint16_t(doc_in["max_temp"]); // if not string or int, will fall back to 0
        print["tray_type"] = doc_in["type"];
        print["setting_id"] = "";
        print["tray_info_idx"] = get_bambu_code(doc_in["type"], doc_in["brand"]);
        // print["tray_sub_brands"] = doc_in["sub_brand"]; //TODO: support sub brands if needed

        std::string result;
        serializeJson(doc_out, result);

        if (result.empty())
        {
            ESP_LOGE("bambu", "Failed to build JSON");
            return {};
        }

        ESP_LOGI("mqtt", "Publishing %s", result.c_str());
        return result;
    }

    //TODO: move this to a utils file or find a more native esphome way to convert to ascii
    inline std::string hex_to_ascii(const std::string& hex) {
        std::vector<uint8_t> bytes;
        if (esphome::parse_hex(hex, bytes, hex.length() / 2)) {
            return std::string(bytes.begin(), bytes.end());
        }
        return "";
    }

    inline FilamentInfo parse_tag_data(const std::vector<uint8_t>& tag_data) {
        FilamentInfo info;
        const int block_size = 16;
        ESP_LOGD("bambu", "Parsing tag data");

        for (int block = 0; block < 16; ++block) {
            if (block * block_size >= tag_data.size()) {
                break;
            }

            const uint8_t* block_data = tag_data.data() + (block * block_size);

            switch (block) {
                case 0:
                    ESP_LOGV("bambu", "UID: %s", format_hex(block_data, 4).c_str());
                    break;
                case 1:
                    ESP_LOGV("bambu", "Material Variant: %s", format_hex(block_data, 8).c_str());
                    ESP_LOGV("bambu", "Unique Material Type: %s", format_hex(block_data + 8, 8).c_str());
                    break;
                case 2:
                    ESP_LOGVV("bambu", "Filament Type: %s", format_hex(block_data, 16).c_str());
                    info.type = hex_to_ascii(format_hex(block_data, 16));
                    ESP_LOGV("bambu", "Filament Type Ascii: %s", info.type.c_str());
                    break;
                case 4:
                    ESP_LOGV("bambu", "Detailed Filament Type: %s", format_hex(block_data, 16).c_str());
                    break;
                case 5:
                    ESP_LOGV("bambu", "Color Code: %s", format_hex(block_data, 4).c_str());
                    info.color_code = format_hex(block_data, 4);
                    ESP_LOGV("bambu", "Spool Weight: %s", format_hex(block_data + 4, 2).c_str());
                    ESP_LOGV("bambu", "Filament Diameter: %s", format_hex(block_data + 8, 4).c_str());
                    break;
                case 6:
                    ESP_LOGV("bambu", "Temperatures: %s", format_hex(block_data, 8).c_str());
                    ESP_LOGV("bambu", "Drying Info: %s", format_hex(block_data + 8, 8).c_str());
                    break;
                case 8:
                    ESP_LOGV("bambu", "X Cam Info: %s", format_hex(block_data, 8).c_str());
                    ESP_LOGV("bambu", "Nozzle Diameter: %s", format_hex(block_data + 8, 8).c_str());
                    break;
                case 9:
                    ESP_LOGV("bambu", "Tray UID: %s", format_hex(block_data, 16).c_str());
                    break;
                case 10:
                    ESP_LOGV("bambu", "Spool Width: %s", format_hex(block_data, 16).c_str());
                    break;
                case 12:
                    ESP_LOGV("bambu", "Production Date/Time: %s", format_hex(block_data, 16).c_str());
                    break;
                case 13:
                    ESP_LOGV("bambu", "Short Production Date/Time: %s", format_hex(block_data, 16).c_str());
                    break;
                case 14:
                    ESP_LOGV("bambu", "Filament Length: %s", format_hex(block_data, 16).c_str());
                    break;
                case 16:
                    ESP_LOGV("bambu", "Extra Color Info: %s", format_hex(block_data, 16).c_str());
                    break;
            }

            if (block != 0 && block != 1 && block !=2 && block != 3 && block != 7 && block != 11 && block != 15) {
                ESP_LOGV("bambu", "Block %d Data: %s", block, format_hex(block_data, block_size).c_str());
            }
        }
        return info;
    }

}
