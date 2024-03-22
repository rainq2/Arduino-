#include <Wire.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <LiquidCrystal_PCF8574.h>
#include "DHT.h"
#include <Ultrasonic.h>
Ultrasonic ultrasonic(12, 13);
int distance;

ThreeWire myWire(6, 5, 7); // DAT, SCLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

#define DHTPIN 2
#define DHTTYPE DHT11
LiquidCrystal_PCF8574 lcd(0x27);
DHT dht(DHTPIN, DHTTYPE);

int s=0;

int sensor = 8;  //訊號線
int ledPin = 9;  // LED连接到数字引脚13

int displayMode = 0; // 0表示显示温度和湿度，1表示显示距离
int buttonPin = 10;


void setup ()
{
  Serial.begin(9600);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  Rtc.SetDateTime(compiled);

  if (!Rtc.IsDateTimeValid())
  {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected())
  {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
  Serial.println("DHTxx test!");
  dht.begin();  //初始化DHT

  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*~ first line.");
  lcd.setCursor(0, 1);
  lcd.print("~* second line.");
  
  pinMode(buttonPin, INPUT);

}

void loop (){
  if (digitalRead(buttonPin) == HIGH) {
  // 检测到按钮按下
  // 切换显示模式
    if (displayMode == 0) {
      displayMode = 1; // 切换到显示距离
    } 
    else{
      displayMode = 0; // 切换回显示温度和湿度
    }
  }

  /*if(s != 100){
    s++;
  }else{
    tem();
  }*/
  tem();
  sr04();
  sr501();
  Serial.println(displayMode);
}
void sr04(){
  distance = ultrasonic.read(); //不加參數就是輸出CM，可用read(INC)輸出英寸
  
  //Serial.print("Distance in CM: ");
  //Serial.println(distance);
}

void sr501(){

  int moving = digitalRead(sensor); //读取D9是否有侦测到物体移动
  if (moving == 1) { //如果有物体移动
    //Serial.println("有東西在動！");
    digitalWrite(ledPin, HIGH); // 点亮LED
  } else {
    digitalWrite(ledPin, LOW); // 关闭LED
  }
}

void tem() {
  RtcDateTime now = Rtc.GetDateTime();

  if ((now.Hour() >= 0 && now.Hour() < 8)) {
    lcd.setBacklight(0);
  } else {
    lcd.setBacklight(255);
  }

  if (!now.IsValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
  }

  if (displayMode == 0) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RH  :");
    lcd.setCursor(7, 0);
    lcd.print(h);
    lcd.setCursor(14, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.setCursor(7, 1);
    lcd.print(t);
    lcd.setCursor(13, 1);
    lcd.print((char)223);
    lcd.setCursor(14, 1);
    lcd.print("C");
  } else if (displayMode == 1) {
    // 根据距离显示内容
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance (cm):");
    lcd.setCursor(0, 1);
    lcd.print(distance);
  }

  delay(1000);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.print(datestring);
}
