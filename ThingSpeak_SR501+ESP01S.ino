#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> //下達HTTP GET用


int test_data; //存放類比腳位讀到的測資
WiFiClient client;
WiFiClientSecure Secure_client;

const char ssid[]     = "Bananananana";// change to your WIFI SSID
const char password[] = "52695269";// change to your WIFI Password


void read_data() {  
   test_data = digitalRead(2);
   if(test_data==0){ //如果有物體移動
    Serial.println("no moving");  
  }else{
    Serial.println("moving");    
  }
}

void uploadThingSpeak() {
  if(client.connect("api.thingspeak.com", 80)) {
  String  things_request = String("UA2DTQIDFQDKU30J")+"&field1="+ String(test_data)+ "\r\n\r\n";
   client.println("POST /update HTTP/1.1");
   client.println("Host: api.thingspeak.com");
   client.println("Connection: close");
   client.println("X-THINGSPEAKAPIKEY: UA2DTQIDFQDKU30J");
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
        Secure_client.println("GET /forms/d/e/1FAIpQLSfAaVqwoRRxx2VmgCB9qs4QT0xXybZAGhTqEHKIk31vEvQRww/formResponse?entry.1516989552="+ String(test_data)+"&submit=Submit HTTP/1.1");
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
   test_data = 0;
   delay(10);
  // We start by connecting to a WiFi network
   Serial.println();
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);
   WiFi.begin(ssid,password);
   while(WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.print(".");
   } //end of while()

   Serial.println("");
   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
} //end of setup()

void loop() {
    read_data();
    uploadThingSpeak();
    upload2GoogleForms();
    delay(60000);
} //end of loop()