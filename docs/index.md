---
title: Home
layout: home
nav_order: 1
---

# 📟 OpenSpool Firmware Installation

![](https://img.shields.io/badge/openspool-1.19.1-magenta)

<div class="install-container">
    <script type="module" src="https://unpkg.com/esp-web-tools@8.0.6/dist/web/install-button.js"></script>

    <h2> 🔩 Install the firmware</h2>
    
    <div class="button-row">
        <esp-web-install-button 
            manifest="https://openspool.io/firmware/manifest.json">
        <span slot="unsupported"> <b>Only <img src="https://img.shields.io/badge/Google%20Chrome-4285F4?style=for-the-badge&logo=GoogleChrome&logoColor=white" alt="Google Chrome"> / <img src="https://img.shields.io/badge/Edge-0078D7?style=for-the-badge&logo=Microsoft-edge&logoColor=white" alt="Edge"> Supported</b> </span>

        <span slot="not-allowed">🔒 HTTPS Required 🔒 </span>
        </esp-web-install-button>
    </div>

    <div class="instructions">
        <h3>Installation Instructions</h3>
        <p>1. Connect your OpenSpool device via USB</p>
        <p>2. Press & Hold D0 Button</p>
        <p>3. Press RST Button</p>
        <p>4. Release both D0 and RST simultaneously</p>
        <p>5. Click the blue 'Connect' button above</p>
        <p>6. Follow the installation wizard</p>
    </div>
</div>

{: .caution }
If the above step fails, see the [README on github](https://github.com/spuder/OpenSpool) to upload manually with command `make lolin_s2_mini` or `make lolin_s3_mini`. 

# 🛠️ Configuration

Once the firmware is installed, You will need to join the RFID reader to the same network as your 3d printer. 

## 🛜 Wireless Configuration

1. Join the wifi network `OpenSpool`
2. Navigate to [192.168.4.1](http://192.168.4.1)
3. Enter your wifi credentials (take note of the mac address, you will need it to connect)
4. Reboot the RFID Reader

![](./images/wifi1.png)

After reboot connect to http://openspool-d89be4.local (where d89be is your specific mac address)


## 🖨️ Printer Configuration

Once openspool has joined your network, navigate to `openspool-xxxxxx.local`, or ip address (where xxxxxx are the last 6 digits of the mac address).

If you forgot the mac address, or if you computer is unable to resolve `.local` domains. You can always connect via ip address. To find the ip address, consult your router documentation, or search your arp table. Instructions below. 

{: .note-title } 
> Finding IP Address via ARP Tables
>
> Mac/Linux:  `arp -a | grep openspool`  
> Windows: `arp -a` 


Enter your Printer IP, LAN Access Code and Serial Number

![](./images/printersettings.png)

{: .note-title }
> Security
>
> `Printer IP`, `Lan Access Code` and `Serial Number` are stored locally on the microcontroller and never leave the device. Do not share your Acces Code with anyone. 

