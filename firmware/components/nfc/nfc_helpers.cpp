#include "nfc_helpers.h"
#include "mbedtls/hkdf.h"
#include "mbedtls/md.h"


namespace esphome {
namespace nfc {

static const char *const TAG = "nfc.helpers";

bool has_ha_tag_ndef(NfcTag &tag) { return !get_ha_tag_ndef(tag).empty(); }

std::string get_ha_tag_ndef(NfcTag &tag) {
  if (!tag.has_ndef_message()) {
    return std::string();
  }
  auto message = tag.get_ndef_message();
  auto records = message->get_records();
  for (const auto &record : records) {
    std::string payload = record->get_payload();
    size_t pos = payload.find(HA_TAG_ID_PREFIX);
    if (pos != std::string::npos) {
      return payload.substr(pos + sizeof(HA_TAG_ID_PREFIX) - 1);
    }
  }
  return std::string();
}

std::string get_random_ha_tag_ndef() {
  static const char ALPHANUM[] = "0123456789abcdef";
  std::string uri = HA_TAG_ID_PREFIX;
  for (int i = 0; i < 8; i++) {
    uri += ALPHANUM[random_uint32() % (sizeof(ALPHANUM) - 1)];
  }
  uri += "-";
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 4; i++) {
      uri += ALPHANUM[random_uint32() % (sizeof(ALPHANUM) - 1)];
    }
    uri += "-";
  }
  for (int i = 0; i < 12; i++) {
    uri += ALPHANUM[random_uint32() % (sizeof(ALPHANUM) - 1)];
  }
  ESP_LOGD("pn7160", "Payload to be written: %s", uri.c_str());
  return uri;
}

// TODO: should this use unique_ptr for better memory management?
std::array<std::array<uint8_t, 6>, 16> generate_keys(const std::vector<uint8_t>& uid) {
    
    // Output buffer
    static uint8_t output[96];
    
    // Context
    const unsigned char context[] = {'R', 'F', 'I', 'D', '-', 'A', '\0'};
    
    // Perform HKDF
    mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
        nfc::BAMBU_SALT.data(), nfc::BAMBU_SALT.size(),
        uid.data(), uid.size(),
        context, sizeof(context),
        output, sizeof(output));
    
    //TODO: should this use std::unique_ptr for better memory management? 
    std::array<std::array<uint8_t, 6>, 16> result;
    for (int i = 0; i < 16; i++) {
        std::copy_n(output + i*6, 6, result[i].begin());
        ESP_LOGD("bambu", "Key %d: %02x%02x%02x%02x%02x%02x", i,
            result[i][0], result[i][1], result[i][2],
            result[i][3], result[i][4], result[i][5]);
    }
    return result;
}

}  // namespace nfc
}  // namespace esphome
