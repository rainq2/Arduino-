#include  <SoftwareSerial.h>
SoftwareSerial Blue(0, 1); // RX | TX
long int data;
int LED=10;
long int password1 = 92;
long int password2 = 79;

char state = 0;

void setup()
{
  pinMode(LED, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(LED, LOW);
  Serial.begin(9600);
  Blue.begin(38400);
}
void loop()
{
  while(Blue.available() == 0);
  if (Blue.available()>0){
    data = Blue.parseInt();
  }
  delay(100);

  if(data == password1){
    digitalWrite(LED,HIGH);
    Serial.println("LED ON");
  }
  if(data == password2){
    digitalWrite(LED,LOW);
    Serial.println("LED OFF");
  }
}