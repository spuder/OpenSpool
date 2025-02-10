
<p align=center>
    <img src="./images/OpenSpoolLogoMedium1.png" width="200">
</p>


# OpenSpool
**Your filament wants to be free**

![](https://img.shields.io/badge/openspool-1.18.0-magenta)
[![Discord](https://img.shields.io/discord/1298381115706576907?logo=discord)](https://discord.gg/4EaXHu9CEj) [![Reddit](https://img.shields.io/badge/reddit-join-orange?logo=reddit)](https://www.reddit.com/r/openspool)  

---


# 📖 [OpenSpool.io](https://openspool.io)

Checkout the official documentation on [https://openspool.io](https://openspool.io) 👈

---


<p align="center"> 
    <img src="https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExcWxoemp6bTdmNm02c2t5cnF4MGU3M2Znb3d3MWQ1c2E5NnF6YTBkZCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/BmAJiIJ0wcBnsKGBBL/giphy.gif" alt="Centered GIF"> 
</p>


Adhere NFC stickers on all your filament. Build an ESP32 OpenSpool reader and place it next to your printer. 
Touching the filament to the reader will automatically update your Bambu Printer filament settings. 
Almost as seamlessly as if you were using Bambu filament with an AMS. 





## 🖨️ Protocol


| Protocol  | Read          | Write         | Type              |
| --------- | ------------- | ------------- | ----------------- |
| OpenSpool | ✅             | ✅             | NTAG 215/216      |
| Bambu     | 🚧 In progress | ❌             | MiFare Classic 1k |
| OpenTag   | 🚧 In progress | 🚧 In progress | NTAG 216          |
| Creality  | 🗓️ Planned     | 🗓️ Planned     | MiFare Classic 1k |
| PrusaRFID | ❓             | ❓             | ❓                 |

### 🧬 OpenTag Protocol

Many 3d printer filament companies are uniting around an open standard called `OpenTag`. It planned to be finalized in early-mid 2025. OpenSpool will support OpenTag formatted tags as soon as it is finalized. See [here](https://github.com/Bambu-Research-Group/RFID-Tag-Guide/blob/main/OpenTag.md) for more information.

### 🧬 OpenSpool Protocol

OpenSpool aims to use the simplest possible protocol for RFID tags. All you need is a [NTAG215/216](https://a.co/d/5ojDUNk) 
#### NDEF Record

Example record stored on NFC tag. 

```
type = `application/json`  
payload = 
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


<p align=center>
    <img src="./images/phone2.png" width="200">
</p>


#### Web Preview

Spool information can be seen using your phone.

<p align=center>
    <img src="./images/NFC2.png" width="200">
</p>


```
https://openspool.io/tag_info?color_hex=FFAABB&type=PLA&brand=Generic&min_temp=220&max_temp=240&protocol=openspool&version=1.0
```


## 🐼 Integrations

Currently, OpenSpool works with Bambu printers over MQTT. Other integrations are planned. Pull Requests welcome

| Integration         | Supported     |
| ------------------- | ------------- |
| Bambu               | ✅             |
| OctoPrint           | 🚧 In Progress |
| Prusa Connect       | 🗓️ Planned     |
| SpoolMan            | 🗓️ Planned     |
| Klipper / Moonraker | 🗓️ Planned     |
| Anker               | ❔             |

## 🔧 Hardware

OpenSpool requires an ESP32-S3 and PN532 NFC Reader (SPI Mode).

The OpenSpool schematics will always be free. If you don't want to source the parts yourself, you can 
[buy a kit on tindie.com](https://www.tindie.com/products/36746/)

<a href="https://www.tindie.com/stores/spuder/?ref=offsite_badges&utm_source=sellers_spuder&utm_medium=badges&utm_campaign=badge_small"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-smalls.png" alt="I sell on Tindie" width="200" height="55"></a>


![](./images/tindie/IMG_6125.png)
![](./images/tindie/IMG_6120.png)



| Image                                                                         | Hardware                                    | Link 1                            | Link 2                                                                                  |
| ----------------------------------------------------------------------------- | ------------------------------------------- | --------------------------------- | --------------------------------------------------------------------------------------- |
| <img src="./images/wemos-d1minis3.png" alt="D1 Mini s3" height="200">         | Wemos D1 Mini s3                            |                                   | [AliExpress](https://www.aliexpress.us/item/3256805262904443.html?gatewayAdapt=glo2usa) |
| <img src="./images/pn532-small.png" alt="PN532" height="200">                 | PN532 (Small)                               | [Amazon](https://amzn.to/4eoBz8s) | [AliExpress](https://www.aliexpress.us/item/3256805787598774.html)                      |
| <img src="./images/NFC.png" alt="NTAG 215/216" height="200">                  | NTAG 215/216 (13.56Mhz NFC Tags >500 bytes) | [Amazon](https://amzn.to/4epJzpO) |                                                                                         |
| <img src="./images/LED1.png" alt="WS2812B LED" height="200">                  | WS2812B LED                                 | [Amazon](https://amzn.to/40FFOt5) |                                                                                         |
| <img src="./images/Headers1.png" alt="2.54mm headers" height="200">           | 2.54mm Headers                              | [Amazon](https://amzn.to/4en6138) |                                                                                         |
| <img src="./images/LLC1.png" alt="Logic Level Converter" height="200">        | 3.3v - 5v Logic Level Converter             | [Amazon](https://amzn.to/3UMfMkp) |                                                                                         |
| <img src="./images/jumper1.png" alt="Female-Female Jumper Wire" height="200"> | female-female jumper wire                   | [Amazon](https://amzn.to/3AMwRDM) |                                                                                         |

If you are unable to find a Wemos D1 Mini `s3`, you can still buy an `s2`, as it is pin compatable. However, be advised the `s2` has less memory and may experience occasional disconnects from the printer

| Image                                                               | Hardware                            | Link 1                            | Link 2                                                             |
| ------------------------------------------------------------------- | ----------------------------------- | --------------------------------- | ------------------------------------------------------------------ |
| <img src="./images/wemos-d1mini.png" alt="D1 Mini s3" height="200"> | Wemos D1 Mini S2 (Beware of Fakes!) | [Amazon](https://amzn.to/4fqq9m7) | [AliExpress](https://www.aliexpress.us/item/3256802958877264.html) |


| Tag      | Bytes     | Supported |
| -------- | --------- | --------- |
| NTAG 213 | 144 bytes | ❌         |
| NTAG 215 | 504 bytes | ✅         |
| NTAG 216 | 888 bytes | ✅         |


# Wiring Diagram


<img src="./images/OpenSpoolMiniWiringDiagram2.png" width="500">

Be sure to put the PN532 into `SPI` mode (not `i2c` or `UART`)

<img src="./images/PN532-4.jpg" width="300">




## 🤖 Firmware

The recommended method to install firmware is by navigating to [openspool.io](https://openspool.io) in Chrome/Edge and using the web based programming tool. 

Alternatively the firmware can be installed with `make` commands as shown below. 

### OSX/Linux

```
brew install esphome

git clone https://github.com/spuder/OpenSpool
cd OpenSpool/firmware
```

Press and hold the D0 button while pressing the reset button on the Wemos D1 Mini.  
A new USB serial device will appear. 

```bash
ls /dev/cu*
```

Upload firmware and restart

```bash
USB_ADDRESS=/dev/cu.usbmodemXXXXX make lolin_s2_mini
```

or

```bash
USB_ADDRESS=/dev/cu.usbmodemXXXXX make lolin_s3_mini
```

## 🛜 First Time Configuration

A new Wi-Fi network will appear called `OpenSpool`, join the network, navigate to [192.168.4.1](http://192.168.4.1), insert your Wi-Fi credentials and reboot. 

You can then navigate to the web interface at `openspool-xxxxxx.local` or IP address. 


## 🎛️ Configuration

On first boot, you will need to specify your 3d printer IP Address, Serial Number and LAN Access Code. 

Do not put your printer into LAN Only Mode, it is not required. 

![](./images/PrinterSettings.png)

After changing these 3 settings, you will need to reboot the ESP32. 
Upon successful connection to the 3d printer, you will observe a checkmark next to MQTT in the web interface.

![](./images/WebInterface1.png)


## Factory Reset

Press and hold the D0 button for 10 seconds. 
This will erase Wi-Fi credentials and Bambu access credentials from esp32. 

# 🔧 Troubleshooting

### LED is breathing blue
This means the OpenSpool is hosting a wireless access point called 'OpenSpool'. Connect to it then navigate to [192.168.4.1](http://192.168.4.1) to configure Wi-Fi. 

### LED is solid white
OpenSpool has successfully joined the Wi-Fi network and is ready to scan for RFID tags


### NFC Tags aren't reading reliably

If using the red NP5332 reader (smaller model), it detects tags best on the corners of the antenna. 

If using the blue PN532 reader (larger model), it detects tags best directly in the center of the antenna.   

Be sure you aren't using a [counterfeit PN532](https://forum.dangerousthings.com/t/success-pn532-is-not-easy-to-work-with/1108/10)

### MQTT Disconnects

The A1/P1 have a limit of 4 mqtt connections. If more than 4 connections are reached clients may be asked to disconnect. 

If a client attempts to write invalid data any mqtt topic, the printer will ask the client to disconnect. 
All ESPhome resources must use `state_topic:` with a null value to prevent ESPhome from broadcasting on any topic. 


### TLS Errors

Reach out to the discord for help to verify that the Root Certificate is correct. 


### Advanced Troubleshooting

Use the [MQTTX client](https://mqttx.app) to observe the messages that OpenSpool and Bambu Printer are sending. 


```bash
mqttx sub -t 'device/$SERIAL_NUMBER/report' -u bblp -P $LAN_ACCESS_CODE --mqtt-version 3.1.1 -h $IP_ADDRESS -p 8883 -l mqtts --insecure
```

## 🎥 Demo

[![](https://img.youtube.com/vi/ah7dm-dtQ5w/0.jpg)](https://youtube.com/watch?v=ah7dm-dtQ5w)
[![](https://img.youtube.com/vi/pWnvJc-8KLI/0.jpg)](https://youtube.com/shorts/pWnvJc-8KLI?si=9-C1rhqrvL1i5CpR)
[![](https://img.youtube.com/vi/kvVpRKFNU9k/0.jpg)](https://www.youtube.com/shorts/kvVpRKFNU9k)


# 👥 Contributors

<!-- readme: contributors -start -->
<table>
	<tbody>
		<tr>
            <td align="center">
                <a href="https://github.com/spuder">
                    <img src="https://avatars.githubusercontent.com/u/242382?v=4" width="100;" alt="spuder"/>
                    <br />
                    <sub><b>Spencer Owen</b></sub>
                </a>
            </td>
            <td align="center">
                <a href="https://github.com/raihei">
                    <img src="https://avatars.githubusercontent.com/u/34209267?v=4" width="100;" alt="raihei"/>
                    <br />
                    <sub><b>raihei</b></sub>
                </a>
            </td>
            <td align="center">
                <a href="https://github.com/Tnthr">
                    <img src="https://avatars.githubusercontent.com/u/131608369?v=4" width="100;" alt="Tnthr"/>
                    <br />
                    <sub><b>Tnthr</b></sub>
                </a>
            </td>
            <td align="center">
                <a href="https://github.com/phillipthelen">
                    <img src="https://avatars.githubusercontent.com/u/298062?v=4" width="100;" alt="phillipthelen"/>
                    <br />
                    <sub><b>Phillip Thelen</b></sub>
                </a>
            </td>
            <td align="center">
                <a href="https://github.com/thijstriemstra">
                    <img src="https://avatars.githubusercontent.com/u/305679?v=4" width="100;" alt="thijstriemstra"/>
                    <br />
                    <sub><b>Thijs Triemstra</b></sub>
                </a>
            </td>
            <td align="center">
                <a href="https://github.com/thomasjpatterson">
                    <img src="https://avatars.githubusercontent.com/u/8883279?v=4" width="100;" alt="thomasjpatterson"/>
                    <br />
                    <sub><b>secprepper</b></sub>
                </a>
            </td>
		</tr>
	<tbody>
</table>
<!-- readme: contributors -end -->

# 📜 Certifications

<p align="center">
    <a href="https://certification.oshwa.org/us002704.html">
        <img src="./images/oshw-logo.svg" width="200" 
        style="border: 1px solid #ffffff; border-radius: 25px; padding: 10px; margin-bottom: 5px;">
    </a>
</p>
<p align="center">
    <a href="https://certification.oshwa.org/us002704.html">
        <img src="./images/certification-mark-US002704-wide.svg" width="300" style="border: 1px solid #ffffff; border-radius: 25px; padding: 10px; margin-bottom: 5px;">
    </a>
</p>
<p align="center">
    <a href="https://certification.oshwa.org/us002704.html">OSHWA Certification: US002704</a>
</p>

<p align="center">
    <a href="https://devices.esphome.io/devices/OpenSpool">
        <img src="./images/made-for-esphome-white-on-black.svg" width="400" >
    </a>
</p>
<p align="center">
    <a href="https://devices.esphome.io/devices/OpenSpool">ESPHome Device Listing</a>
</p>

# 🪪 License

<p align="left">
    <a href="https://oshwa.github.io/certification-mark-generator/facts">
        <img src="./images/oshw_facts-2.svg" width="300">
    </a>
</p>


# Donations

All contributions to the project are welcome. Skills needed:

- Software Development
- Photography
- Videography
- Documentation
- Electrical Design
- Testing/Integration

You can also donate to support research and development costs

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/openspool)
<a href="https://www.tindie.com/stores/spuder/?ref=offsite_badges&utm_source=sellers_spuder&utm_medium=badges&utm_campaign=badge_small"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-smalls.png" alt="I sell on Tindie" width="200" height="55"></a>

https://oshwlab.com/owenspencer/openspool-mini