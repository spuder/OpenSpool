---
title: Home
layout: home
---

# 📟 OpenSpool Firmware Installation

<div class="install-container">
    <script type="module" src="https://unpkg.com/esp-web-tools@8.0.6/dist/web/install-button.js"></script>

    <h2> 🔩 Install the firmware</h2>
    
    <div class="button-row">
        <esp-web-install-button 
            manifest="https://raw.githubusercontent.com/spuder/OpenSpool/refs/heads/main/firmware/manifest.json">
        </esp-web-install-button>
    </div>

    <div class="instructions">
        <h3>Installation Instructions</h3>
        <p>1. Connect your OpenSpool device via USB</p>
        <p>2. Press & Hold D0 Button</p>
        <p>3. Press RST Button</p>
        <p>4. Release both D0 and RST simultaneously</p>
        <p>5. Click the Install button above</p>
        <p>6. Follow the installation wizard</p>
        
        <p><strong>Note:</strong> Installation requires Chrome or Edge browser on desktop</p>
    </div>
</div>

# 🛠️ Configuration

Once the firmware is installed, You will need to join the RFID reader to the same network as your 3d printer. 


## 🛜 Wireless Configuration

1. Join the wifi network 'OpenSpool'
2. Navigate to [192.168.4.1](http://192.168.4.1)
3. Enter your wifi credentials
4. Reboot the RFID Reader


## 🖨️ Printer Configuration

Once openspool has joined your network, navigate to `openspool-xxXXxx.local` (where xxXXxx are the last 6 digits of the mac address). Or the ip address. 

{: .note }
Mac/Linux:  `arp -a| grep openspool`
Windows: `arp -a` 

Enter your Printer IP, LAN Access Code and Serial Number

![](./images/printersettings.png)

{: .note }
`Printer IP`, `Lan Access Code` and `Serial Number` are stored locally on the microcontroller and never leave the device. Do not share your Acces Code with anyone. 

----

[Just the Docs]: https://just-the-docs.github.io/just-the-docs/
[GitHub Pages]: https://docs.github.com/en/pages
[README]: https://github.com/just-the-docs/just-the-docs-template/blob/main/README.md
[Jekyll]: https://jekyllrb.com
[GitHub Pages / Actions workflow]: https://github.blog/changelog/2022-07-27-github-pages-custom-github-actions-workflows-beta/
[use this template]: https://github.com/just-the-docs/just-the-docs-template/generate