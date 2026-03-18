const csvURL = "https://docs.google.com/spreadsheets/d/e/2PACX-1vQ2v5fqXNJ3fSzRqGwhzyIuFL3Z8V52fnw3rY4uozvqn_LD5HQ-RRRZVCeRk77L3Wn7UhsNEVHhbVmR/pub?gid=0&single=true&output=csv";

async function updateDashboard() {
  try {
    const response = await fetch(csvURL);
    const data = await response.text();

    // Split into rows
    const rows = data.trim().split("\n");
    // Last row is the latest reading
    const lastRow = rows[rows.length - 1].split(",");

    // Map values to your dashboard
    document.getElementById("temp").innerText = lastRow[1] + "°C";
    document.getElementById("light").innerText = lastRow[2];
    document.getElementById("occupancy").innerText = lastRow[3];
    document.getElementById("heater").innerText = lastRow[4];
    document.getElementById("lights").innerText = lastRow[5];

  } catch (error) {
    console.error("Error fetching data:", error);
  }
}

// Update every 5 seconds
setInterval(updateDashboard, 5000);
updateDashboard();  // Initial load