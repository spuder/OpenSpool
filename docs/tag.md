---
title: Tag Info
layout: default
custom_js: true
permalink: /tag_info
---

<div class="spool-container">
  <div id="noParamsMessage" class="message-banner" style="display: none;">
    <div class="message-content">
      <span class="message-icon">🔍</span>
      <span class="message-text">Touch your phone to an NFC tag to view data</span>
    </div>
  </div>
  <div id="colorBox" class="color-circle"></div>
  <div id="hexValue" class="hex-value"></div>

  <div class="spool-data">
    <h2>Filament Properties</h2>
    <div class="data-grid">
      <div class="data-item">
        <span class="label">Type</span>
        <span class="value" id="type"></span>
      </div>
      <div class="data-item">
        <span class="label">Brand</span>
        <span class="value" id="brand"></span>
      </div>
      <div class="data-item">
        <span class="label">Temperature Range</span>
        <span class="value" id="temp-range"></span>
      </div>
    </div>
    <h2>Tag Properties</h2>
      <div class="data-grid">
        <div class="data-item">
          <span class="label">Protocol</span>
          <span class="value" id="protocol"></span>
        </div>
        <div class="data-item">
           <span class="label">Version</span>
          <span class="value" id="version"></span>
        </div>
      </div>
  </div>

  <div class="json-display">
    <h2>Raw JSON Data</h2>
    <pre id="jsonDisplay" class="json-code"></pre>
  </div>

  <!-- <div class="qr-container">
    <h2>QR Code</h2>
    <div id="qrcode"></div>
    <p class="qr-hint">Scan to share these filament settings</p>
  </div> -->
</div>

<style>
.color-display {
  display: flex;
  flex-direction: column;
  align-items: center;
  margin-bottom: 2rem;
}

.hex-value {
  margin-top: 1rem;
  font-family: monospace;
  font-size: 1.2em;
  font-weight: bold;
  color: #64B5F6;
  text-transform: uppercase;
}

.spool-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 2rem;
  max-width: 800px;
  margin: 0 auto;
}

.color-circle {
  width: 200px;
  height: 200px;
  border-radius: 50%;
  border: 4px solid #fff;
  box-shadow: 0 4px 8px rgba(0,0,0,0.1);
  transition: transform 0.3s ease;
}

.color-circle:hover {
  transform: scale(1.05);
}

.spool-data {
  width: 100%;
  text-align: center;
}

.spool-data h2 {
  margin-bottom: 2rem;
  font-size: 2em;
  color: #2196F3;
}

.data-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 1.5rem;
  padding: 1rem;
  background: rgba(255,255,255,0.05);
  border-radius: 10px;
}

.data-item {
  display: flex;
  flex-direction: column;
  padding: 1rem;
  background: rgba(255,255,255,0.03);
  border-radius: 8px;
  transition: transform 0.2s ease;
}

.data-item:hover {
  transform: translateY(-2px);
}

.label {
  font-size: 0.9em;
  text-transform: uppercase;
  letter-spacing: 1px;
  margin-bottom: 0.5rem;
  color: #64B5F6;
}

.value {
  font-size: 1.2em;
  font-weight: bold;
}

.json-display {
  width: 100%;
  max-width: 800px;
  margin-top: 2rem;
  text-align: left;
}

.json-display h2 {
  text-align: center;
  color: #2196F3;
  margin-bottom: 1rem;
}

.json-code {
  background: rgba(255,255,255,0.05);
  border-radius: 10px;
  padding: 1.5rem;
  overflow-x: auto;
  font-family: monospace;
  font-size: 1.1em;
  line-height: 1.5;
  border: 1px solid rgba(100,181,246,0.2);
  box-shadow: 0 4px 8px rgba(0,0,0,0.1);
}
/* .qr-container {
  width: 100%;
  max-width: 800px;
  margin-top: 2rem;
  text-align: center;
}

.qr-container h2 {
  color: #2196F3;
  margin-bottom: 1rem;
}

#qrcode {
  display: flex;
  justify-content: center;
  margin: 1rem auto;
  padding: 1rem;
  background: rgba(255,255,255,0.05);
  border-radius: 10px;
  box-shadow: 0 4px 8px rgba(0,0,0,0.1);
}

.qr-hint {
  color: #64B5F6;
  font-size: 0.9em;
  margin-top: 0.5rem;
} */

.message-banner {
  width: 100%;
  background: rgba(33, 150, 243, 0.1);
  border: 1px solid #2196F3;
  border-radius: 8px;
  margin-bottom: 2rem;
  padding: 1rem;
  text-align: center;
  animation: fadeIn 0.5s ease-in;
}

.message-content {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0.5rem;
}

.message-icon {
  font-size: 1.5em;
}

.message-text {
  color: #2196F3;
  font-size: 1.1em;
}

@keyframes fadeIn {
  from { opacity: 0; transform: translateY(-10px); }
  to { opacity: 1; transform: translateY(0); }
}
</style>

<script>
document.addEventListener('DOMContentLoaded', function() {
  const params = new URLSearchParams(window.location.search);
  const spoolData = {
    protocol: params.get('protocol'),
    version: params.get('version'),
    type: params.get('type'),
    color_hex: params.get('color_hex'),
    brand: params.get('brand'),
    min_temp: params.get('min_temp'),
    max_temp: params.get('max_temp')
  };
  
  if (spoolData.color_hex) {
    const colorBox = document.getElementById('colorBox');
    let color = spoolData.color_hex;
    if (/^[0-9A-Fa-f]{6}$/.test(color)) {
      color = '#' + color;
    }
    colorBox.style.backgroundColor = color;
  }

  // Update the individual fields
  document.getElementById('protocol').textContent = spoolData.protocol || 'N/A';
  document.getElementById('version').textContent = spoolData.version || 'N/A';
  document.getElementById('type').textContent = spoolData.type || 'N/A';
  document.getElementById('brand').textContent = spoolData.brand || 'N/A';
  document.getElementById('temp-range').textContent = 
    `${spoolData.min_temp || 'N/A'}°C - ${spoolData.max_temp || 'N/A'}°C`;
});


</script>

<!-- <script src="https://cdnjs.cloudflare.com/ajax/libs/qrcodejs/1.0.0/qrcode.min.js"></script> -->
<script type="module" src="https://unpkg.com/esp-web-tools@8.0.6/dist/web/install-button.js"></script>
<script src="{{ '/assets/js/tag-handler.js' | relative_url }}"></script>