/*
紅外線感應
out接9
3.3 或 5V都可以
led長的接8
oled sda-a4 scl-a5
 */
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
int sensor = 9;  //訊號線
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); //Arduino搭配SSD1306(0.96" OLED)用這行

void setup() {                
  Serial.begin(9600); 
  pinMode(sensor, INPUT); 
  pinMode(8, OUTPUT);
  u8g2.begin();
}

void loop() {
  
  int moving = digitalRead(sensor); //讀取D9是否有偵測到物體移動
  u8g2.setFont(u8g2_font_HelvetiPixelOutline_te); //設定字型
  if(moving==1){ //如果有物體移動
   u8g2.firstPage();
    do {
     u8g2.drawStr(0,13,"moving!");  //輸出文字
     Serial.println("mov");  
     digitalWrite(8, HIGH); 
    } while ( u8g2.nextPage() );
  }
  if(moving==0){ 
   u8g2.firstPage();
    do {
     u8g2.drawStr(0,13,"no moving!");  //輸出文字
     Serial.println("no mov");  
     digitalWrite(8, LOW);
    } while ( u8g2.nextPage() );
  }
}