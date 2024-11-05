---
title: Home
layout: home
---

# 📟 OpenSpool Firmware Installation

![](https://img.shields.io/badge/openspool-0.1.25-magenta)

<div class="install-container">
    <script type="module" src="https://unpkg.com/esp-web-tools@8.0.6/dist/web/install-button.js"></script>

    <h2> 🔩 Install the firmware</h2>
    
    <div class="button-row">
        <esp-web-install-button 
            manifest="https://raw.githubusercontent.com/spuder/OpenSpool/refs/heads/main/artifacts/manifest.json">
        <span slot="unsupported">🛑  <b>Only <img src="https://img.shields.io/badge/Google%20Chrome-4285F4?style=for-the-badge&logo=GoogleChrome&logoColor=white" alt="Google Chrome"> / <img src="https://img.shields.io/badge/Edge-0078D7?style=for-the-badge&logo=Microsoft-edge&logoColor=white" alt="Edge"> Supported</b> 🛑</span>

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
If the above step fails, see the [README on github](https://github.com/spuder/OpenSpool) to upload manually with command `make run-usb`. 

# 🛠️ Configuration

Once the firmware is installed, You will need to join the RFID reader to the same network as your 3d printer. 


## 🛜 Wireless Configuration

1. Join the wifi network `OpenSpool`
2. Navigate to [192.168.4.1](http://192.168.4.1)
3. Enter your wifi credentials
4. Reboot the RFID Reader


## 🖨️ Printer Configuration

Once openspool has joined your network, navigate to `openspool-xxxxxx.local`, or ip address (where xxxxxx are the last 6 digits of the mac address).

{: .note-title } 
> Finding IP Address
>
> Mac/Linux:  `arp -a| grep openspool`  
> Windows: `arp -a` 

  

Enter your Printer IP, LAN Access Code and Serial Number

![](./images/printersettings.png)

{: .note-title }
> Security
>
> `Printer IP`, `Lan Access Code` and `Serial Number` are stored locally on the microcontroller and never leave the device. Do not share your Acces Code with anyone. 

----

[Just the Docs]: https://just-the-docs.github.io/just-the-docs/
[GitHub Pages]: https://docs.github.com/en/pages
[README]: https://github.com/just-the-docs/just-the-docs-template/blob/main/README.md
[Jekyll]: https://jekyllrb.com
[GitHub Pages / Actions workflow]: https://github.blog/changelog/2022-07-27-github-pages-custom-github-actions-workflows-beta/
[use this template]: https://github.com/just-the-docs/just-the-docs-template/generate