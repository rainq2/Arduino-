#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>        //LCD函式庫
#include <Button2.h>        //Button2函式庫



// Replace with your own Wi-Fi credentials
const char* ssid = "banana";
const char* password = "52695269";

// Replace with your own OpenWeatherMap API key
const char* apiKey = "e73c5ba692c77e42d1203639a936d79f";

//U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE);

const char* server = "api.openweathermap.org";
const char* city = "TAMSUI"; // Replace with the city you want to get weather information for

TFT_eSPI tft; // Declare the TFT object

// Function to fetch weather data from OpenWeatherMap API
String getWeatherData() {
  WiFiClient client;
  HTTPClient http;

  char url[100];
  sprintf(url, "/data/2.5/weather?q=%s&appid=%s", city, apiKey);

  if (!http.begin(client, server, 80, url)) {
    Serial.println("Failed to connect to OpenWeatherMap API");
    return "";
  }

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.println("HTTP request failed");
    http.end();
    return "";
  }

  String payload = http.getString();
  http.end();

  return payload;
}

void setup(void) {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3);  // 调整屏幕方向
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);

  // 连接 WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    tft.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  tft.println("Connected to WiFi!");

}

void loop(void) {
  if (WiFi.status() == WL_CONNECTED) {
    String payload = getWeatherData();

    // Parse the JSON data
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    JsonObject weather = doc["weather"][0];
    JsonObject main = doc["main"];
    float temperatureKelvin = main["temp"];
    int humidity = main["humidity"];
    String description = weather["description"];
    int rainProbability = doc["clouds"]["all"]; // Extract rain probability from "clouds" object

    // Convert temperature from Kelvin to Celsius
    float temperatureCelsius = temperatureKelvin - 273.15;

    // Display weather data on the OLED screen
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(4);
    tft.setCursor(10, 5);
    tft.print("T: ");
    tft.print(temperatureCelsius, 1); // Display temperature with one decimal place
    tft.print(" C");

    tft.setCursor(10, 40);
    tft.print("H: ");
    tft.print(humidity);
    tft.print(" %");
/*
    tft.setCursor(10, 70);
    tft.print("Weather: ");
    tft.print(description);*/

    tft.setCursor(10, 80);
    tft.print("R: ");
    tft.print(rainProbability);
    tft.print(" %");

    // Wait for a few seconds before updating the display again
    delay(10000);
  }
}

