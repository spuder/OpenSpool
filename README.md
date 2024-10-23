# OpenSpool

RFID tracking system for 3d printer filament


OpenSpool is a combination of hardware and software that mimics the functionality of RFID tags on BambuLab filament roles. 

## Objectives

- Make it so easy to put RFID stickers on filament spools that your grandma could do it. 
- 100% programable using just a modern iphone/android (no RFID programmer or scripting).
- Change filament in 10 seconds using 1 hand. 
- 100% reliability so that filament can be changed with your eyes closed. 

## Demo

[![](https://img.youtube.com/vi/ah7dm-dtQ5w/0.jpg)](https://youtube.com/watch?v=ah7dm-dtQ5w)



## Protocol

OpenSpool supports 2 protocols

- OpenSpool  
- [OpenSourceRFID](https://github.com/Bambu-Research-Group/RFID-Tag-Guide/blob/main/OpenSourceRfid.md)  #TODO: not yet working


### OpenSpool Protocol

OpenSpool aims to use the simplest possible protocol for RFID tags. All you need is a 13.56Mhz RFID tag that supports NDEF records (NFC Tags). 

#### NDEF Record

Example record stored on NFC tag. 

type = `application/json`
payload = 

```json
{
    "protocol": "openspool",
    "version": "1.0",
    "type": "PLA",
    "color": "bambu_green",
    "brand": "Generic",
    "min_temp": "220",
    "max_temp": "240"
}
```

`color` and `type` are currently limited to the predefined colors of BambuLabs AMS. 



## Hardware

OpenSpool requires an ESP32-S2 and PN532 NFC Reader that supports SPI. 

| Microcontroller | Supported |
| --- | --- | 
| esp32-s2 | ✅ | 
| esp8266 | ❌ | 

| 

| | Hardware | Link 1 | Link 2| 
| --- | --- | --- | --- | 
| ![](./images/wemos-d1mini.png)| Wemos D1 Mini S2 | [Amazon](https://www.amazon.com/dp/B0B291LZ99?ref_=cm_sw_r_cp_ud_dp_373FWQ1MNKAS6SD4DDXV) | [Aliexpress](https://www.aliexpress.us/item/3256807359221969.html)|
| ![](./images/pn532-large.png)| PN532 (Large) | [Amazon](https://www.amazon.com/dp/B0CDWZ7SM9?ref_=cm_sw_r_cp_ud_dp_1GHJ4S94NDNBJSEZ05YX) | [Aliexpress](https://www.aliexpress.us/item/2251801319172315.html) | 
| ![](./images/pn532-small.png) |  PN532 (Small) | [Amazon](https://a.co/d/8ytFjLO) | [Aliexpress](https://www.aliexpress.us/item/3256805787598774.html)| 



## Firmware

### OSX

```
brew install esphome

git clone https://github.com/spuder/OpenSpool
cd OpenSpool/firmware
touch secrets.yaml
```

In the secrets.yaml file, populate the following variables

```yaml
bambu_ip: "192.168.2.0"
bambu_access_code: "12345678"
bambu_serial_number: XXXXXXXXXXXXXXXX
```


Press and hold the D0 button while pressing the reset button on the Wemos D1 Mini
A new usb serial device will appear. 

```bash
ls /dev/cu*
```

Upload firmware and restart

```bash
USB_ADDRESS=/dev/cu.usbmodemXXXXX make run-usb
```

A new wifi network will appear called `OpenSpool-xxxx`, join the network, insert your wifi credentials and reboot. 

You can then navigate to the web interface at `openspool.local` or ip address. 