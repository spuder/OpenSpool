document.addEventListener('DOMContentLoaded', function() {
  const params = new URLSearchParams(window.location.search);
  const noParamsMessage = document.getElementById('noParamsMessage');
  let spoolData = {};
  let colorParam;

  if (params.toString() === '') {
    // No parameters case
    noParamsMessage.style.display = 'block';
  } else {
    // Parameters exist
    noParamsMessage.style.display = 'none';
    colorParam = params.get('color_hex');
    
    // Set spoolData
    spoolData = {
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
    const hexValue = document.getElementById('hexValue');
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

    // Show raw json data
    const jsonDisplay = document.getElementById('jsonDisplay');
    if (jsonDisplay) {
      const prettyJson = JSON.stringify(spoolData, null, 2)
        .replace(/null/g, '"?"');
      jsonDisplay.textContent = prettyJson;
    }

    // Show url display
    const urlNdefDisplay = document.getElementById('urlNdefDisplay');
    if (urlNdefDisplay) {
        // Create URL with parameters
        const url = new URL('https://openspool.io/tag_info');
        
        // Only add parameters that exist and have values
        Object.entries(spoolData).forEach(([key, value]) => {
            if (value) {
                url.searchParams.set(key, value);
            }
        });

        // Set the URL text content and make it visible
        urlNdefDisplay.textContent = url.toString();
        urlNdefDisplay.style.display = 'block';  // Make sure it's visible
        
        // Optional: Add a class for styling
        urlNdefDisplay.classList.add('url-display');
    }
    // Generate QR code
    // try {
    //   const qrcodeContainer = document.getElementById('qrcode');
    //   if (qrcodeContainer) {
    //     qrcodeContainer.innerHTML = '';
    //     new QRCode(qrcodeContainer, {
    //       text: JSON.stringify(spoolData),
    //       width: 256,
    //       height: 256,
    //       colorDark: "#64B5F6",
    //       colorLight: "#ffffff",
    //       correctLevel: QRCode.CorrectLevel.H
    //     });
    //   }
    // } catch (error) {
    //   console.error('Error generating QR code:', error);
    // }
  }
});