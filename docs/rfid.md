---
layout: default
title: RFID Tags
nav_order: 2
---

# RFID Tags

At the heart of OpenSpool are the RFID/NFC tags that you will place on every spool you want to automate. The OpenSpool hardware workds with a wide variety of High-Frequency tags (13.56Mhz)

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