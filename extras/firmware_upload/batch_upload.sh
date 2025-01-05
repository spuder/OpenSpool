#!/bin/bash

FIRMWARE_PATH="../../docs/firmware/openspool-esp32s3.factory.bin"
DEVICE_PREFIX="/dev/cu.usbmodem"

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

print_red() {
    echo -e "${RED}$1${NC}"
}

print_green() {
    echo -e "${GREEN}$1${NC}"
}

flash_device() {
    local device=$1
    echo "Flashing device: $device"
    esptool.py --chip esp32-s3 -p "$device" --baud 921600 --before default_reset --after hard_reset write_flash 0x0 "$FIRMWARE_PATH"
    if [ $? -eq 0 ]; then
        print_green "Flashing successful: $device"
    else
        print_red "Flashing failed: $device"
    fi
}

watch_for_devices() {
    local old_devices=""
    while true; do
        local new_devices=$(ls ${DEVICE_PREFIX}* 2>/dev/null)
        for device in $new_devices; do
            if [[ ! $old_devices =~ $device ]]; then
                echo "New device detected: $device"
                flash_device "$device"
            fi
        done
        old_devices="$new_devices"
        sleep 1
    done
}

echo "Starting ESP32-S3 flashing script"
echo "Firmware path: $FIRMWARE_PATH"
echo "Watching for devices with prefix: $DEVICE_PREFIX"
watch_for_devices
