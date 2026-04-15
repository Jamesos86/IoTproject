#include <WiFi.h>
#include <HTTPClient.h>

// WiFi details
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// Google Script URL
String serverName = "https://script.google.com/macros/s/AKfycbzqT3z6C0uzpQDqmGLgVu-GSuabNbnNQuGdNcVgMqvibmage1rHN5ZzxiBh3osaCWlMbg/exec";

// Sensors
const int tempPin = A0;
const int lightPin = A1;
const int soundPin = A2;

// Outputs
const int ledPin = 4;
const int relayPin = 5;

// Thresholds
float tempLow = 18.0;
float tempHigh = 21.0;
int lightThreshold = 600;
int soundThreshold = 100;

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
}

void loop() {

  // Read sensors
  int tempRaw = analogRead(tempPin);
  int lightRaw = analogRead(lightPin);
  int soundRaw = analogRead(soundPin);

  float temperature = tempRaw * (5.0 / 1023.0) * 100.0;
  bool occupied = (soundRaw > soundThreshold);

  // Control logic
  int heater = 0;
  int led = 0;

  if (temperature < tempLow && occupied) {
    digitalWrite(relayPin, HIGH);
    heater = 1;
  } else if (temperature > tempHigh) {
    digitalWrite(relayPin, LOW);
    heater = 0;
  }

  if (lightRaw < lightThreshold && occupied) {
    digitalWrite(ledPin, HIGH);
    led = 1;
  } else {
    digitalWrite(ledPin, LOW);
    led = 0;
  }

  // Send to Google Sheets
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = serverName +
      "?temp=" + String(temperature) +
      "&light=" + String(lightRaw) +
      "&sound=" + String(soundRaw) +
      "&occupied=" + String(occupied) +
      "&heater=" + String(heater) +
      "&led=" + String(led);

    http.begin(url);
    int httpResponseCode = http.GET();

    Serial.println(httpResponseCode);
    http.end();
  }

  delay(5000);
}
