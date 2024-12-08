#pragma once

#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "ndef_record.h"
#include "ndef_message.h"
#include "nfc_tag.h"

#include <vector>

namespace esphome {
namespace nfc {

static const uint8_t MIFARE_CLASSIC_BLOCK_SIZE = 16;
static const uint8_t MIFARE_CLASSIC_LONG_TLV_SIZE = 4;
static const uint8_t MIFARE_CLASSIC_SHORT_TLV_SIZE = 2;
static const uint8_t MIFARE_CLASSIC_BLOCKS_PER_SECT_LOW = 4;
static const uint8_t MIFARE_CLASSIC_BLOCKS_PER_SECT_HIGH = 16;
static const uint8_t MIFARE_CLASSIC_16BLOCK_SECT_START = 32;

static const uint8_t MIFARE_ULTRALIGHT_PAGE_SIZE = 4;
static const uint8_t MIFARE_ULTRALIGHT_READ_SIZE = 4;
static const uint8_t MIFARE_ULTRALIGHT_DATA_START_PAGE = 4;
static const uint8_t MIFARE_ULTRALIGHT_MAX_PAGE = 63;

static const uint8_t TAG_TYPE_MIFARE_CLASSIC = 0;
static const uint8_t TAG_TYPE_1 = 1;
static const uint8_t TAG_TYPE_2 = 2;
static const uint8_t TAG_TYPE_3 = 3;
static const uint8_t TAG_TYPE_4 = 4;
static const uint8_t TAG_TYPE_UNKNOWN = 99;

// Mifare Commands
static const uint8_t MIFARE_CMD_AUTH_A = 0x60;
static const uint8_t MIFARE_CMD_AUTH_B = 0x61;
static const uint8_t MIFARE_CMD_HALT = 0x50;
static const uint8_t MIFARE_CMD_READ = 0x30;
static const uint8_t MIFARE_CMD_WRITE = 0xA0;
static const uint8_t MIFARE_CMD_WRITE_ULTRALIGHT = 0xA2;

// Mifare Ack/Nak
static const uint8_t MIFARE_CMD_ACK = 0x0A;
static const uint8_t MIFARE_CMD_NAK_INVALID_XFER_BUFF_VALID = 0x00;
static const uint8_t MIFARE_CMD_NAK_CRC_ERROR_XFER_BUFF_VALID = 0x01;
static const uint8_t MIFARE_CMD_NAK_INVALID_XFER_BUFF_INVALID = 0x04;
static const uint8_t MIFARE_CMD_NAK_CRC_ERROR_XFER_BUFF_INVALID = 0x05;

static const char *const MIFARE_CLASSIC = "Mifare Classic";
static const char *const NFC_FORUM_TYPE_2 = "NFC Forum Type 2";
static const char *const ERROR = "Error";

static const uint8_t DEFAULT_KEY[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static const uint8_t NDEF_KEY[6] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};
static const uint8_t MAD_KEY[6] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5};
// static const uint8_t BAMBU_SALT[16] = {0x9a, 0x75, 0x9c, 0xf2, 0xc4, 0xf7, 0xca, 0xff,0x22, 0x2c, 0xb9, 0x76, 0x9b, 0x41, 0xbc, 0x96};
// static const uint8_t KEY0[6] = {0x63, 0xe5, 0xaf, 0x2c, 0x1d, 0x75};
// static const uint8_t KEY1[6] = {0x40, 0xd1, 0x46, 0xce, 0x6e, 0x01};
// static const uint8_t KEY2[6] = {0x6a, 0x66, 0x95, 0x7d, 0xcc, 0x91};
// static const uint8_t KEY3[6] = {0x15, 0xe7, 0x04, 0x1f, 0x68, 0xd9};
// static const uint8_t KEY4[6] = {0x7e, 0xe1, 0xac, 0x7f, 0xa7, 0x5f};
// static const uint8_t KEY5[6] = {0x55, 0xcb, 0xba, 0xd1, 0x86, 0x73};
// static const uint8_t KEY6[6] = {0xce, 0x59, 0x01, 0xaf, 0x94, 0x16};
// static const uint8_t KEY7[6] = {0xa2, 0x23, 0xa1, 0x93, 0xe6, 0xa3};
// static const uint8_t KEY8[6] = {0x24, 0xf4, 0xd0, 0x22, 0xf4, 0x02};
// static const uint8_t KEY9[6] = {0x7d, 0xf9, 0x99, 0xdd, 0x83, 0x6b};
// static const uint8_t KEY10[6] = {0xb0, 0xda, 0xc4, 0xa4, 0x89, 0x03};
// static const uint8_t KEY11[6] = {0xb0, 0x26, 0xab, 0x56, 0x6f, 0x11};
// static const uint8_t KEY12[6] = {0x8b, 0x49, 0x5d, 0x5a, 0x0b, 0x44};
// static const uint8_t KEY13[6] = {0x7e, 0xbe, 0xf1, 0xcb, 0x3e, 0x94};
// static const uint8_t KEY14[6] = {0x46, 0x85, 0x79, 0x0c, 0x6e, 0x01};
// static const uint8_t KEY15[6] = {0x3f, 0x00, 0x14, 0x4c, 0x7b, 0x4a};
// static const std::array<const uint8_t*, 16> KEYS = {{
//     KEY0, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, 
//     KEY8, KEY9, KEY10, KEY11, KEY12, KEY13, KEY14, KEY15
// }};


std::string format_uid(std::vector<uint8_t> &uid);
std::string format_bytes(std::vector<uint8_t> &bytes);

uint8_t guess_tag_type(uint8_t uid_length);
uint8_t get_mifare_classic_ndef_start_index(std::vector<uint8_t> &data);
bool decode_mifare_classic_tlv(std::vector<uint8_t> &data, uint32_t &message_length, uint8_t &message_start_index);
uint32_t get_mifare_classic_buffer_size(uint32_t message_length);

bool mifare_classic_is_first_block(uint8_t block_num);
bool mifare_classic_is_trailer_block(uint8_t block_num);

uint32_t get_mifare_ultralight_buffer_size(uint32_t message_length);

class NfcTagListener {
 public:
  virtual void tag_off(NfcTag &tag) {}
  virtual void tag_on(NfcTag &tag) {}
};

class Nfcc {
 public:
  void register_listener(NfcTagListener *listener) { this->tag_listeners_.push_back(listener); }

 protected:
  std::vector<NfcTagListener *> tag_listeners_;
};

}  // namespace nfc
}  // namespace esphome
