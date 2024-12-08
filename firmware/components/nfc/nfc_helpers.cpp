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

//TODO: should generate_keys() use a vector? 
std::vector<std::vector<uint8_t>> generate_keys(std::vector<uint8_t>& uid) {
    // std::vector<uint8_t> uid = {0x5A, 0xC9, 0x00, 0xA6};
    std::vector<uint8_t> master = {
        0x9a, 0x75, 0x9c, 0xf2, 0xc4, 0xf7, 0xca, 0xff,
        0x22, 0x2c, 0xb9, 0x76, 0x9b, 0x41, 0xbc, 0x96
    };
    size_t master_len = master.size();
    
    // Output buffer
    uint8_t output[96];
    
    // Context
    const unsigned char context[] = {'R', 'F', 'I', 'D', '-', 'A', '\0'};
    size_t context_len = sizeof(context);
    
    // Perform HKDF
    mbedtls_hkdf(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
        master.data(), master_len, // Use master as salt
        uid.data(), uid.size(),
        context, context_len,
        output, sizeof(output));
    
    std::vector<std::vector<uint8_t>> result;
    for (int i = 0; i < 16; i++) {
        ESP_LOGD("bambu", "Key %d: %02x%02x%02x%02x%02x%02x", i,
            output[i*6], output[i*6+1], output[i*6+2],
            output[i*6+3], output[i*6+4], output[i*6+5]);
        
        // Create a vector for each 6-byte key
        std::vector<uint8_t> key(output + i*6, output + i*6 + 6);
        result.push_back(key);
    }
    return result;
}

//TODO: take uid as parameter
//TODO: consider vector instead of array
//TODO: support more than 16 keys
// std::array<const uint8_t*, 16> generate_keys() {
//   //TODO: remove all these hard coded keys
//   static const uint8_t KEY0[6] = {0x63, 0xe5, 0xaf, 0x2c, 0x1d, 0x75};
//   static const uint8_t KEY1[6] = {0x40, 0xd1, 0x46, 0xce, 0x6e, 0x01};
//   static const uint8_t KEY2[6] = {0x6a, 0x66, 0x95, 0x7d, 0xcc, 0x91};
//   static const uint8_t KEY3[6] = {0x15, 0xe7, 0x04, 0x1f, 0x68, 0xd9};
//   static const uint8_t KEY4[6] = {0x7e, 0xe1, 0xac, 0x7f, 0xa7, 0x5f};
//   static const uint8_t KEY5[6] = {0x55, 0xcb, 0xba, 0xd1, 0x86, 0x73};
//   static const uint8_t KEY6[6] = {0xce, 0x59, 0x01, 0xaf, 0x94, 0x16};
//   static const uint8_t KEY7[6] = {0xa2, 0x23, 0xa1, 0x93, 0xe6, 0xa3};
//   static const uint8_t KEY8[6] = {0x24, 0xf4, 0xd0, 0x22, 0xf4, 0x02};
//   static const uint8_t KEY9[6] = {0x7d, 0xf9, 0x99, 0xdd, 0x83, 0x6b};
//   static const uint8_t KEY10[6] = {0xb0, 0xda, 0xc4, 0xa4, 0x89, 0x03};
//   static const uint8_t KEY11[6] = {0xb0, 0x26, 0xab, 0x56, 0x6f, 0x11};
//   static const uint8_t KEY12[6] = {0x8b, 0x49, 0x5d, 0x5a, 0x0b, 0x44};
//   static const uint8_t KEY13[6] = {0x7e, 0xbe, 0xf1, 0xcb, 0x3e, 0x94};
//   static const uint8_t KEY14[6] = {0x46, 0x85, 0x79, 0x0c, 0x6e, 0x01};
//   static const uint8_t KEY15[6] = {0x3f, 0x00, 0x14, 0x4c, 0x7b, 0x4a};
//   static const std::array<const uint8_t*, 16> KEYS = {{
//       KEY0, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, 
//       KEY8, KEY9, KEY10, KEY11, KEY12, KEY13, KEY14, KEY15
//   }};
//   return KEYS;
//   // std::vector<int> keys = {};
//   // keys.push_back(KEY0);
//   // keys.push_back(KEY1);
//   // keys.push_back(KEY2);
//   // keys.push_back(KEY3);
//   // keys.push_back(KEY4);
//   // keys.push_back(KEY5);
//   // keys.push_back(KEY6);
//   // keys.push_back(KEY7);
//   // keys.push_back(KEY8);
//   // keys.push_back(KEY9);
//   // keys.push_back(KEY10);
//   // keys.push_back(KEY11);
//   // keys.push_back(KEY12);
//   // keys.push_back(KEY13);
//   // keys.push_back(KEY14);
//   // keys.push_back(KEY15);
//   // return keys;
// }


}  // namespace nfc
}  // namespace esphome
