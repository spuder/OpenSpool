// filament_mappings.h

#pragma once

#include <map>
#include <string>

std::map<std::string, std::string> filament_mappings = {
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
    {"Support", "GFS00"}
};

// Special cases for brand-specific codes
std::map<std::string, std::map<std::string, std::string>> brand_specific_codes = {
    {"PLA", {
        {"Bambu", "GFA00"},
        {"PolyTerra", "GFL01"},
        {"PolyLite", "GFL00"}
    }},
    {"TPU", {
        {"Bambu", "GFU01"}
    }},
    {"ABS", {
        {"Bambu", "GFB00"}
    }},
    {"PC", {
        {"Bambu", "GFC00"}
    }},
    {"PA-CF", {
        {"Bambu", "GFN03"}
    }},
    {"PET-CF", {
        {"Bambu", "GFT00"}
    }}
};

// Function with two parameters
std::string get_bambu_code(const std::string& type, const std::string& brand = "") {
    if (!brand.empty()) {
        auto brand_it = brand_specific_codes.find(type);
        if (brand_it != brand_specific_codes.end()) {
            auto code_it = brand_it->second.find(brand);
            if (code_it != brand_it->second.end()) {
                return code_it->second;
            }
        }
    }
    
    auto it = filament_mappings.find(type);
    if (it != filament_mappings.end()) {
        return it->second;
    }
    return "";  // Unknown type
}

// Function with three parameters (for Bambu PLA subtypes)
std::string get_bambu_code(const std::string& type, const std::string& brand, const std::string& subtype) {
    if (type == "PLA" && brand == "Bambu") {
        if (subtype == "Matte") return "GFA01";
        if (subtype == "Metal") return "GFA02";
        if (subtype == "Impact") return "GFA03";
        return "GFA00";  // Default to Basic for unknown subtypes
    }
    return get_bambu_code(type, brand);
}