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
