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

// #include <mbedtls/hkdf.h>
// #include <mbedtls/md.h>
// #include <limits.h>

//#include <limits.h>

//#include "esp_system.h"
//#include "esp_hmac.h"

//#define TAG "RFID_KEYS"


namespace bambulabs
{
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
        if (!doc_in.containsKey("version") || std::string(doc_in["version"]) != "1.0")
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

        // TODO: verify color_hex is 6 digit code, not 8 digit

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

    //TODO: refactor so generate_keys takes a string (uid) as a parameter
    // esphome's nfc object creates tags with a string in this format: 'aa-bb-cc-dd' or 'aa-bb-cc-dd-ee-ff-gg'
    inline std::vector<int> generate_keys() {
        uint8_t uid[] = {0x5a, 0xc9, 0x00, 0xa6};
        // 5a-c9-00-a6 should produce the following 16 keys
        // 63e5af2c1d75
        // 40d146ce6e01
        // 6a66957dcc91
        // 15e7041f68d9
        // 7ee1ac7fa75f
        // 55cbbad18673
        // ce5901af9416
        // a223a193e6a3
        // 24f4d022f402
        // 7df999dd836b
        // b0dac4a48903
        // b026ab566f11
        // 8b495d5a0b44
        // 7ebef1cb3e94
        // 4685790c6e01
        // 3f00144c7b4a
        size_t uid_len = sizeof(uid);
    
        // Master key
        uint8_t master[] = {
            0x9a, 0x75, 0x9c, 0xf2, 0xc4, 0xf7, 0xca, 0xff,
            0x22, 0x2c, 0xb9, 0x76, 0x9b, 0x41, 0xbc, 0x96
        };
        size_t master_len = sizeof(master);
    
        // Output buffer
        uint8_t output[96];
    
        // Context
        const unsigned char context[] = {'R', 'F', 'I', 'D', '-', 'A', '\0'};
        size_t context_len = sizeof(context);

        // Perform HKDF
        // https://mbed-tls.readthedocs.io/projects/api/en/development/api/file/hkdf_8h/
        mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                    master, master_len,  // Use master as salt
                    uid, uid_len,        // Use UID as IKM
                    context, context_len,
                    output, sizeof(output));

        std::vector<int> result;
        for (int i = 0; i < 16; i++) {
            ESP_LOGD("bambu", "Key %d: %02x%02x%02x%02x%02x%02x", i,
                    output[i*6], output[i*6+1], output[i*6+2],
                    output[i*6+3], output[i*6+4], output[i*6+5]);
            for (int j = 0; j < 6; j++) {
                result.push_back(output[i*6 + j]);
            }
        }
        return result;
    }

}
