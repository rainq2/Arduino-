#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


// Replace with your own Wi-Fi credentials
const char* ssid = "banana";
const char* password = "52695269";

// Replace with your own OpenWeatherMap API key
const char* apiKey = "ebae9700db9685cdf4aba7e3b048fcad";

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE);

const char* server = "api.openweathermap.org";
const char* city = "TAMSUI"; // Replace with the city you want to get weather information for


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
  Serial.begin(9600);
  u8g2.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop(void) {
  if (WiFi.status() == WL_CONNECTED) {
    String payload = getWeatherData();
    // Parse the JSON data
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    JsonObject weather = doc["weather"][0];
    JsonObject main = doc["main"];
    float temperature = main["temp"];
    float temperatureCelsius = main["temperatureKelvin - 273.15"];
    int humidity = main["humidity"];
    String description = weather["description"];

    // Display weather data on the OLED screen
    u8g2.setFont(u8g2_font_HelvetiPixelOutline_te);
    u8g2.firstPage();
    do {
      u8g2.drawStr(0, 13, ("Temperature: " + String(temperatureCelsius) + " C").c_str());
      Serial.println("Temperature: " + String(temperatureCelsius) + " C");
      u8g2.drawStr(0, 26, ("Humidity: " + String(humidity) + "%").c_str());
      u8g2.drawStr(0, 39, ("Weather: " + description).c_str());
    } while (u8g2.nextPage());

    // Wait for a few seconds before updating the display again
    delay(50000);
  }
}
