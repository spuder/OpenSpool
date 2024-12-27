#pragma once

#include "nfc_tag.h"

namespace esphome {
namespace nfc {

static const char HA_TAG_ID_EXT_RECORD_TYPE[] = "android.com:pkg";
static const char HA_TAG_ID_EXT_RECORD_PAYLOAD[] = "io.homeassistant.companion.android";
static const char HA_TAG_ID_PREFIX[] = "https://www.home-assistant.io/tag/";
static const std::vector<uint8_t> BAMBU_SALT = {
  0x9a, 0x75, 0x9c, 0xf2, 0xc4, 0xf7, 0xca, 0xff,
  0x22, 0x2c, 0xb9, 0x76, 0x9b, 0x41, 0xbc, 0x96
};

std::string get_ha_tag_ndef(NfcTag &tag);
std::string get_random_ha_tag_ndef();
bool has_ha_tag_ndef(NfcTag &tag);
std::array<std::array<uint8_t, 6>, 16>  generate_keys(const std::vector<uint8_t> &uid);

}  // namespace nfc
}  // namespace esphome
