#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <TFT_eSPI.h>  // LCD函式庫
#include <NTPClient.h> // NTP客戶端函式庫
#include <WiFiUdp.h>   // WiFi UDP函式庫

const char* ssid = "Banana";
const char* password = "52695269";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset = 28800; // GMT+8時區，以秒為單位
const int daylightOffset = 0; // 不使用夏令時

TFT_eSPI tft; // 宣告TFT物件

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset, daylightOffset);

void setup(void) {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);  // 調整屏幕方向
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);

  // 連接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    tft.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  tft.println("Connected to WiFi!");

  // 初始化NTP客戶端
  timeClient.begin();
}

void loop(void) {
  // 更新NTP時間
  timeClient.update();

  // 獲取當前時間
  //String currentTime = timeClient.getFormattedTime();
   // 獲取當前時間（不包括秒數）
  String currentTime = timeClient.getFormattedTime().substring(0, 5); // 提取小時和分鐘部分

  // 顯示時間在TFT顯示屏上 時 分 秒
  /*tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(10, 5);
  //tft.print("Time:");
  tft.setCursor(10, 40);
  tft.print(currentTime);*/

  // 顯示時間在TFT顯示屏上 時 分
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(8); // 調整時間文字大小
  tft.setCursor(10, 37);
  tft.print(currentTime);

  // 等待幾秒後再次更新顯示
  delay(40000);
}

