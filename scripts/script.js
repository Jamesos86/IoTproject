const csvURL = "https://docs.google.com/spreadsheets/d/e/2PACX-1vQ2v5fqXNJ3fSzRqGwhzyIuFL3Z8V52fnw3rY4uozvqn_LD5HQ-RRRZVCeRk77L3Wn7UhsNEVHhbVmR/pub?gid=0&single=true&output=csv";

async function updateDashboard() {
  try {
    const response = await fetch(csvURL);
    const data = await response.text();

    // Split into rows
    const rows = data.trim().split("\n");
    // Last row is the latest reading
    const lastRow = rows[rows.length - 1].split(",");

    document.getElementById("temp").innerText = lastRow[1].trim() + "°C";
    document.getElementById("light").innerText = lastRow[2].trim();
    document.getElementById("occupancy").innerText = lastRow[3].trim();
    document.getElementById("heater").innerText = lastRow[4].trim();
    document.getElementById("lights").innerText = lastRow[5].trim();

  } catch (error) {
    console.error("Error fetching data:", error);
  }
}

// Update every 5 seconds
setInterval(updateDashboard, 5000);
updateDashboard();  // Initial load
