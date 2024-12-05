#include <memory>

#include "pn532.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pn532 {

static const char *const TAG = "pn532.mifare_classic";

//TODO: remove backdoor key and these hardcoded uids
static const uint8_t BACKDOOR_KEY[6] = {0xA3,0x96,0xEF,0xA4,0xE2,0x4F};
static const uint8_t KEY0[6] = {0x63, 0xe5, 0xaf, 0x2c, 0x1d, 0x75};
static const uint8_t KEY1[6] = {0x40, 0xd1, 0x46, 0xce, 0x6e, 0x01};
static const uint8_t KEY2[6] = {0x6a, 0x66, 0x95, 0x7d, 0xcc, 0x91};
static const uint8_t KEY3[6] = {0x15, 0xe7, 0x04, 0x1f, 0x68, 0xd9};
static const uint8_t KEY4[6] = {0x7e, 0xe1, 0xac, 0x7f, 0xa7, 0x5f};
static const uint8_t KEY5[6] = {0x55, 0xcb, 0xba, 0xd1, 0x86, 0x73};
static const uint8_t KEY6[6] = {0xce, 0x59, 0x01, 0xaf, 0x94, 0x16};
static const uint8_t KEY7[6] = {0xa2, 0x23, 0xa1, 0x93, 0xe6, 0xa3};
static const uint8_t KEY8[6] = {0x24, 0xf4, 0xd0, 0x22, 0xf4, 0x02};
static const uint8_t KEY9[6] = {0x7d, 0xf9, 0x99, 0xdd, 0x83, 0x6b};
static const uint8_t KEY10[6] = {0xb0, 0xda, 0xc4, 0xa4, 0x89, 0x03};
static const uint8_t KEY11[6] = {0xb0, 0x26, 0xab, 0x56, 0x6f, 0x11};
static const uint8_t KEY12[6] = {0x8b, 0x49, 0x5d, 0x5a, 0x0b, 0x44};
static const uint8_t KEY13[6] = {0x7e, 0xbe, 0xf1, 0xcb, 0x3e, 0x94};
static const uint8_t KEY14[6] = {0x46, 0x85, 0x79, 0x0c, 0x6e, 0x01};
static const uint8_t KEY15[6] = {0x3f, 0x00, 0x14, 0x4c, 0x7b, 0x4a};
static const std::array<const uint8_t*, 16> KEYS = {{
    KEY0, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, 
    KEY8, KEY9, KEY10, KEY11, KEY12, KEY13, KEY14, KEY15
}};

//TODO: refactor read_mifare_classic_tag_ so that it takes in an array of keys
//For bambu tags this will always be 16 keys, however for mifare classic 4k it might be different
//Ideally this should not break the existing mifare classic ndef parsing so this could be contributed back upstream to esphome
std::unique_ptr<nfc::NfcTag> PN532::read_mifare_classic_tag_(std::vector<uint8_t> &uid) {
  uint8_t current_block = 4;
  uint8_t message_start_index = 0;
  uint32_t message_length = 0;
  bool is_ndef = false; //TOOD: there might be a more native way to check if ndef


  // Note: If the first auth fails, you can't immediatly try and auth again
  // https://community.nxp.com/t5/NFC/NXP-NFC-Reader-Library-Authentication-with-two-three-possible-A/td-p/1377179
  // Most likely this will require implmenting `PICC_CMD_WUPA = 0x52,` 
  // if (this->auth_mifare_classic_block_(uid, current_block, nfc::MIFARE_CMD_AUTH_A, nfc::NDEF_KEY)) {
  //   std::vector<uint8_t> data;
  //   if (this->read_mifare_classic_block_(current_block, data)) {
  //     if (!nfc::decode_mifare_classic_tlv(data, message_length, message_start_index)) {
  //       return make_unique<nfc::NfcTag>(uid, nfc::ERROR);
  //     }
  //   } else {
  //     ESP_LOGE(TAG, "Failed to read block %d", current_block);
  //     return make_unique<nfc::NfcTag>(uid, nfc::MIFARE_CLASSIC);
  //   }
  // } else {
  //   ESP_LOGV(TAG, "Tag is not NDEF formatted");
  //   return make_unique<nfc::NfcTag>(uid, nfc::MIFARE_CLASSIC);
  // }

  auto trailer_block_number = (current_block / 4) * 4 + 3;
  uint8_t sector_first_block = (current_block / 4) * 4;

// This code works `this->auth_mifare_classic_block_(uid, current_block, 0x60, KEY0)`
// This code fails `this->auth_mifare_classic_block_(uid, current_block, 0x64, BACKDOOR_KEY)`
// Both using current_block for auth (block 0)

  ESP_LOGD(TAG, "Authenticating block %d using %d", current_block, 0x64);
  if (this->auth_mifare_classic_block_(uid, current_block, 0x64, BACKDOOR_KEY)) {
    ESP_LOGD(TAG, "Block %d authenticated", sector_first_block);
    std::vector<uint8_t> data;
  } else {
    ESP_LOGE(TAG, "Error, Block authentication failed for 1 %d", current_block);
  }


  uint32_t index = 0;
  uint32_t buffer_size = nfc::get_mifare_classic_buffer_size(message_length);
  std::vector<uint8_t> buffer;

  while (index < buffer_size) {
    if (nfc::mifare_classic_is_first_block(current_block)) {
      auto current_sector = current_block / 4; 
      auto trailer_block_number = (current_block / 4) * 4 + 3;
      auto sector_first_block = (current_block / 4) * 4;
      if (!this->auth_mifare_classic_block_(uid, current_block, 0x64, BACKDOOR_KEY)) {
        ESP_LOGE(TAG, "Error, Block authentication failed for %d", current_block);
      } else {
        ESP_LOGD(TAG, "Block %d authenticated", current_block);
      }

    }
    std::vector<uint8_t> block_data;
    if (this->read_mifare_classic_block_(current_block, block_data)) {
      buffer.insert(buffer.end(), block_data.begin(), block_data.end());
    } else {
      ESP_LOGE(TAG, "Error reading block %d", current_block);
    }

    index += nfc::MIFARE_CLASSIC_BLOCK_SIZE;
    current_block++;

    if (nfc::mifare_classic_is_trailer_block(current_block)) {
      current_block++;
    }
  }

  if (buffer.begin() + message_start_index < buffer.end()) {
    buffer.erase(buffer.begin(), buffer.begin() + message_start_index);
  } else {
    return make_unique<nfc::NfcTag>(uid, nfc::MIFARE_CLASSIC);
  }

  return make_unique<nfc::NfcTag>(uid, nfc::MIFARE_CLASSIC, buffer);
}

bool PN532::read_mifare_classic_block_(uint8_t block_num, std::vector<uint8_t> &data) {
  if (!this->write_command_({
          PN532_COMMAND_INDATAEXCHANGE,
          0x01,  // One card
          nfc::MIFARE_CMD_READ,
          block_num,
      })) {
    return false;
  }

  if (!this->read_response(PN532_COMMAND_INDATAEXCHANGE, data) || data[0] != 0x00) {
    return false;
  }
  data.erase(data.begin());

  ESP_LOGVV(TAG, " Block %d: %s", block_num, nfc::format_bytes(data).c_str());
  return true;
}

bool PN532::auth_mifare_classic_block_(std::vector<uint8_t> &uid, uint8_t block_num, uint8_t key_num,
                                       const uint8_t *key) {
  std::vector<uint8_t> data({
      PN532_COMMAND_INDATAEXCHANGE,
      0x01,       // One card
      key_num,    // Mifare Key slot
      block_num,  // Block number
  });
  data.insert(data.end(), key, key + 6);
  data.insert(data.end(), uid.begin(), uid.end());
  if (!this->write_command_(data)) {
    ESP_LOGE(TAG, "Authentication failed - Block %d", block_num);
    return false;
  }

  std::vector<uint8_t> response;
  if (!this->read_response(PN532_COMMAND_INDATAEXCHANGE, response) || response[0] != 0x00) {
    ESP_LOGE(TAG, "Authentication failed - Block 0x%02x", block_num);
    return false;
  }

  return true;
}

bool PN532::format_mifare_classic_mifare_(std::vector<uint8_t> &uid) {
  std::vector<uint8_t> blank_buffer(
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  std::vector<uint8_t> trailer_buffer(
      {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF});

  bool error = false;

  for (int block = 0; block < 64; block += 4) {
    if (!this->auth_mifare_classic_block_(uid, block + 3, nfc::MIFARE_CMD_AUTH_B, nfc::DEFAULT_KEY)) {
      continue;
    }
    if (block != 0) {
      if (!this->write_mifare_classic_block_(block, blank_buffer)) {
        ESP_LOGE(TAG, "Unable to write block %d", block);
        error = true;
      }
    }
    if (!this->write_mifare_classic_block_(block + 1, blank_buffer)) {
      ESP_LOGE(TAG, "Unable to write block %d", block + 1);
      error = true;
    }
    if (!this->write_mifare_classic_block_(block + 2, blank_buffer)) {
      ESP_LOGE(TAG, "Unable to write block %d", block + 2);
      error = true;
    }
    if (!this->write_mifare_classic_block_(block + 3, trailer_buffer)) {
      ESP_LOGE(TAG, "Unable to write block %d", block + 3);
      error = true;
    }
  }

  return !error;
}

bool PN532::format_mifare_classic_ndef_(std::vector<uint8_t> &uid) {
  std::vector<uint8_t> empty_ndef_message(
      {0x03, 0x03, 0xD0, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  std::vector<uint8_t> blank_block(
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  std::vector<uint8_t> block_1_data(
      {0x14, 0x01, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1});
  std::vector<uint8_t> block_2_data(
      {0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1});
  std::vector<uint8_t> block_3_trailer(
      {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x78, 0x77, 0x88, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF});
  std::vector<uint8_t> ndef_trailer(
      {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 0x07, 0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF});

  if (!this->auth_mifare_classic_block_(uid, 0, nfc::MIFARE_CMD_AUTH_B, nfc::DEFAULT_KEY)) {
    ESP_LOGE(TAG, "Unable to authenticate block 0 for formatting!");
    return false;
  }
  if (!this->write_mifare_classic_block_(1, block_1_data))
    return false;
  if (!this->write_mifare_classic_block_(2, block_2_data))
    return false;
  if (!this->write_mifare_classic_block_(3, block_3_trailer))
    return false;

  ESP_LOGD(TAG, "Sector 0 formatted to NDEF");

  for (int block = 4; block < 64; block += 4) {
    if (!this->auth_mifare_classic_block_(uid, block + 3, nfc::MIFARE_CMD_AUTH_B, nfc::DEFAULT_KEY)) {
      return false;
    }
    if (block == 4) {
      if (!this->write_mifare_classic_block_(block, empty_ndef_message)) {
        ESP_LOGE(TAG, "Unable to write block %d", block);
      }
    } else {
      if (!this->write_mifare_classic_block_(block, blank_block)) {
        ESP_LOGE(TAG, "Unable to write block %d", block);
      }
    }
    if (!this->write_mifare_classic_block_(block + 1, blank_block)) {
      ESP_LOGE(TAG, "Unable to write block %d", block + 1);
    }
    if (!this->write_mifare_classic_block_(block + 2, blank_block)) {
      ESP_LOGE(TAG, "Unable to write block %d", block + 2);
    }
    if (!this->write_mifare_classic_block_(block + 3, ndef_trailer)) {
      ESP_LOGE(TAG, "Unable to write trailer block %d", block + 3);
    }
  }
  return true;
}

bool PN532::write_mifare_classic_block_(uint8_t block_num, std::vector<uint8_t> &write_data) {
  std::vector<uint8_t> data({
      PN532_COMMAND_INDATAEXCHANGE,
      0x01,  // One card
      nfc::MIFARE_CMD_WRITE,
      block_num,
  });
  data.insert(data.end(), write_data.begin(), write_data.end());
  if (!this->write_command_(data)) {
    ESP_LOGE(TAG, "Error writing block %d", block_num);
    return false;
  }

  std::vector<uint8_t> response;
  if (!this->read_response(PN532_COMMAND_INDATAEXCHANGE, response)) {
    ESP_LOGE(TAG, "Error writing block %d", block_num);
    return false;
  }

  return true;
}

bool PN532::write_mifare_classic_tag_(std::vector<uint8_t> &uid, nfc::NdefMessage *message) {
  auto encoded = message->encode();

  uint32_t message_length = encoded.size();
  uint32_t buffer_length = nfc::get_mifare_classic_buffer_size(message_length);

  encoded.insert(encoded.begin(), 0x03);
  if (message_length < 255) {
    encoded.insert(encoded.begin() + 1, message_length);
  } else {
    encoded.insert(encoded.begin() + 1, 0xFF);
    encoded.insert(encoded.begin() + 2, (message_length >> 8) & 0xFF);
    encoded.insert(encoded.begin() + 3, message_length & 0xFF);
  }
  encoded.push_back(0xFE);

  encoded.resize(buffer_length, 0);

  uint32_t index = 0;
  uint8_t current_block = 4;

  while (index < buffer_length) {
    if (nfc::mifare_classic_is_first_block(current_block)) {
      if (!this->auth_mifare_classic_block_(uid, current_block, nfc::MIFARE_CMD_AUTH_A, nfc::NDEF_KEY)) {
        return false;
      }
    }

    std::vector<uint8_t> data(encoded.begin() + index, encoded.begin() + index + nfc::MIFARE_CLASSIC_BLOCK_SIZE);
    if (!this->write_mifare_classic_block_(current_block, data)) {
      return false;
    }
    index += nfc::MIFARE_CLASSIC_BLOCK_SIZE;
    current_block++;

    if (nfc::mifare_classic_is_trailer_block(current_block)) {
      // Skipping as cannot write to trailer
      current_block++;
    }
  }
  return true;
}

}  // namespace pn532
}  // namespace esphome
