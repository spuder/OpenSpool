
# ls /dev/cu*
# export USB_ADDRESS=/dev/cu.usbmodem14301
USB_ADDRESS ?= $(error USB_ADDRESS is not set)

.PHONY: all clean build upload dashboard logs

all: build upload dashboard logs

clean:
	esphome clean config.yaml
	rm -rf .esphome/idedata/*
build:
	esphome config config.yaml
	esphome compile config.yaml
upload:
	@echo "Uploading to $(USB_ADDRESS)"
	esphome upload --device $(USB_ADDRESS) config.yaml
logs:
	@echo "Uploading to $(USB_ADDRESS)"
	esphome logs config.yaml --device $(USB_ADDRESS)
dashboard:
	esphome dashboard . --open-ui --address 127.0.0.1 --port 6053
run-usb:
	# run-usb is used for the first time a device is setup 
	# uncomment the wifi section in config.yaml
	# then put the device into DFU mode (button pressed -> reset -> button release)
	# you can then run the run-ota command which will preserve the wifi settings
	# this is a workaround until eshome supports improv_serial on esp32s2
	esphome \
		-s name openspool \
		-s friendly_name "OpenSpool" \
		run config.yaml --device $(USB_ADDRESS)
	# podman run -v $PWD:/data -it ghcr.io/esphome/esphome:dev compile /data/config.yaml
	# podman run -v $PWD:/data --device=$(USB_ADDRESS) -it ghcr.io/esphome/esphome:dev run /data/config.yaml --device $(USB_ADDRESS)
run-ota:
	# run-ota is used for subsequent uploads after the device has been joined to the wifi network
	esphome \
		-s name openspool \
		-s friendly_name "OpenSpool" \
		run config.yaml
