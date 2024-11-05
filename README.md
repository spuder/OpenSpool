
<img src="./images/OpenSpoolLogo.png" width="200">

# OpenSpool
**Your filament wants to be free**

![](https://img.shields.io/badge/openspool-v0.1.41-magenta)
![](https://github.com/spuder/openspool/actions/workflows/esphome.yaml/badge.svg)
![](https://github.com/spuder/openspool/actions/workflows/pages.yaml/badge.svg)
[![](https://dcbadge.limes.pink/api/server/4EaXHu9CEj)](https://discord.gg/4EaXHu9CEj)

---


# 📖 [OpenSpool.io](https://openspool.io)

Checkout the ofifical documentation on [https://openspool.io](https://openspool.io) 👈

---

Adhere NFC stickers on all your filament. Build an ESP32 OpenSpool reader and place it next to your printer. 
Touching the filament to the reader will automatically update your Bambu Printer filament settings. 
Almost as seamlessly as if you were using Bambu filament with an AMS. 

## Objectives

- Make it so easy to put RFID stickers on filament spools that your grandma could do it. 
- 100% programable using just a modern iphone/android (no RFID programmer or scripting).
- Change filament in 10 seconds using 1 hand. 
- 100% reliability so that filament can be changed with your eyes closed. 

## 🎥 Demo

[![](https://img.youtube.com/vi/ah7dm-dtQ5w/0.jpg)](https://youtube.com/watch?v=ah7dm-dtQ5w)



## 🖨️ Protocol

OpenSpool supports 2 protocols

- [OpenSourceRFID](https://github.com/Bambu-Research-Group/RFID-Tag-Guide/blob/main/OpenSourceRfid.md)  #Pending 
- OpenSpool  

### OpenSourceRFID Protocol

The OpenSourceRFID protocol is still pending finalization, once it is finalized it will be the preferred protocol for OpenSpool. OpenSourceRFID has momentium from suppliers [PolarFilament](https://polarfilament.com) and [3D-fuel](https://www.3dfuel.com)
It uses Mifare 1k classic RFID tags. 

Follow [this github page](https://github.com/Bambu-Research-Group/RFID-Tag-Guide/blob/main/OpenSourceRfid.md) for updates to OpenSourceRFID


### OpenSpool Protocol

OpenSpool aims to use the simplest possible protocol for RFID tags. All you need is a [13.56Mhz RFID tag](https://a.co/d/5ojDUNk) that supports NDEF records (NFC Tags) and has atleast 500 bytes of memory. NTAG 215/216 are a great choice. Purchasing links provided below. 

#### NDEF Record

Example record stored on NFC tag. 

type = `application/json`  
payload = 

```json
{
    "protocol": "openspool",
    "version": "1.0",
    "type": "PLA",
    "color_hex": "FFAABB",
    "brand": "Generic",
    "min_temp": "220",
    "max_temp": "240"
}
```

<img src="./images/phone2.png" width="400">


## Integrations

Currently OpenSpool only works with Bambu printers over mqtt. Other integrations are planned. Pull Requests welcome

| Integration | Supported |
| --- | --- |
| Bambu | ✅ |
| Anker | ❔ | 
| Prusa Connect | ❔ |
| OctoPrint | ❔ |
| SpoolMan | ❔ |
| Klipper / Moonraker | ❔ |

## Hardware

OpenSpool requires an ESP32-S2 and PN532 NFC Reader that supports SPI. 

| Microcontroller | Supported |
| --- | --- | 
| esp32-s2 | ✅ | 
| esp8266 | ❌ | 



| | Hardware | Link 1 | Link 2| 
| --- | --- | --- | --- | 
| ![](./images/wemos-d1mini.png)| Wemos D1 Mini S2 (Beware of Clones!)| [Amazon](https://www.amazon.com/dp/B0B291LZ99?ref_=cm_sw_r_cp_ud_dp_373FWQ1MNKAS6SD4DDXV) | [Aliexpress](https://www.aliexpress.us/item/3256802958877264.html)|
| ![](./images/pn532-large.png) |  PN532 (Large) | [Amazon](https://a.co/d/8ytFjLO) | [Aliexpress](https://www.aliexpress.us/item/3256805787598774.html)|
| ![](./images/pn532-small.png) |  PN532 (Small) | [Amazon](https://a.co/d/aOXpu3g) | [Aliexpress](https://www.aliexpress.us/item/3256806348384449.html)| 
| ![](./images/NFC.png) | NTAG 215/216 (13.56Mhz NFC Tags >500 bytes) | [Amazon](https://a.co/d/5ojDUNk) | | 
| ![](./images/LED1.png) | WS2812B LED | [Amazon](https://a.co/d/dzN19Ky) | |
| ![](./images/PCB1.png) | 30x70mm PCB | [Amazon](https://a.co/d/dMfGMGb) | |
| ![](./images/Headers1.png) | 2.54mm Headers | [Amazon](https://a.co/d/gVvpTyd) | | 
| ![](./images/LLC1.png) | 3.3v - 5v Logic Level Converter | [Amazon](https://a.co/d/7IfuISb) | | 

# Wiring Diagram

![](./images/WiringDiagram2.png)


Be sure to put the PN532 into `SPI` mode (not `i2c` or `UART`)

![](./images/PN532-4.jpg)


| Tag | Bytes | Supported |  
| --- | --- | --- |  
| NTAG 213 | 144 bytes | ❌ | 
| NTAG 215 | 504 bytes | ✅ |  
| NTAG 216 | 888 bytes | ✅ |   


## Tag Placement

In order to comply with the [OpenSourceRfid standard](https://github.com/Bambu-Research-Group/RFID-Tag-Guide/blob/main/OpenSourceRfid.md), the tag should be placed according to the following constraints. 

- Tag center should be 56.0mm away from the center of the spool (see pic)
- The tag should never be more than 4.0mm away from the outside edge of the spool. For spool sides thicker than 4mm, there must be a cutout to embed the tag, or the tag should be fixed to the outside of the spool
- Two tags should be used, one on each end of the spool

![](https://github.com/Bambu-Research-Group/RFID-Tag-Guide/raw/main/images/TagLocation.png))

## Firmware

The recomended method to install firmware is by navigating to [openspool.io](https://openspool.io) in Chrome/Edge and using the web based programming tool. 

Alternativly the firmware can be installed with `make` commands as shown below. 

### OSX/Linux

```
brew install esphome

git clone https://github.com/spuder/OpenSpool
cd OpenSpool/firmware
```

Press and hold the D0 button while pressing the reset button on the Wemos D1 Mini.  
A new usb serial device will appear. 

```bash
ls /dev/cu*
```

Upload firmware and restart

```bash
USB_ADDRESS=/dev/cu.usbmodemXXXXX make run-usb
```

## Windows

TODO: Document windows flashing procedure


## First Time Configuration

A new wifi network will appear called `OpenSpool`, join the network, navigate to [192.168.4.1](http://192.168.4.1),  insert your wifi credentials and reboot. 

You can then navigate to the web interface at `openspool-xxxxxx.local` or ip address. 


## Configuration

On first boot, you will need to specify your 3d printer IP Address, Serial Number and Lan Access Code. 

Do not put your printer into LAN Only Mode, it is not required. 

![](./images/PrinterSettings.png)

After changing these 3 settings, you will need to reboot the ESP32. 
Upon successful connection to the 3d printer, you will observe a checkmark next to MQTT in the web interface, and a blue LED will illuminate next to the USB port. 

![](./images/WebInterface1.png)


## Facotry Reset

Press and hold the D0 button for 10 seconds. 
This will erase wifi credentials and bambu access credentials from OpenSpool. 

# Troubleshooting

### LED is breathing blue
This means the OpenSpool is hosting a wireless access point called 'OpenSpool'. Connect to it then navigate to [192.168.4.1](http://192.168.4.1) to configure wifi. 

### LED is solid white
OpenSpool has succesfully joined the wifi network and is ready to scan for RFID tags


### NFC Tags aren't reading reliably

If using the red NP5332 reader (smaller model), it detects tags best on the corners of the antenna. 

If using the blue NP532 reader (larger model), it detects tags best directly in the center of the antenna.   

Be sure you aren't using a [counterfit NP532](https://forum.dangerousthings.com/t/success-pn532-is-not-easy-to-work-with/1108/10)

### MQTT Disconnects

The A1/P1 have a limit of 4 mqtt connections. If more than 4 connections are reached clients may be asked to disconnect. 

If a client attempts to write invalid data any mqtt topic, the printer will ask the client to disconnect. 
All esphome resources must use `state_topic:` with a null value to prevent esphome from broadcasting on any topic. 


### TLS Errors

Reach out to the discord for help verifying that the Root Certificate is correct. 


### Advanced Troubleshooting

Use the [MQTTX client](https://mqttx.app) to observe the messages that OpenSpool and Bambu Printer are sending. 


```bash
mqttx sub -t 'device/$SERIAL_NUMBER/report' -u -P $LAN_ACCESS_CODE --mqtt-version 3.1.1 -h $IP_ADDRESS -p 8883 -l mqtts --insecure
```

# License


<p xmlns:cc="http://creativecommons.org/ns#" >Software, Hardware and 3d models are released under <a href="https://creativecommons.org/licenses/by-sa/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">CC BY-SA 4.0<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1" alt=""><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1" alt=""><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/sa.svg?ref=chooser-v1" alt=""></a></p>
