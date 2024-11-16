document.addEventListener('DOMContentLoaded', function() {
  const params = new URLSearchParams(window.location.search);
  
  // Handle legacy 'color' parameter for backwards compatibility
  const colorParam = params.get('color') || params.get('color_hex');
  
  // Set default values for all parameters
  const spoolData = {
    protocol: params.get('protocol'),
    version: params.get('version'),
    type: params.get('type'),
    color_hex: colorParam,
    brand: params.get('brand'),
    min_temp: params.get('min_temp'),
    max_temp: params.get('max_temp')
  };
  
  // Handle the color display
  const colorBox = document.getElementById('colorBox');
  if (colorBox && colorParam) {
    let color = colorParam;
    if (/^[0-9A-Fa-f]{6}$/.test(color)) {
      color = '#' + color;
    }
    colorBox.style.backgroundColor = color;
    hexValue.textContent = color.toUpperCase();
  } else {
    // Set default or placeholder values
    colorBox.style.backgroundColor = '#CCCCCC';
    hexValue.textContent = '?';
  }

  // Update the individual fields
  const updateElement = (id, value, suffix = '') => {
    const element = document.getElementById(id);
    if (element) {
      element.textContent = value ? value + suffix : '?';
    }
  };

  // Update all fields
  updateElement('protocol', spoolData.protocol);
  updateElement('version', spoolData.version);
  updateElement('type', spoolData.type);
  updateElement('brand', spoolData.brand);
  
  // Special handling for temperature range
  const tempRange = document.getElementById('temp-range');
  if (tempRange) {
    const minTemp = spoolData.min_temp ? `${spoolData.min_temp}°C` : '?';
    const maxTemp = spoolData.max_temp ? `${spoolData.max_temp}°C` : '?';
    tempRange.textContent = `${minTemp} - ${maxTemp}`;
  }

  // Show all fields, including temperature
  const dataItems = document.querySelectorAll('.data-item');
  dataItems.forEach(item => {
    item.style.display = 'flex';
  });
});