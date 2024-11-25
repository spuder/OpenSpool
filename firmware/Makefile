
# ls /dev/cu*
# export USB_ADDRESS=/dev/cu.usbmodem14301
USB_ADDRESS ?= $(error USB_ADDRESS is not set)

.PHONY: all clean build upload dashboard logs

all: build upload dashboard logs

# Preserve backwards compatabliity in documentation
run-usb: run-usb-breadboard-solo
run-usb-pcb: run-usb-pcb-solo
build-pcb-solo: build-esp32s2-solo
build-pcb-ams: build-esp32s2-ams

clean:
	esphome clean openspool-mini.yaml
	esphome clean openspool-ams.yaml
	rm -rf .esphome/idedata/*
	rm -rf ~/.platformio/penv
build-esp32:
	esphome config config-esp32.yaml
	esphome compile config-esp32.yaml
build-esp32s2-solo:
	esphome config openspool-mini.yaml
	esphome compile openspool-mini.yaml
build-esp32s2-ams:
	esphome config openspool-ams.yaml
	esphome compile openspool-ams.yaml
build-d1-mini-s2-breadboard:
	esphome \
		-s version dev \ 
		-s spi2_miso_pin "GPIO39" \
		-s spi2_clk_pin "GPIO36" \
		-s spi2_mosi_pin "GPIO35" \
		-s rfid0_ss_pin "GPIO33" \
		-s spi2_type "any" \
		config config-esp32s2.yaml
	esphome \
		-s version dev \ 
		-s spi2_miso_pin "GPIO39" \
		-s spi2_clk_pin "GPIO36" \
		-s spi2_mosi_pin "GPIO35" \
		-s rfid0_ss_pin "GPIO33" \
		-s spi2_type "any" \
		compile config-esp32s2.yaml
upload-solo:
	@echo "Uploading to $(USB_ADDRESS)"
	esphome upload openspool-mini.yaml --device $(USB_ADDRESS)
upload-ams:
	@echo "Uploading to $(USB_ADDRESS)"
	esphome upload openspool-ams.yaml --device $(USB_ADDRESS)
logs-solo:
	@echo "Uploading to $(USB_ADDRESS)"
	esphome logs config-esp32s2.yaml --device $(USB_ADDRESS)
dashboard:
	esphome dashboard . --open-ui --address 127.0.0.1 --port 6053
run-usb-breadboard-solo:
	esphome \
		-s spi2_miso_pin "GPIO39" \
		-s spi2_clk_pin "GPIO36" \
		-s spi2_mosi_pin "GPIO35" \
		-s rfid0_ss_pin "GPIO33" \
		-s spi2_type "any" \
		run openspool-mini.yaml --device $(USB_ADDRESS)
run-usb-pcb-solo:
	esphome \
		run openspool-mini.yaml --device $(USB_ADDRESS)
run-usb-pcb-ams:
	esphome \
		run openspool-ams.yaml --device $(USB_ADDRESS)
