// script.js - Smart Room Data Display

// Replace this with your published Google Sheet CSV URL
const csvUrl = 'https://docs.google.com/spreadsheets/d/e/2PACX-1vQ2v5fqXNJ3fSzRqGwhzyIuFL3Z8V52fnR3rY4uozvqn_LD5HQ-RRRZVCeRk77L3Wn7UhsNEVHhbVmR/pub?gid=0&single=true&output=csv';

async function loadData() {
  try {
    // Add a timestamp to bypass browser cache
    const response = await fetch(csvUrl + '&nocache=' + new Date().getTime());
    const csvText = await response.text();

    // Split into lines and headers
    const lines = csvText.trim().split('\n');
    const headers = lines[0].split(',');

    // Clear existing table body
    const tableBody = document.querySelector('#data-table tbody');
    tableBody.innerHTML = '';

    // Loop through each row of data
    for (let i = 1; i < lines.length; i++) {
      const row = lines[i].split(',');
      const tr = document.createElement('tr');

      row.forEach(cell => {
        const td = document.createElement('td');
        td.textContent = cell;
        tr.appendChild(td);
      });

      tableBody.appendChild(tr);
    }

  } catch (error) {
    console.error('Error loading CSV:', error);
  }
}

// Load data initially
loadData();

// Refresh data every 5 seconds (optional)
setInterval(loadData, 5000);
