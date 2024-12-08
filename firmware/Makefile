
# ls /dev/cu*
# export USB_ADDRESS=/dev/cu.usbmodem14301
USB_ADDRESS ?= $(error USB_ADDRESS is not set)

.PHONY: all clean build upload dashboard logs

all: build upload dashboard logs

# Preserve backwards compatabliity in documentation
# run-usb: run-usb-breadboard-mini
# run-usb-pcb: run-usb-pcb-mini
# build-pcb-mini: build-esp32s2-mini
# build-pcb-ams: build-esp32s2-ams

clean:
	esphome clean lolin_s2_mini.yaml
	esphome clean lolin_s3_mini.yaml
	esphome clean esp32-s3-devkitc-1.yaml
	rm -rf .esphome/idedata/*
	rm -rf ~/.platformio/penv
# build-esp32s2-mini:
# 	esphome config openspool-mini.yaml
# 	esphome compile openspool-mini.yaml
# build-esp32s2-ams:
# 	esphome config openspool-ams.yaml
# 	esphome compile openspool-ams.yaml
# build-d1-mini-s2-breadboard:
# 	esphome \
# 		-s version dev \ 
# 		-s spi2_miso_pin "GPIO39" \
# 		-s spi2_clk_pin "GPIO36" \
# 		-s spi2_mosi_pin "GPIO35" \
# 		-s rfid0_ss_pin "GPIO33" \
# 		-s spi2_type "any" \
# 		config openspool-mini.yaml
# 	esphome \
# 		-s version dev \ 
# 		-s spi2_miso_pin "GPIO39" \
# 		-s spi2_clk_pin "GPIO36" \
# 		-s spi2_mosi_pin "GPIO35" \
# 		-s rfid0_ss_pin "GPIO33" \
# 		-s spi2_type "any" \
# 		compile openspool-mini.yaml
# upload-mini:
# 	@echo "Uploading to $(USB_ADDRESS)"
# 	esphome upload openspool-mini.yaml --device $(USB_ADDRESS)
# upload-ams:
# 	@echo "Uploading to $(USB_ADDRESS)"
# 	esphome upload openspool-ams.yaml --device $(USB_ADDRESS)
# logs-mini:
# 	@echo "Uploading to $(USB_ADDRESS)"
# 	esphome logs openspool-mini.yaml --device $(USB_ADDRESS)
dashboard:
	esphome dashboard . --open-ui --address 127.0.0.1 --port 6053
# run-usb-breadboard-mini:
# 	esphome \
# 		-s spi2_miso_pin "GPIO39" \
# 		-s spi2_clk_pin "GPIO36" \
# 		-s spi2_mosi_pin "GPIO35" \
# 		-s rfid0_ss_pin "GPIO33" \
# 		-s spi2_type "any" \
# 		run openspool-mini.yaml --device $(USB_ADDRESS)
esp32-s3-devkitc-1: 
	esphome run esp32-s3-devkitc-1.yaml --device $(USB_ADDRESS)
lolin_s2_mini:
	esphome \
		run lolin_s2_mini.yaml --device $(USB_ADDRESS)
lolin_s3_mini:
	esphome \
		run lolin_s3_mini.yaml --device $(USB_ADDRESS)
devkit:
	esphome \
		run esp32-s3-devkitc-1.yaml --device $(USB_ADDRESS)
host:
	esphome run host.yaml
