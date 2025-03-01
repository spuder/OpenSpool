// filament_mappings.h

#pragma once

#include <unordered_map>
#include <string>

namespace bambulabs
{
    const std::unordered_map<std::string, std::string> filament_mappings = {
        {"TPU for AMS", "GFU02"},
        {"TPU High Speed", "GFU00"},
        {"TPU", "GFU99"},
        {"PLA", "GFL99"},
        {"PLA High Speed", "GFL95"},
        {"PLA Silk", "GFL96"},
        {"PETG", "GFG99"},
        {"PET-CF", "GFG99"},
        {"ASA", "GFB98"},
        {"ABS", "GFB99"},
        {"PC", "GFC99"},
        {"PA", "GFN99"},
        {"PA-CF", "GFN98"},
        {"PLA-CF", "GFL98"},
        {"PVA", "GFS99"},
        {"BVOH", "GFS97"},
        {"EVA", "GFR99"},
        {"HIPS", "GFS98"},
        {"PC", "GFC99"},
        {"PCTG", "GFG97"},
        {"PE", "GFP99"},
        {"PE-CF", "GFP98"},
        {"PHA", "GFR98"},
        {"PP", "GFP97"},
        {"PP-CF", "GFP96"},
        {"PP-GF", "GFP95"},
        {"PPA-CF", "GFN97"},
        {"PPA-GF", "GFN96"},
        {"Support", "GFS00"}};

    // Special cases for brand-specific codes
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> brand_specific_codes = {
        {"PLA", {{"Bambu", "GFA00"}, {"PolyTerra", "GFL01"}, {"PolyLite", "GFL00"}, {"Sunlu", "SNL02"}}},
        {"PLA Aero", {{"Bambu", "GFG01"}}},
        {"TPU", {{"Bambu", "GFU01"}}},
        {"ABS", {{"Bambu", "GFB00"}, {"PolyLite", "GFB60"}}},
        {"ASA", {{"Bambu", "GFB01"}, {"PolyLite", "GFB61"}}},
        {"PC", {{"Bambu", "GFC00"}}},
        {"PA-CF", {{"Bambu", "GFN03"}}},
        {"PET-CF", {{"Bambu", "GFT00"}}},
        {"PETG HF", {{"Bambu", "GFG02"}}},
        {"PETG Translucent", {{"Bambu", "GFG01"}}},
        {"PETG", {{"Bambu", "GFG00"}, {"PolyLite", "GFG60"}, {"Sunlu", "SNL08"}}}};

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

        if (doc_in["color_hex"].as<std::string>().length() != 6 && doc_in["color_hex"].as<std::string>().length() != 8) {
            ESP_LOGE("bambu", "Invalid color_hex length (expected 6 or 8 characters)");
            return {};
        }

        JsonObject print = doc_out.createNestedObject("print");
        print["sequence_id"] = "0";
        print["command"] = "ams_filament_setting";
        print["ams_id"] = ams_id;
        print["tray_id"] = ams_tray;
        if (doc_in["color_hex"].as<std::string>().length() == 6) {
            print["tray_color"] = doc_in["color_hex"].as<std::string>() + "FF";
        }
        else{
            print["tray_color"] = doc_in["color_hex"].as<std::string>();
        }
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

}