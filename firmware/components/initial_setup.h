#pragma once

#include "esphome.h"

// Add any additional helper functions here if needed
bool is_valid_ip(const char* ip_str) {
    if (ip_str == nullptr) return false;
    
    int segments = 0;
    int curr_num = 0;
    int len = strlen(ip_str);
    
    for (int i = 0; i < len; i++) {
        if (ip_str[i] == '.') {
            if (curr_num > 255) return false;
            segments++;
            curr_num = 0;
        } else if (ip_str[i] >= '0' && ip_str[i] <= '9') {
            curr_num = curr_num * 10 + (ip_str[i] - '0');
            if (curr_num > 255) return false;
        } else {
            return false;
        }
    }
    
    // Check final segment
    if (curr_num > 255) return false;
    
    // Valid IP has exactly 3 dots (4 segments)
    return segments == 3;
}