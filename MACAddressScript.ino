/*
Prints the UNO R4 WiFi's MAC address every 5 seconds.
- Uses WiFiS3 (ESP32-S3 coprocessor) to read the MAC
- Verifies WiFi module presence and shows firmware version
- Outputs to Serial at 9600 baud
*/
#include <WiFiS3.h>
void setup() {
// Start USB serial; Serial Monitor must be set to 9600
Serial.begin(9600);
// On native USB boards (like UNO R4), wait for Serial Monitor to open
while (!Serial) {
; // do nothing until serial is ready
}
// Ensure the WiFi coprocessor (ESP32-S3) is reachable
if (WiFi.status() == WL_NO_MODULE) {
Serial.println("Communication with WiFi module failed!");
while (true) {
// halt here if the WiFi module isn't detected
}
}
// Read WiFi firmware version (useful for debugging/updates)
String fv = WiFi.firmwareVersion();
// Compare against Arduino's latest-known version macro
if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
Serial.println("Please upgrade the firmware");
}
}
void loop() {
byte mac[6]; // buffer to hold 6 MAC bytes
// Fetch MAC address from the WiFi coprocessor
WiFi.macAddress(mac);
// Pretty-print the MAC in hex with colon separators
Serial.println();
Serial.print("Your MAC Address is: ");
printMacAddress(mac);
// Wait 5 seconds before printing again
delay(5000);
}
/*
printMacAddress
---------------
Prints a 6-byte MAC address as XX:XX:XX:XX:XX:XX with leading zeros.
*/
void printMacAddress(byte mac[]) {
for (int i = 0; i < 6; i++) {
if (i > 0) {
Serial.print(":"); // separator between bytes
}
if (mac[i] < 16) {
Serial.print("0"); // ensure two hex digits (leading zero)
}
Serial.print(mac[i], HEX); // print byte in hexadecimal
}
Serial.println();
}