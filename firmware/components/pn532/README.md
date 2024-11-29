# pn532

This is an external component, which is a fork of the pn532 component in esphome

Its goal is to provide the ability to read mifare classic tags which use 2 sets of keys

The keys are derived using a KDF with a discovered salt

Here is a jupiter notebook to generate keys. Keys are derived from the UID of the tag
https://colab.research.google.com/drive/1EUgIYV4-bZEq6-YeYGrTfTZLhPcWnuno#scrollTo=77LulMfgYg7x


## Hurdles

The tag already has a mechanism to use a key to talk to the tag. [The key is hard coded in `nfc::NDEF_KEY`](https://github.com/esphome/esphome/blob/dev/esphome/components/nfc/nfc.h#L53) 

```cpp
if (!this->auth_mifare_classic_block_(uid, current_block, nfc::MIFARE_CMD_AUTH_A, nfc::NDEF_KEY)) {
```

```cpp
static const uint8_t DEFAULT_KEY[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static const uint8_t NDEF_KEY[6] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};
static const uint8_t MAD_KEY[6] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5};
```

Apparently the NDEF_KEY used to determine if the tag has NDEF message on it or not. The esphome libarary only supports NDEF data on the tag which makes sense since any additional data would need custom parsing anyway. 


Esphome has 2 ways to make a tag. The first only creates an empty tag with UID. The latter contains the UID and the actual data (called `buffer`)

```cpp
return make_unique<nfc::NfcTag>(uid, nfc::MIFARE_CLASSIC);
```
vs
```cpp
return make_unique<nfc::NfcTag>(uid, nfc::MIFARE_CLASSIC, buffer);
```

It probably will be best to add the keys as attributes to the object so that data can be read in the future without having to rerun the KDF. 


## Likely Path forward

The details to implement this are stil vague, however most likely it will require the following

1. Usage of [nfc-tools/mfcuk/crytpo1.c](https://github.com/nfc-tools/mfcuk/blob/master/src/crypto1.c) to implement the crypto1 algorithm for leverating keys on a mifare classic tag
2. Creating a wrapper around crytpo1.c 
3. Changing the licence to be a multi license file
4. Deciding if using the leaked known keys is a good idea or not. The leaked keys can be found on [XaviTorello/mifare-classic-toolkit](https://github.com/XaviTorello/mifare-classic-toolkit)
5. Studying how one of the [popular arduino libaries](https://github.com/miguelbalboa/rfid) implemented crypto1, or look at how they [read the keys](https://github.com/miguelbalboa/rfid/blob/master/examples/rfid_default_keys/rfid_default_keys.ino)
6. Implment `PN532_COMMAND_INAUTHMIFAREKEY`


## Arduino libarary

The arduino libarary has the crypto1 algorithm here: https://github.com/miguelbalboa/rfid/blob/master/src/MFRC522.cpp#L901-L925

The pn532 has a built in hardware encryption. You might be able to start/stop the encryption on the card

https://github.com/miguelbalboa/rfid/blob/master/src/MFRC522.cpp#L931-L934


## Resources

- [Chat with AI](https://www.perplexity.ai/search/write-arduino-function-to-read-_FE4WYe0RnehNTZVpKD9uA)
- [Reading YubiKeys NDEF data with keys](https://github.com/esphome/feature-requests/issues/2207)



I've verified that I can read my tags using either the keys dervied from the KDF, or using the leaked key `A396EFA4E24F`.


```
[usb] pm3 --> hf mf rdbl -c 4 --key A396EFA4E24F --blk 1

[=]   # | sector 00 / 0x00                                | ascii
[=] ----+-------------------------------------------------+-----------------
[=]   1 | 41 30 30 2D 41 30 00 00 47 46 41 30 30 00 00 00 | A00-A0..GFA00...
```

And using correct keys

```
[usb] pm3 --> hf mf rdbl -c 0 --key 63e5af2c1d75 --blk 1

[=]   # | sector 00 / 0x00                                | ascii
[=] ----+-------------------------------------------------+-----------------
[=]   1 | 41 30 30 2D 41 30 00 00 47 46 41 30 30 00 00 00 | A00-A0..GFA00...
```


Known mifare keys

```
ffffffffffff
a0b0c0d0e0f0
a1b1c1d1e1f1
a0a1a2a3a4a5 
b0b1b2b3b4b5
4d3a99c351dd 
1a982c7e459a
000000000000
aabbccddeeff
d3f7d3f7d3f7
aabbccddeeff
714c5c886e97
587ee5f9350f
a0478cc39091
533cb6c723f6
8fd0a4f256e9
```

Backdoor key: https://news.risky.biz/risky-biz-news-hardware-backdoors-found-in-chinese-key-cards/


----

static const uint8_t BACKDOOR_KEY[6] = {(uint8_t)0xA3, (uint8_t)0x96, (uint8_t)0xEF, (uint8_t)0xA4, (uint8_t)0xE2, (uint8_t)0x4F};
static const uint8_t KEY1[6] =   {(uint8_t)0x63, (uint8_t)0xe5, (uint8_t)0xaf, (uint8_t)0x2c, (uint8_t)0x1d, (uint8_t)0x75};
static const uint8_t KEY2[6] =   {(uint8_t)0x40, (uint8_t)0xd1, (uint8_t)0x46, (uint8_t)0xce, (uint8_t)0x6e, (uint8_t)0x01};
static const uint8_t KEY3[6] =   {(uint8_t)0x6a, (uint8_t)0x66, (uint8_t)0x95, (uint8_t)0x7d, (uint8_t)0xcc, (uint8_t)0x91};
static const uint8_t KEY4[6] =   {(uint8_t)0x15, (uint8_t)0xe7, (uint8_t)0x04, (uint8_t)0x1f, (uint8_t)0x68, (uint8_t)0xd9};
static const uint8_t KEY5[6] =   {(uint8_t)0x7e, (uint8_t)0xe1, (uint8_t)0xac, (uint8_t)0x7f, (uint8_t)0xa7, (uint8_t)0x5f};
static const uint8_t KEY6[6] =   {(uint8_t)0x55, (uint8_t)0xcb, (uint8_t)0xba, (uint8_t)0xd1, (uint8_t)0x86, (uint8_t)0x73};
static const uint8_t KEY7[6] =   {(uint8_t)0xce, (uint8_t)0x59, (uint8_t)0x01, (uint8_t)0xaf, (uint8_t)0x94, (uint8_t)0x16};
static const uint8_t KEY8[6] =   {(uint8_t)0xa2, (uint8_t)0x23, (uint8_t)0xa1, (uint8_t)0x93, (uint8_t)0xe6, (uint8_t)0xa3};
static const uint8_t KEY9[6] =   {(uint8_t)0x24, (uint8_t)0xf4, (uint8_t)0xd0, (uint8_t)0x22, (uint8_t)0xf4, (uint8_t)0x02};
static const uint8_t KEY10[6] =  {(uint8_t)0x7d, (uint8_t)0xf9, (uint8_t)0x99, (uint8_t)0xdd, (uint8_t)0x83, (uint8_t)0x6b};
static const uint8_t KEY11[6] =  {(uint8_t)0xb0, (uint8_t)0xda, (uint8_t)0xc4, (uint8_t)0xa4, (uint8_t)0x89, (uint8_t)0x03};
static const uint8_t KEY12[6] =  {(uint8_t)0xb0, (uint8_t)0x26, (uint8_t)0xab, (uint8_t)0x56, (uint8_t)0x6f, (uint8_t)0x11};
static const uint8_t KEY13[6] =  {(uint8_t)0x8b, (uint8_t)0x49, (uint8_t)0x5d, (uint8_t)0x5a, (uint8_t)0x0b, (uint8_t)0x44};
static const uint8_t KEY14[6] =  {(uint8_t)0x7e, (uint8_t)0xbe, (uint8_t)0xf1, (uint8_t)0xcb, (uint8_t)0x3e, (uint8_t)0x94};
static const uint8_t KEY15[6] =  {(uint8_t)0x46, (uint8_t)0x85, (uint8_t)0x79, (uint8_t)0x0c, (uint8_t)0x6e, (uint8_t)0x01};
static const uint8_t KEY16[6] =  {(uint8_t)0x3f, (uint8_t)0x00, (uint8_t)0x14, (uint8_t)0x4c, (uint8_t)0x7b, (uint8_t)0x4a};

static const uint8_t *const keys[] = { nfc::NDEF_KEY, BACKDOOR_KEY, KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9, KEY10, KEY11, KEY12, KEY13, KEY14, KEY15, KEY16};
