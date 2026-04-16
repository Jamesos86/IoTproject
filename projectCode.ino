#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include <math.h>

// ===============================
// WiFi Details
// ===============================
char ssid[] = "IOT-MPSK";
char password[] = "qpfwtnfr";

// ===============================
// Google Apps Script
// ===============================
const char serverAddress[] = "script.google.com";
const int port = 443;

String scriptPath =
"/macros/s/AKfycbzqT3z6C0uzpQDqmGLgVu-GSuabNbnNQuGdNcVgMqvibmage1rHN5ZzxiBh3osaCWlMbg/exec";

// ===============================
// WiFi / HTTP Client
// ===============================
WiFiSSLClient wifi;
HttpClient client(wifi, serverAddress, port);

// ===============================
// Pins
// ===============================
const int tempPin  = A0;
const int lightPin = A1;
const int soundPin = A2;

// Grove LED plugged into D4
const int ledPin   = 4;

// Relay plugged into D5
const int relayPin = 5;

// ===============================
// Thresholds
// ===============================
float tempLow = 18.0;
float tempHigh = 21.0;

int lightThreshold = 600;
int soundThreshold = 100;

// ===============================
// WiFi Connect
// ===============================
void connectWiFi() {

  Serial.print("Connecting to WiFi");

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(3000);
  }

  Serial.println();
  Serial.println("WiFi Connected!");
}

// ===============================
// Setup
// ===============================
void setup() {
  Serial.begin(9600);
  delay(2000);

  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin, LOW);

  connectWiFi();
}

// ===============================
// Loop
// ===============================
void loop() {

  // ---------------------------
  // Read Sensors
  // ---------------------------
  int tempRaw  = analogRead(tempPin);
  int lightRaw = analogRead(lightPin);
  int soundRaw = analogRead(soundPin);

  // Grove Temp Sensor v1.3
  float resistance = (1023.0 - tempRaw) * 10000.0 / tempRaw;
  float temperature =
    1 / (log(resistance / 10000.0) / 3975 + 1 / 298.15) - 273.15;

  bool occupied = (soundRaw > soundThreshold);

  // ---------------------------
  // Outputs
  // ---------------------------
  int heater = 0;
  int led = 0;

  // Heater control
  if (temperature < tempLow && occupied) {
    digitalWrite(relayPin, HIGH);
    heater = 1;
  } else if (temperature > tempHigh || !occupied) {
    digitalWrite(relayPin, LOW);
    heater = 0;
  }

  // LED control (real LED on Arduino / Grove LED)
  if (lightRaw < lightThreshold && occupied) {
    digitalWrite(ledPin, HIGH);
    led = 1;
  } else {
    digitalWrite(ledPin, LOW);
    led = 0;
  }

  // ---------------------------
  // Serial Output
  // ---------------------------
  Serial.print("Temp: ");
  Serial.println(temperature, 1);

  Serial.print("Light: ");
  Serial.println(lightRaw);

  Serial.print("Sound: ");
  Serial.println(soundRaw);

  Serial.print("Occupied: ");
  Serial.println(occupied);

  Serial.print("LED: ");
  Serial.println(led);

  Serial.print("Heater: ");
  Serial.println(heater);

  // ---------------------------
  // WiFi Check
  // ---------------------------
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost...");
    connectWiFi();
  }

  // ---------------------------
  // Send to Google Sheets
  // ---------------------------
  String url = scriptPath +
               "?temp=" + String(temperature, 1) +
               "&light=" + String(lightRaw) +
               "&sound=" + String(soundRaw) +
               "&occupied=" + String(occupied) +
               "&heater=" + String(heater) +
               "&led=" + String(led);

  client.get(url);

  unsigned long timeout = millis();

  while (client.connected() && !client.available()) {
    if (millis() - timeout > 8000) {
      client.stop();
      Serial.println("-------------------");
      delay(5000);
      return;
    }
  }

  // read response silently (no extra last line)
  client.responseStatusCode();
  client.responseBody();

  client.stop();

  Serial.println("-------------------");

  delay(5000);
}
