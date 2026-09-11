// filament_mappings.h

#pragma once

#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>

#include "esphome/components/nfc/nfc_tag.h"

// Native decoding for the OpenTag3D memory maps: https://opentag3d.info/spec
namespace opentag3d
{
    struct Color
    {
        uint8_t r{0};
        uint8_t g{0};
        uint8_t b{0};
        uint8_t a{255};
    };

    struct Tag
    {
        uint16_t version{0}; // eg. 2000 -> v2.000
        std::string material;
        std::string modifier;
        std::string manufacturer;
        Color color;
        float diameter{0};
        float density{0};
        uint16_t target_weight{0};
        uint16_t print_temp{0};
        uint16_t min_print_temp{0};
        uint16_t max_print_temp{0};
        uint16_t chamber_temp{0};
        uint16_t bed_temp{0};
        uint16_t min_bed_temp{0};
        uint16_t max_bed_temp{0};
    };

    // Bytes beyond the end of the payload are treated as 0x00, per the reader implementation guidelines.
    inline uint8_t read_u8(const std::string &payload, size_t offset)
    {
        return offset < payload.size() ? static_cast<uint8_t>(payload[offset]) : 0;
    }

    inline uint16_t read_u16(const std::string &payload, size_t offset)
    {
        return (static_cast<uint16_t>(read_u8(payload, offset)) << 8) | read_u8(payload, offset + 1);
    }

    inline std::string read_string(const std::string &payload, size_t offset, size_t length)
    {
        if (offset >= payload.size())
        {
            return {};
        }
        const size_t end = std::min({payload.find('\0', offset), offset + length, payload.size()});
        return payload.substr(offset, end - offset);
    }

    inline Color read_color(const std::string &payload, size_t offset)
    {
        return {read_u8(payload, offset), read_u8(payload, offset + 1), read_u8(payload, offset + 2),
                read_u8(payload, offset + 3)};
    }

    // Decodes the v1 core/extended memory map: https://opentag3d.info/assets/json/spec_v1.json
    inline bool decode_v1(const std::string &payload, Tag &tag)
    {
        tag.material = read_string(payload, 0x02, 5);
        tag.modifier = read_string(payload, 0x07, 5);
        tag.manufacturer = read_string(payload, 0x1B, 16);
        tag.color = read_color(payload, 0x4B);
        tag.diameter = read_u16(payload, 0x5C) / 1000.0f;
        tag.target_weight = read_u16(payload, 0x5E);
        tag.print_temp = read_u8(payload, 0x60) * 5;
        tag.bed_temp = read_u8(payload, 0x61) * 5;
        tag.density = read_u16(payload, 0x62) / 1000.0f;
        tag.min_print_temp = read_u8(payload, 0xB4) * 5;
        tag.max_print_temp = read_u8(payload, 0xB5) * 5;
        tag.min_bed_temp = read_u8(payload, 0xB6) * 5;
        tag.max_bed_temp = read_u8(payload, 0xB7) * 5;

        return !tag.material.empty() && !tag.manufacturer.empty() && tag.diameter > 0 && tag.print_temp != 0 &&
               tag.bed_temp != 0 && tag.density > 0 && tag.target_weight != 0;
    }

    // Decodes the v2 core memory map: https://opentag3d.info/spec
    inline bool decode_v2(const std::string &payload, Tag &tag)
    {
        tag.material = read_string(payload, 0x02, 5);
        tag.modifier = read_string(payload, 0x07, 5);
        tag.manufacturer = read_string(payload, 0x0C, 16);
        tag.color = read_color(payload, 0x3C);
        tag.diameter = read_u16(payload, 0x8C) / 1000.0f;
        tag.print_temp = read_u8(payload, 0x90) * 5;
        tag.min_print_temp = read_u8(payload, 0x91) * 5;
        tag.max_print_temp = read_u8(payload, 0x92) * 5;
        tag.chamber_temp = read_u8(payload, 0x93) * 5;
        tag.bed_temp = read_u8(payload, 0x94) * 5;
        tag.min_bed_temp = read_u8(payload, 0x95) * 5;
        tag.max_bed_temp = read_u8(payload, 0x96) * 5;
        tag.density = read_u16(payload, 0x9C) / 1000.0f;
        tag.target_weight = read_u16(payload, 0x9E);

        return !tag.material.empty() && !tag.manufacturer.empty() && tag.diameter > 0 && tag.print_temp != 0 && tag.bed_temp != 0 && tag.density > 0 && tag.target_weight != 0;
    }

    // Parses an `application/opentag3d` NDEF payload into its native fields.
    inline bool decode(const std::string &payload, Tag &tag)
    {
        tag.version = read_u16(payload, 0x00);
        const uint16_t major_version = tag.version / 1000;

        bool ok = false;
        if (major_version == 1)
        {
            ok = decode_v1(payload, tag);
        }
        else if (major_version == 2)
        {
            ok = decode_v2(payload, tag);
        }
        else
        {
            ESP_LOGE("opentag3d", "Tag version %u is not a supported major memory map version", tag.version);
            return false;
        }

        if (!ok)
        {
            ESP_LOGW("opentag3d", "OpenTag3D payload is corrupt or missing one or more required fields");
        }
        return ok;
    }

    // Combines the base material and its modifier into a single type string, eg. "PLA-CF".
    inline std::string format_type(const Tag &tag)
    {
        if (tag.modifier.empty())
        {
            return tag.material;
        }
        const char separator = (tag.modifier == "CF" || tag.modifier == "GF") ? '-' : ' ';
        return tag.material + separator + tag.modifier;
    }

    // Renders the raw NDEF payload bytes as a space-separated hex string for display/debugging.
    inline std::string to_hex_string(const std::string &payload)
    {
        static const char *const hex_digits = "0123456789ABCDEF";
        std::string result;
        result.reserve(payload.size() * 3);
        for (size_t i = 0; i < payload.size(); i++)
        {
            if (i)
                result += ' ';
            const uint8_t byte = static_cast<uint8_t>(payload[i]);
            result += hex_digits[byte >> 4];
            result += hex_digits[byte & 0x0F];
        }
        return result;
    }
}

namespace openspool
{
    struct Tag
    {
        std::string color_hex;
        std::string type;
        std::string brand;
        uint16_t min_temp{0};
        uint16_t max_temp{0};
    };

    // Parses and validates the OpenSpool JSON tag format.
    inline bool decode(const std::string &payload, Tag &tag)
    {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            ESP_LOGE("openspool", "Failed to parse input JSON: %s", error.c_str());
            return false;
        }

        if (!doc.is<JsonObject>())
        {
            ESP_LOGE("openspool", "Invalid JSON: expected an object");
            return false;
        }

        if (doc["version"].isNull() || doc["version"].as<std::string>() != "1.0")
        {
            ESP_LOGE("openspool", "Invalid or missing version. Expected version '1.0'");
            return false;
        }

        if (doc["protocol"].as<std::string>() != "openspool")
        {
            return false;
        }

        const char *required_fields[] = {"color_hex", "min_temp", "max_temp", "brand", "type"};
        for (const char *field : required_fields)
        {
            if (doc[field].isNull())
            {
                ESP_LOGE("openspool", "Missing required field: %s", field);
                return false;
            }
        }

        tag.color_hex = doc["color_hex"].as<std::string>();
        tag.type = doc["type"].as<std::string>();
        tag.brand = doc["brand"].as<std::string>();
        tag.min_temp = doc["min_temp"].as<uint16_t>();
        tag.max_temp = doc["max_temp"].as<uint16_t>();

        if (tag.color_hex.length() != 6 && tag.color_hex.length() != 8)
        {
            ESP_LOGE("openspool", "Invalid color_hex length (expected 6 or 8 characters)");
            return false;
        }

        return true;
    }

    inline std::string format_display(const std::string &payload)
    {
        if (payload.empty())
        {
            ESP_LOGD("NFC", "Input string is empty");
            return payload;
        }
        ESP_LOGD("NFC", "Input string: %s", payload.c_str());

        if (payload[0] != '{')
        {
            return payload;
        }

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            ESP_LOGE("NFC", "JSON parsing failed: %s", error.c_str());
            return {};
        }

        if (!doc.is<JsonObject>())
        {
            ESP_LOGE("NFC", "Invalid JSON: Not an object");
            return {};
        }

        const char *required_fields[] = {"protocol", "color_hex", "type", "min_temp", "max_temp", "brand"};
        for (const char *field : required_fields)
        {
            if (!doc[field].is<const char *>() && !doc[field].is<int>() && !doc[field].is<float>())
            {
                ESP_LOGE("NFC", "Invalid JSON: Missing required field '%s'", field);
                return {};
            }
        }

        std::string output;
        serializeJsonPretty(doc, output);
        if (output.length() > 1024)
        {
            ESP_LOGE("NFC", "Prettified JSON exceeds 1024 bytes");
            return {};
        }
        return output;
    }
}

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
        {"PLA", {{"Bambu", "GFA00"}, {"PolyTerra", "GFL01"}, {"PolyLite", "GFL00"}, {"Sunlu", "GFSNL03"}}},
        {"PLA Aero", {{"Bambu", "GFG01"}}},
        {"TPU", {{"Bambu", "GFU01"}}},
        {"ABS", {{"Bambu", "GFB00"}, {"PolyLite", "GFB60"}}},
        {"ASA", {{"Bambu", "GFB01"}, {"PolyLite", "GFB61"}}},
        {"PC", {{"Bambu", "GFC00"}}},
        {"PA-CF", {{"Bambu", "GFN03"}}},
        {"PET-CF", {{"Bambu", "GFT00"}}},
        {"PETG HF", {{"Bambu", "GFG02"}}},
        {"PETG Translucent", {{"Bambu", "GFG01"}}},
        {"PETG", {{"Bambu", "GFG00"}, {"PolyLite", "GFG60"}, {"Sunlu", "GFSNL08"}}}};

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

    // Builds the Bambu `ams_filament_setting` an MQTT payload from an OpenSpool tag
    inline std::string generate_mqtt_payload(const openspool::Tag &tag, uint16_t ams_id, uint16_t ams_tray)
    {
        JsonDocument doc_out;
        JsonObject print = doc_out["print"].to<JsonObject>();
        print["sequence_id"] = "0";
        print["command"] = "ams_filament_setting";
        print["ams_id"] = ams_id;
        print["tray_id"] = ams_tray;
        if (tag.color_hex.length() == 6)
        {
            print["tray_color"] = tag.color_hex + "FF";
        }
        else
        {
            print["tray_color"] = tag.color_hex;
        }
        print["nozzle_temp_min"] = tag.min_temp;
        print["nozzle_temp_max"] = tag.max_temp;
        print["tray_type"] = tag.type;
        print["setting_id"] = "";
        print["tray_info_idx"] = get_bambu_code(tag.type, tag.brand);
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

    // Builds the Bambu `ams_filament_setting` MQTT payload from an OpenTag3D tag
    inline std::string generate_mqtt_payload(const opentag3d::Tag &tag, uint16_t ams_id, uint16_t ams_tray)
    {
        JsonDocument doc_out;
        JsonObject print = doc_out["print"].to<JsonObject>();
        print["sequence_id"] = "0";
        print["command"] = "ams_filament_setting";
        print["ams_id"] = ams_id;
        print["tray_id"] = ams_tray;

        char color_hex[9];
        snprintf(color_hex, sizeof(color_hex), "%02X%02X%02X%02X", tag.color.r, tag.color.g, tag.color.b,
                 tag.color.a);
        print["tray_color"] = color_hex;

        print["nozzle_temp_min"] = tag.min_print_temp ? tag.min_print_temp : tag.print_temp;
        print["nozzle_temp_max"] = tag.max_print_temp ? tag.max_print_temp : tag.print_temp;

        const std::string type = opentag3d::format_type(tag);
        print["tray_type"] = type;
        print["setting_id"] = "";
        print["tray_info_idx"] = get_bambu_code(type, tag.manufacturer);

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

namespace rfid
{
    struct TagResult
    {
        bool is_valid{false};
        std::string display_payload;
        std::string mqtt_payload;
    };

    inline TagResult process_tag(esphome::nfc::NfcTag &tag, uint16_t ams_id, uint16_t ams_tray)
    {
        TagResult result;
        if (!tag.has_ndef_message())
        {
            ESP_LOGI("NFC", "Tag found without NDEF message");
            return result;
        }

        const auto &records = tag.get_ndef_message()->get_records();
        int json_index = -1;
        int opentag3d_index = -1;
        for (size_t i = 0; i < records.size(); i++)
        {
            const auto &type = records[i]->get_type();
            if (type == "application/json")
            {
                if (json_index < 0)
                {
                    ESP_LOGI("NFC", "OpenSpool record detected");
                    json_index = i;
                }
                else
                {
                    ESP_LOGW("NFC", "Multiple OpenSpool records detected, using first one");
                }
            }
            else if (type == "application/opentag3d")
            {
                if (opentag3d_index < 0)
                {
                    ESP_LOGI("NFC", "OpenTag3D record detected");
                    opentag3d_index = i;
                }
                else
                {
                    ESP_LOGW("NFC", "Multiple OpenTag3D records detected, using first one");
                }
            }
        }

        // Parse OpenSpool
        if (json_index >= 0)
        {
            result.display_payload = records[json_index]->get_payload();
            ESP_LOGD("NFC", "Payload: %s", result.display_payload.c_str());

            openspool::Tag openspool_tag;
            if (openspool::decode(result.display_payload, openspool_tag))
            {
                result.is_valid = true;
                result.mqtt_payload = bambulabs::generate_mqtt_payload(openspool_tag, ams_id, ams_tray);

                return result;
            }
            else
            {
                ESP_LOGE("NFC", "Failed to decode OpenSpool payload");
            }
        }
        
        // Parse OpenTag3D
        if (opentag3d_index >= 0)
        {
            const auto &record_payload = records[opentag3d_index]->get_payload();
            result.display_payload = opentag3d::to_hex_string(record_payload);
            ESP_LOGD("NFC", "Payload: %s", result.display_payload.c_str());

            opentag3d::Tag opentag3d_tag;
            if (opentag3d::decode(record_payload, opentag3d_tag))
            {
                result.is_valid = true;
                result.mqtt_payload = bambulabs::generate_mqtt_payload(opentag3d_tag, ams_id, ams_tray);
                ESP_LOGI("NFC", "Decoded OpenTag3D tag: %s from %s",
                         opentag3d::format_type(opentag3d_tag).c_str(), opentag3d_tag.manufacturer.c_str());
                return result;
            }
            else
            {
                ESP_LOGE("NFC", "Failed to decode OpenTag3D payload");
            }
        }

        // No tag could be parsed
        ESP_LOGW("NFC", "No recognized NDEF record found");
        return result;
    }
}