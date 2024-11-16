---
title: Tag Info
layout: default
nav_order: 3
custom_js: true
---

<div class="spool-container">
  <div id="colorBox" class="color-circle"></div>
  <div id="hexValue" class="hex-value"></div>

  <div class="spool-data">
    <h2>Filament Properties</h2>
    <div class="data-grid">
      <div class="data-item">
        <span class="label">Protocol</span>
        <span class="value" id="protocol"></span>
      </div>
      <div class="data-item">
        <span class="label">Version</span>
        <span class="value" id="version"></span>
      </div>
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
  </div>
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