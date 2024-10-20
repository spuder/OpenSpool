# MQTT Documentation

https://github.com/Doridian/OpenBambuAPI/blob/main/mqtt.md

# MQTT connection

[](/images/mqttx.png)
[](/images/Change_filament.png)


I used applciation MQTTX. I couldn't get MQTT Explorer to work since I coudln't find a way to downgrade to version 3.1.1


- Use mqtt version 3.1.1 (5 doesn't seem to work)
- Subscribe to topic `device/$serialNumber/report`


## Change filament

Send `request` to topic `device/$serialNumber/request`
```
{
  "print": {
      "sequence_id": "0",
      "command": "ams_filament_setting",
      "ams_id": 0,
      "tray_id": 0,
      "tray_info_idx": "GFL99",
      "tray_color": "0000FFFF",  // BLUE
      "nozzle_temp_min": 190,
      "nozzle_temp_max": 230,
      "tray_type": "PLA"
  }
}
```


Panda touch uses ESP32S3R8


https://github.com/gpambrozio/BambuAppDaemon



Espressif fourms mention error 119

https://www.esp32.com/viewtopic.php?p=98798#p98798


here is how xtouch talks mqtt

https://github.com/xperiments-in/xtouch/blob/c0b474b5e608ceacf23dfea3a39d029456d38948/src/xtouch/mqtt.h

Here are the certs for public cloud

https://github.com/xperiments-in/xtouch/blob/c0b474b5e608ceacf23dfea3a39d029456d38948/src/xtouch/bbl-certs.h



mqtt has a printer type that is different for X1

https://github.com/xperiments-in/xtouch/blob/c0b474b5e608ceacf23dfea3a39d029456d38948/src/xtouch/mqtt.h#L49-L67


# Ideas

Unable to auth to bambu printers

- TLS 1.3? 
- What version of esp-if does esphome use? 
- Possible to upgrade? 
- Does esp-if support leaf certificates?
- Can I setup a mqtt server with leaf certificates to reproduce?
  - broker.hivemq.com
 
- Am I sure I have the correct certificate in the config? 
- Couild it be a Certifiate type problem? 
  - No, both hivemq and bambu use RSA 2048 (RSA-SHA256)
- Self signed cert in certificate chain
  - hivemq.com has a signed chert, bambu has a self signed cert
- TLS version? 
  - Both are using TLS v1.2
  - hivemq is using ECDHE-RSA-AES128-GCM-SHA256
  - bambu is using ECDHE-RSA-AES256-GCM-SHA384
- Can esp32 handle AES256 (SHA384)? 
- Can esp32 handle 

NOtice openssl gives error 19 about self signed cert, mqtt was giving an error 19 at one point
 `Verify return code: 19 (self-signed certificate in certificate chain)`



## Test1 

```
No client certificate CA names sent
Peer signing digest: SHA256
Peer signature type: RSA-PSS
Server Temp Key: X25519, 253 bits
---
SSL handshake has read 2360 bytes and written 379 bytes
Verification error: self-signed certificate in certificate chain
---
New, TLSv1.3, Cipher is TLS_AES_256_GCM_SHA384
Server public key is 2048 bit
This TLS version forbids renegotiation.
Compression: NONE
Expansion: NONE
No ALPN negotiated
Early data was not sent
Verify return code: 19 (self-signed certificate in certificate chain)
```

Result: 

```
[21:04:35][V][esp-idf:000][mqtt_task]: E (188632) esp-tls: [sock=57] delayed connect error: Connection reset by peer
[21:04:35]
[21:04:35][V][esp-idf:000][mqtt_task]: E (188634) esp-tls: Failed to open new connection
[21:04:35]
[21:04:35][V][esp-idf:000][mqtt_task]: E (188637) TRANSPORT_BASE: Failed to open a new connection
[21:04:35]
[21:04:35][V][esp-idf:000][mqtt_task]: E (188639) MQTT_CLIENT: Error transport connect
[21:04:35]
[21:04:35][V][mqtt.idf:121]: Event dispatched from event loop event_id=0
[21:04:35][E][mqtt.idf:162]: MQTT_EVENT_ERROR
[21:04:35][E][mqtt.idf:164]: Last error code reported from esp-tls: 0x8004
[21:04:35][E][mqtt.idf:165]: Last tls stack error number: 0x0
[21:04:35][E][mqtt.idf:167]: Last captured errno : 104 (Connection reset by peer)
[21:04:35][V][mqtt.idf:121]: Event dispatched from event loop event_id=2
[21:04:35][V][mqtt.idf:133]: MQTT_EVENT_DISCONNECTED
[21:04:35][W][mqtt:333]: MQTT Disconnected: TCP disconnected.
[21:04:35][I][mqtt:244]: Connecting to MQTT...
[21:04:35][V][esp-idf:000]: I (188646) MQTT_CLIENT: Client asked to disconnect
```

Results, mistakenly pointed at 127.0.0.1 instead of 192.168.81.x

## Test 2

Success! 

config:
```
listener 8883
protocol mqtt
cafile /mosquitto/certs/ca-cert.crt
# certfile /mosquitto/certs/printer1-cert.crt
# keyfile /mosquitto/certs/printer1-key.pem
require_certificate false
password_file /etc/mosquitto/password_file
allow_anonymous true
```

```
[22:33:52][V][esp-idf:000][wifi]: I (4772) wifi:
[22:33:52][V][esp-idf:000][wifi]: <ba-add>idx:1 (ifx:0, 8a:2a:a8:c7:3d:0d), tid:0, ssn:0, winSize:64
[22:33:52][V][esp-idf:000][wifi]: 
[22:33:52]
[22:33:52][V][esp-idf:000][mqtt_task]: E (4808) esp-tls-mbedtls: mbedtls_ssl_handshake returned -0x7280
[22:33:52]
[22:33:52][V][esp-idf:000][mqtt_task]: I (4810) esp-tls-mbedtls: Certificate verified.
[22:33:52]
[22:33:52][V][esp-idf:000][mqtt_task]: E (4812) esp-tls: Failed to open new connection
[22:33:52]
[22:33:52][V][esp-idf:000][mqtt_task]: E (4814) TRANSPORT_BASE: Failed to open a new connection
[22:33:52]
[22:33:52][V][esp-idf:000][mqtt_task]: E (4818) MQTT_CLIENT: Error transport connect
[22:33:52]
[22:33:52][V][mqtt.idf:121]: Event dispatched from event loop event_id=0
[22:33:52][E][mqtt.idf:162]: MQTT_EVENT_ERROR
[22:33:52][E][mqtt.idf:164]: Last error code reported from esp-tls: 0x801a
[22:33:52][E][mqtt.idf:165]: Last tls stack error number: 0x7280
[22:33:52][E][mqtt.idf:167]: Last captured errno : 0 (Success)
[22:33:52][V][mqtt.idf:121]: Event dispatched from event loop event_id=2
[22:33:52][V][mqtt.idf:133]: MQTT_EVENT_DISCONNECTED
[22:33:52][W][mqtt:333]: MQTT Disconnected: TCP disconnected.
[22:33:57][I][mqtt:244]: Connecting to MQTT...
[22:33:57][V][esp-idf:000]: I (9730) MQTT_CLIENT: Client asked to disconnect
```

After I rebooted esp32

```
 1728534937: New connection from 10.89.0.22:38392 on port 8883.
[mqtt-broker-tls] | 1728534938: New client connected from 10.89.0.22:38392 as openspool (p2, c0, k15, u'bblp').
```

no clue why it says 10.89.0.22 (docker network perhaps?)


## Arduino

Someone got it working with arduino

https://www.reddit.com/r/BambuLab/comments/167r16h/esp32_mqtt_connection/

https://randomnerdtutorials.com/esp32-https-requests/#esp32-https-requests-wificlientsecure-without-certificate



{
    "print": {
        "sequence_id": "2011",
        "command": "ams_filament_setting",
        "ams_id": 255,
        "tray_id": 254,
        "tray_info_idx": "GFA05",
        "tray_color": "FFF144FF",
        "nozzle_temp_min": 190,
        "nozzle_temp_max": 240,
        "tray_type": "PLA",
        "setting_id": "",
        "reason": "success",
        "result": "success"
    }
}



# Black PETG
{
    "print": {
        "sequence_id": "2006",
        "command": "ams_filament_setting",
        "ams_id": 255,
        "tray_id": 254,
        "tray_info_idx": "GFG99",
        "tray_color": "161616FF",
        "nozzle_temp_min": 220,
        "nozzle_temp_max": 270,
        "tray_type": "PETG",
        "setting_id": "",
        "reason": "success",
        "result": "success"
    }
}


# Blue TPU
{
    "print": {
        "sequence_id": "2010",
        "command": "ams_filament_setting",
        "ams_id": 255,
        "tray_id": 254,
        "tray_info_idx": "GFU99",
        "tray_color": "2850E0FF",
        "nozzle_temp_min": 200,
        "nozzle_temp_max": 250,
        "tray_type": "TPU",
        "setting_id": "",
        "reason": "success",
        "result": "success"
    }
}

# Purple PETG
{
    "print": {
        "sequence_id": "2012",
        "command": "ams_filament_setting",
        "ams_id": 255,
        "tray_id": 254,
        "tray_info_idx": "GFG99",
        "tray_color": "A03CF7FF",
        "nozzle_temp_min": 220,
        "nozzle_temp_max": 270,
        "tray_type": "PETG",
        "setting_id": "",
        "reason": "success",
        "result": "success"
    }
}
{
    "print": {
        "sequence_id": "2013",
        "command": "extrusion_cali_sel",
        "nozzle_diameter": "0.4",
        "nozzle_volume_type": "normal",
        "filament_id": "GFG99",
        "tray_id": 254,
        "ams_id": 255,
        "cali_idx": -1,
        "reason": "success",
        "result": "success"
    }
}
{
    "print": {
        "vt_tray": {
            "tray_info_idx": "GFG99",
            "tray_type": "PETG",
            "tray_color": "A03CF7FF",
            "nozzle_temp_max": "270",
            "nozzle_temp_min": "220"
        },
        "command": "push_status",
        "msg": 1,
        "sequence_id": "5707"
    }
}