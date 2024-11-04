
# ls /dev/cu*
# export USB_ADDRESS=/dev/cu.usbmodem14301
USB_ADDRESS ?= $(error USB_ADDRESS is not set)

.PHONY: all clean build upload dashboard logs

all: build upload dashboard logs

# Preserve backwards compatabliity in documentation
run-usb: run-usb-breadboard

clean:
	esphome clean config-esp32.yaml
	esphome clean config-esp32s2.yaml
	rm -rf .esphome/idedata/*
	rm -rf ~/.platformio/penv
build-esp32:
	esphome config config-esp32.yaml
	esphome compile config-esp32.yaml
build-esp32s2:
	esphome config config-esp32s2.yaml
	esphome compile config-esp32s2.yaml
build-d1-mini-s2-breadboard:
	esphome \
		-s version dev \ 
		-s rfid1_miso_pin "GPIO39" \
		-s rfid1_clk_pin "GPIO36" \
		-s rfid1_mosi_pin "GPIO35" \
		-s rfid1_ss_pin "GPIO33" \
		-s rfid1_spi_interface "any" \
		config config-esp32s2.yaml
	esphome \
		-s version dev \ 
		-s rfid1_miso_pin "GPIO39" \
		-s rfid1_clk_pin "GPIO36" \
		-s rfid1_mosi_pin "GPIO35" \
		-s rfid1_ss_pin "GPIO33" \
		-s rfid1_spi_interface "any" \
		compile config-esp32s2.yaml
build-pcb:
	esphome \
		config config-esp32s2.yaml
	esphome \
		compile config-esp32s2.yaml
upload:
	@echo "Uploading to $(USB_ADDRESS)"
	esphome upload --device $(USB_ADDRESS) config-esp32s2.yaml
logs:
	@echo "Uploading to $(USB_ADDRESS)"
	esphome logs config-esp32s2.yaml --device $(USB_ADDRESS)
dashboard:
	esphome dashboard . --open-ui --address 127.0.0.1 --port 6053
run-usb-breadboard:
	# run-usb is used for the first time a device is setup 
	# uncomment the wifi section in config.yaml
	# then put the device into DFU mode (button pressed -> reset -> button release)
	# you can then run the run-ota command which will preserve the wifi settings
	# this is a workaround until eshome supports improv_serial on esp32s2
	esphome \
		-s version dev \
		-s rfid1_miso_pin "GPIO39" \
		-s rfid1_clk_pin "GPIO36" \
		-s rfid1_mosi_pin "GPIO35" \
		-s rfid1_ss_pin "GPIO33" \
		-s rfid1_spi_interface "any" \
		run config-esp32s2.yaml --device $(USB_ADDRESS)
	# podman run -v $PWD:/data -it ghcr.io/esphome/esphome:dev compile /data/config.yaml
	# podman run -v $PWD:/data --device=$(USB_ADDRESS) -it ghcr.io/esphome/esphome:dev run /data/config.yaml --device $(USB_ADDRESS)
run-usb-pcb:
	esphome \
		run config-esp32s2.yaml --device $(USB_ADDRESS)
run-ota:
	# run-ota is used for subsequent uploads after the device has been joined to the wifi network
	esphome \
		run config-esp32s2.yaml
