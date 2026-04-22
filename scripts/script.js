// script.js - Fixed column-based version (NO phantom columns)

const csvUrl = 'https://docs.google.com/spreadsheets/d/e/2PACX-1vQ2v5fqXNJ3fSzRqGwhzyIuFL3Z8V52fnw3rY4uozvqn_LD5HQ-RRRZVCeRk77L3Wn7UhsNEVHhbVmR/pub?output=csv';

function loadData() {

  const url = csvUrl + '&nocache=' + new Date().getTime();

  Papa.parse(url, {
    download: true,
    skipEmptyLines: true,

    complete: function(results) {

      const tableBody = document.querySelector('#data-table tbody');
      tableBody.innerHTML = '';

      // Skip header row
      const rows = results.data.slice(1);

      rows.forEach(row => {

        const tr = document.createElement('tr');

        // FIXED COLUMN ORDER (matches your sheet)
        const cells = [
          row[0], // Timestamp
          row[1], // Temperature
          row[2], // Light
          row[3], // Sound
          row[4], // Occupied
          row[5], // Heater
          row[6]  // LED / Lights
        ];

        cells.forEach(val => {
          const td = document.createElement('td');
          td.textContent = val ?? '';
          tr.appendChild(td);
        });

        tableBody.appendChild(tr);
      });
    },

    error: function(err) {
      console.error('CSV Error:', err);
    }
  });
}

// Initial load
loadData();

// Refresh every 3 seconds
setInterval(loadData, 3000);
