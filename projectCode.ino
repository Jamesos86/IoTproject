// Grove Smart Room Demo - Local Only

#include <math.h>

// Grove analog sensors
const int tempPin = A0;    
const int lightPin = A1;   
const int soundPin = A2;   

// Grove digital outputs
const int ledPin = 4;      
const int relayPin = 5;    

// Thresholds
float tempLow = 18.0;      
float tempHigh = 21.0;     
int lightThreshold = 600;  
int soundThreshold = 100;  

// Thermistor constant
const int B = 3975;

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  #include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

String sheetURL = "YOUR_WEB_APP_URL_HERE";

void sendData(float temperature, int lightRaw, bool occupied, bool heaterStatus, bool lightsStatus) {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    String url = sheetURL + "?temperature=" + String(temperature) 
                 + "&light=" + String(lightRaw)
                 + "&occupancy=" + (occupied ? "Yes" : "No")
                 + "&heater=" + (heaterStatus ? "ON" : "OFF")
                 + "&lights=" + (lightsStatus ? "ON" : "OFF");
    http.begin(url);
    int code = http.GET();  // For GET method, POST can also be used
    http.end();
  }
}
  // ===== Read sensors =====
  int tempRaw = analogRead(tempPin);
  int lightRaw = analogRead(lightPin);
  int soundRaw = analogRead(soundPin);

  // ===== Temperature calculation (Thermistor) =====
  float resistance = (1023.0 - tempRaw) * 10000.0 / tempRaw;
  float temperature = 1 / (log(resistance / 10000.0) / B + 1 / 298.15) - 273.15;

  bool occupied = (soundRaw > soundThreshold);

  // ===== Heater control =====
  bool heaterOn = false;
  if (temperature < tempLow && occupied) {
    digitalWrite(relayPin, HIGH);
    heaterOn = true;
  } else if (temperature > tempHigh) {
    digitalWrite(relayPin, LOW);
    heaterOn = false;
  }

  // ===== Light control =====
  bool lightOn = false;
  if (lightRaw < lightThreshold && occupied) {
    digitalWrite(ledPin, HIGH);
    lightOn = true;
  } else {
    digitalWrite(ledPin, LOW);
    lightOn = false;
  }

  // ===== Clean Serial Output =====
  Serial.println("----- Smart Room Status -----");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Light Level: ");
  Serial.println(lightRaw);

  Serial.print("Sound Level: ");
  Serial.println(soundRaw);

  Serial.print("Occupied: ");
  Serial.println(occupied ? "Yes" : "No");

  Serial.print("Heater: ");
  Serial.println(heaterOn ? "ON" : "OFF");

  Serial.print("Lights: ");
  Serial.println(lightOn ? "ON" : "OFF");

  Serial.println("-----------------------------\n");

  delay(500);
}
