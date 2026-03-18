#include <WiFiS3.h>

char ssid[] ="IOT-MPSK";
char pass[] = "qpfwtnfr";

void setup(){
  Serial.begin(9600);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected!");
  Serial.println(WiFi.localIP());
}

void loop(){
  
}