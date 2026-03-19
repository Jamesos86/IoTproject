// script.js - Fetch and display CSV data from Google Sheet every 5 seconds using PapaParse
// Works regardless of header text by using column order

const csvUrl = 'https://docs.google.com/spreadsheets/d/e/2PACX-1vQ2v5fqXNJ3fSzRqGwhzyIuFL3Z8V52fnw3rY4uozvqn_LD5HQ-RRRZVCeRk77L3Wn7UhsNEVHhbVmR/pub?output=csv';

function loadData() {
  // Add timestamp to URL to prevent caching
  const url = csvUrl + '&nocache=' + new Date().getTime();

  Papa.parse(url, {
    download: true,
    header: true,       // Treat first row as header (optional)
    skipEmptyLines: true,
    complete: function(results) {
      const tableBody = document.querySelector('#data-table tbody');
      tableBody.innerHTML = ''; // Clear old rows

      results.data.forEach(row => {
        const tr = document.createElement('tr');

        // Use column order instead of header text
        const values = Object.values(row);  // get all cell values in order
        values.forEach(val => {
          const td = document.createElement('td');
          td.textContent = val || '';      // empty string if missing
          tr.appendChild(td);
        });

        tableBody.appendChild(tr);
      });
    },
    error: function(err) {
      console.error('Error parsing CSV:', err);
    }
  });
}

// Initial load
loadData();

// Refresh every 5 seconds
setInterval(loadData, 5000);
