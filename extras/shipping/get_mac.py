#!/usr/bin/env python3

import subprocess
import re
import sys
import qrcode
from PIL import Image, ImageDraw, ImageFont

def get_esp32_mac():
    try:
        result = subprocess.run(['esptool.py', 'read_mac'], capture_output=True, text=True, check=True)
        mac_addresses = re.findall(r'MAC: ([0-9A-Fa-f:]{17})', result.stdout)
        
        if len(mac_addresses) == 0:
            print("Error: No ESP32 device found.")
            sys.exit(1)
        elif len(mac_addresses) == 2 and mac_addresses[0] == mac_addresses[1]:
            return mac_addresses[0]  # Return the single MAC address if both are identical
        elif len(mac_addresses) > 1:
            print("Error: Multiple ESP32 devices detected. Please connect only one device.")
            sys.exit(1)
        
        return mac_addresses[0]
    except subprocess.CalledProcessError:
        print("Error: Failed to read MAC address. Make sure esptool.py is installed and the device is connected.")
        sys.exit(1)

def generate_qr_code_with_text(url):
    # Generate QR code
    qr = qrcode.QRCode(version=1, box_size=10, border=4)
    qr.add_data(url)
    qr.make(fit=True)
    qr_img = qr.make_image(fill_color="black", back_color="white")

    # Convert to RGB mode if it's not already
    if qr_img.mode != 'RGB':
        qr_img = qr_img.convert('RGB')

    # Set up font
    try:
        font = ImageFont.truetype("Arial.ttf", 20)
    except IOError:
        font = ImageFont.load_default()

    # Calculate text size
    draw = ImageDraw.Draw(qr_img)
    bbox = draw.textbbox((0, 0), url, font=font)
    text_width = bbox[2] - bbox[0]
    text_height = bbox[3] - bbox[1]

    # Create a new image with space for QR code and text
    padding = 20  # Padding around the QR code and text
    img_width = max(qr_img.width, text_width) + (padding * 2)
    img_height = qr_img.height + text_height + (padding * 3)  # Extra padding between QR and text
    img = Image.new('RGB', (img_width, img_height), color='white')

    # Paste QR code
    qr_x = (img_width - qr_img.width) // 2
    qr_y = padding
    img.paste(qr_img, (qr_x, qr_y))

    # Add text below QR code
    draw = ImageDraw.Draw(img)
    text_x = (img_width - text_width) // 2
    text_y = qr_y + qr_img.height + padding
    draw.text((text_x, text_y), url, font=font, fill="black")

    # Display the image
    img.show()

    # Optionally, save the image
    # img.save("qr_code_with_url.png")

def main():
    mac_address = get_esp32_mac()
    last_six_digits = mac_address.replace(':', '')[-6:]
    url = f"http://openspool-{last_six_digits.lower()}.local"
    print(f"Generated URL: {url}")
    
    generate_qr_code_with_text(url)
    print("QR code with URL text has been generated and displayed.")

if __name__ == "__main__":
    main()
