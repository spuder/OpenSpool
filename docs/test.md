---
title: Test
layout: default
custom_js: true
permalink: /test
---


<div class="install-container">
    <script type="module" src="https://unpkg.com/esp-web-tools@8.0.6/dist/web/install-button.js"></script>

    <h2> 🔩 Install the firmware</h2>
    
    <div class="button-row">
        <esp-web-install-button 
            manifest="https://raw.githubusercontent.com/spuder/OpenSpool/refs/heads/main/firmware/manifest.json">
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
