# Development

```
brew install esphome
```


```bash
ls /dev/cu*
export USB_ADDRESS=/dev/cu.usbmodemXXXX # replace XXXX with your usb port
cd firmware

make run-usb
```

After you have succesfully uploaded firmware once, you can start using a faster update

```bash
make run-ota
```


## MQTT

Connect
```bash
mqttx conn -u bblp -P $LAN_ACCESS_CODE --mqtt-version 3.1.1 -h 192.168.81.176 -p 8883 -l mqtts --insecure
```

Connect and subscribe

```bash
mqttx sub -t 'device/$SERIAL_NUMBER/report' -u   -P $LAN_ACCESS_CODE --mqtt-version 3.1.1 -h $IP_ADDRESS -p 8883 -l mqtts --insecure
```

Be sure no sensor posts its data to mqtt by leaving `state_topic` and `command_topic` empty

https://github.com/esphome/esphome/pull/5213

```yaml
sensor:
    state_topic:
switch:
    command_topic:
```



### Certs

CA Cert expires: `Apr  1 03:42:11 2032 GMT`

Get Certificate authority (pem format)

openssl s_client -connect 192.168.81.176:8883 -showcerts < /dev/null 2>/dev/null | openssl x509 -outform PEM
openssl s_client -connect 192.168.81.103:8883 -showcerts < /dev/null 2>/dev/null | openssl x509 -outform PEM

openssl s_client -connect test.mosquitto.org:8883 -showcerts < /dev/null 2>/dev/null | openssl x509 -outform PEM
openssl s_client -connect broker.emqx.io:8883 -showcerts < /dev/null 2>/dev/null | openssl x509 -outform PEM






openssl s_client -showcerts -connect 192.168.81.176:8883 </dev/null




### Bookmarks

PR where esp-idf support was added to esphome: https://github.com/esphome/esphome/pull/2930/files
Mbed-TLS doesn't support ignoring on TLS 1.3: https://github.com/Mbed-TLS/mbedtls/issues/7075
- https://github.com/Mbed-TLS/mbedtls/issues/9018



## Bambu

Bambu Printers mqtt server will reject connections from mqtt clients that are missing the clean_session tag. 

Here is an example of a connection where `c0` is set. 
`[mqtt-broker-tls-a1mini]  | 1728959892: New client connected from 10.89.0.14:44848 as thisshoudlbreak (p2, c0, k15, u'bblp').`

Not that this isn't yet in mainstream esphome, its only in the beta channel 

https://discord.com/channels/429907082951524364/1295578749638676480/1295580772350165092

```yaml
mqtt:
  clean_session: true
```