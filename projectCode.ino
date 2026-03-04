// Grove Smart Room Demo - Local Only

// Grove analog sensors
const int tempPin = A0;    // Temperature sensor (LM35-style)
const int lightPin = A1;   // Light sensor
const int soundPin = A2;   // Sound sensor / occupancy

// Grove digital outputs
const int ledPin = 4;      // D4 - LED simulating lights
const int relayPin = 5;    // D5 - Relay simulating heater

// Thresholds (adjust after testing)
float tempLow = 18.0;      // Turn heater ON if below 18°C
float tempHigh = 21.0;     // Turn heater OFF if above 21°C
int lightThreshold = 600;  // Adjust based on room lighting
int soundThreshold = 100;  // Reduced for demo

//==// Define the B-value of the thermistor.
// This value is a property of the thermistor used in the Grove - Temperature Sensor,
// and used to convert from the analog value it measures and a temperature value.
  const int B = 3975;
//===

void setup() {
  Serial.begin(9600);       // Serial monitor

  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  // ===== Read sensors =====
  int tempRaw = analogRead(tempPin);   // 0–1023
  int lightRaw = analogRead(lightPin);
  int soundRaw = analogRead(soundPin);

// =======Temperature calculation======
// Determine the current resistance of the thermistor based on the sensor value.
  float resistance = (float)(1023-tempRaw)*10000/tempRaw;
// Calculate the temperature based on the resistance value.
  float temperature = 1/(log(resistance/10000)/B+1/298.15)-273.15;
//

  bool occupied = (soundRaw > soundThreshold);

  // ===== Print sensor readings =====
  Serial.print("Temperature (°C): "); Serial.println(temperature);
  Serial.print("Light Level: "); Serial.println(lightRaw);
  Serial.print("Sound Level: "); Serial.println(soundRaw);
  Serial.print("Occupied: "); Serial.println(occupied ? "Yes" : "No");
  Serial.println("--------");

  // ===== Heater control =====
  if (temperature < tempLow && occupied) {
    digitalWrite(relayPin, HIGH);  // Heater ON
  } else if (temperature > tempHigh) {
    digitalWrite(relayPin, LOW);   // Heater OFF
  }

  // ===== Light control =====
  if (lightRaw < lightThreshold && occupied) {
    digitalWrite(ledPin, HIGH);    // Lights ON
  } else {
    digitalWrite(ledPin, LOW);     // Lights OFF
  }

  delay(500);  // Small delay for responsive demo
}