#include <ESP8266WiFi.h>
const char *ssid = "Bananananana";
const char *password = "52695269";


void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.print('n');
  WiFi.softAP(ssid, password);
  Serial.print("Access Point ");
  Serial.print(ssid);
  Serial.println(" started" );
  Serial.print("IP address:");
  Serial.println(WiFi.softAPIP());

}

void loop() {
  // put your main code here, to run repeatedly:

}
