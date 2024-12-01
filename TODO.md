- [ ] Polylite manufacture codes
- [ ] Setup static page with improv-serial
    See dutch developer site for example
    ```
    <esp-web-install-button manifest="https://raw.githubusercontent.com/DutchDevelop/BLLEDController/main/firmware/manifest.json" install-supported=""></esp-web-install-button>
    ```

- [x] OpenSpool listed on improv website https://www.improv-wifi.com
- [x] Setup static tag validator page
- [x] Fix error when writing tags, worked in 0.1.41, but broken in master
- [x] Add BambuRFID creator button
- [x] Add openspool-xxxx.local creator button
- [x] Fix wifi blinking blue
- [x] Enable OTA updates on D1 mini
- [x] Add breathing animation
- [x] Add rainbow breathing
- [ ] Add magenta blinking
- [ ] Increase all 512 json docs to 1024 in prep for OpenTag standard
- [ ] MQTT reboots afeter x minutes if not connected
- [x] Add esp32-s3 support to manifest.json

# Hardware

- [x] Add 3mm offset to led light pipe
- [x] Print final version of mini
- [ ] Upload mini to MakerWorld so I start earning points
- [x] Make PCB v2 for mini
- [ ] Make PCB for Pro/AMS
- [ ] Test aluminum Tape
- [ ] Test RFID antennas

# Social

- [ ] Share on tiktok
- [ ] Share RFID Tag video
- [ ] Send 10 units to youtubers
- [ ] Host on etsy shop
- [ ] Host on tindie.com

# Pipeline
- [ ] Create releases with .bin files so people can install old versions

# Bug
- [x] Fix app crashing when writing tags
  - 0.1.36 ✅ (with and without mqtt)
    note: it did take a couple of tries and the tag needed to be on the reader when pushing the write button
  - 0.1.42 ❌ broken because I started trying to pass object into script

# Features

- [ ] Add nominal diameter to protocol
- [ ] Add 'weight' data to nfc tag
- [ ] Read bambu tags
- [ ] Read creality tags
- [ ] Support simultanious RFID tags on pn532

# Misc
- [ ] Enable status LED on s3 boards when mqtt connected
- [ ] Bambu support
- [ ] Octoprint support
- [ ] Klipper support