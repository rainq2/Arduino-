#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> //下達HTTP GET用
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int h;
int t; //存放類比腳位讀到的測資
WiFiClient client;
WiFiClientSecure Secure_client;


void read_data() {  
  h = dht.readHumidity();   //取得濕度
  t = dht.readTemperature();  //取得溫度C

  //顯示在監控視窗裡
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
}

void uploadThingSpeak() {
    if(client.connect("api.thingspeak.com", 80)) {
  String  things_request = String("ZXPHSTZRE3OX3PLA")+"&field1="+ String(h)+"&field2="+ String(t)+"\r\n\r\n";
   client.println("POST /update HTTP/1.1");
   client.println("Host: api.thingspeak.com");
   client.println("Connection: close");
   client.println("X-THINGSPEAKAPIKEY: ZXPHSTZRE3OX3PLA");
   client.println("Content-Type: application/x-www-form-urlencoded");
   client.print("Content-Length: ");
   client.println(things_request.length());
   client.println();
   client.print(things_request);
   delay(1000);
   client.stop();
  }
  Serial.println("Sent...");
} //end of upload2GoogleForms()

void upload2GoogleForms() {
    Secure_client.setInsecure(); //[2]
    if (Secure_client.connect("docs.google.com", 443)) {
        Secure_client.println("GET /forms/d/e/1FAIpQLSeM0iUSgjUEa-bl78wvUdoHAdKu2BvXVtMruB4GhDDy0JvM0g/formResponse?entry.1400018429="+ String(h)+"&entry.1291160928="+String(t)+"&submit=Submit HTTP/1.1");
        Secure_client.println("Host: docs.google.com");
        Secure_client.println("Connection: close");
        Secure_client.println();
        Secure_client.stop();
   } else { //Google returned 0
      Serial.println("Google said no!");
   } //end of if-else
} //end of upload2GoogleForms()

void setup() {
   pinMode(2, INPUT);
   Serial.begin(9600);
   dht.begin();  //初始化DHT
   h = 0;
   t = 0;
   WiFi.begin("Bananananana","52695269");
   while(WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.print(".");
   } //end of while()

   Serial.println(WiFi.localIP());

} //end of setup()

void loop() {
    read_data();
    uploadThingSpeak();
    upload2GoogleForms();
    delay(60000);
} //end of loop()