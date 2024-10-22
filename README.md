# OpenSpool

RFID tracking system for 3d printer filament


OpenSpool is a combination of hardware and software that mimics the functionality of RFID tags on BambuLab filament roles. 

## Objectives

- Make it so easy to put RFID stickers on filament spools that your grandma could do it. 
- 100% programable using just a modern iphone/android (no RFID programmer or scripting).
- Change filament in 10 seconds using 1 hand. 
- 100% reliability so that filament can be changed with your eyes closed. 

## Demo

[![](https://img.youtube.com/vi/ah7dm-dtQ5w)](https://youtube.com/watch?v=ah7dm-dtQ5w)



## Protocol

OpenSpool aims to use the simplest possible protocol for RFID tags. All you need is a 13.56Mhz RFID tag that supports NDEF records (NFC Tags). 

### NDEF Record

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

TODO: Document hardware requirements



