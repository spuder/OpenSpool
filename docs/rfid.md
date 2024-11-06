---
layout: default
title: RFID Tags
nav_order: 2
---

# RFID Tags

At the heart of OpenSpool are the `NFC/RFID` tags that you will place on every spool you want to automate. The OpenSpool hardware works with a wide variety of High-Frequency tags (13.56Mhz)

{: .note-title }
> NFC vs RFID
> 
> `NFC` is to `RFID` like `Bluetooth` is to `Radio`.  
> All NFC Tags are RFID tags, but not all RFID tags are NFC tags. 

### Tag Selection

Purchase NTAG 215 or 216

[NTAG 215 - Amazon](https://a.co/d/5ojDUNk)  
[NTAG 215 - Aliexpress](https://www.aliexpress.us/item/3256806144939092.html)  

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


## Protocol

OpenSpool NFC tags create an NDEF message on the tag. NDEF messages are human readable and can be viewed from your phone.

[iPhone - NFC Tools app](https://apps.apple.com/us/app/nfc-tools/id1252962749)  
[Android - NFC Tools app](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://play.google.com/store/apps/details%3Fid%3Dcom.wakdev.wdnfc%26hl%3Den_US&ved=2ahUKEwicptyi7caJAxXomO4BHZo4FIgQFnoECAwQAQ&usg=AOvVaw0XjR90J9AV8I4375hycopuz)  

The NDEF message is a single record of type `application/json`

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

![](./images/phoneNFC.jpeg)  