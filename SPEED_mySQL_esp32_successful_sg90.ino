#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // 添加用于处理JSON的库
#include <TFT_eSPI.h> 
#include <ESP32Servo.h>

int inpin = 37;
//int inpin = 36;
int sensorValue;
int i = 0;
int j = 0;
int x = 0;
double hKM = 0;
int ii = 0;
int test = 10;
TFT_eSPI tft; // Declare the TFT_eSPI object

Servo myservo;
int buttonPinPlus = 33;    
int buttonPinReduce = 27;  
int angle = 90;           // 初始化伺服电机位置为90度
int speed = 10;           // 初始化移动速度
bool direction = true;    // 初始化运动方向为正向

const char ssid[] = "Banana";      // 更改為您的WIFI SSID
const char password[] = "52695269";      // 更改為您的WIFI密碼

const char *server_url = "http://163.13.201.94/insert.php";  // 更改為您的PHP腳本的URL

void read(){
  sensorValue = analogRead(inpin);
  tft.fillScreen(TFT_BLACK); // 用全黑清除螢幕
  tft.drawString(String(sensorValue),50,2,4);
  tft.drawString(String(hKM),120,2,4);
  tft.drawString(String(speed),50,30,4);
  
}

void speedd() {
  i++;

  if (sensorValue < 2500) {
    x = i - j;
    j = i;
  }
  if (x != 1 ) {
    hKM = 370.0 / x;
  }
}


void uploadPHP() {
  HTTPClient http;
  http.begin(server_url);

  String hKMString = String(hKM, 0); // 將 double 轉換為字串，保留兩位小數
  // 创建JSON对象
  StaticJsonDocument<200> jsonDoc; // 根据您的数据大小适当调整容量
  jsonDoc["value"] = hKMString;

  // 将JSON数据序列化为字符串
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // 发送JSON数据作为POST请求正文
  int httpResponseCode = http.POST(jsonString);

  if (httpResponseCode == 200) {
    Serial.println("Data Saved.");
    Serial.println(hKMString);      // 印出字串
  } else {
    Serial.print("HTTP Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void setup() {
  Serial.begin(115200);
  pinMode(inpin, INPUT);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  tft.begin();               // 初始化LCD
  tft.setRotation(3);  // landscape
  tft.fillScreen(TFT_BLACK); // 用全黑清除螢幕 
  tft.setFreeFont(&FreeSerifBold24pt7b);//設定字型

  myservo.attach(21);
  pinMode(buttonPinPlus, INPUT_PULLUP);   // 设置按钮引脚为输入，并启用内部上拉电阻
  pinMode(buttonPinReduce, INPUT_PULLUP); // 设置按钮引脚为输入，并启用内部上拉电阻
}

void sg90(){
  int buttonState1 = digitalRead(buttonPinPlus);   // 读取按钮状态
  int buttonState2 = digitalRead(buttonPinReduce); // 读取按钮状态

  if (buttonState1 == LOW) { // 如果按钮1被按下
    if (speed < 40) {
      speed += 1; // 逐渐增加速度
    }
  }

  if (buttonState2 == LOW) { // 如果按钮2被按下
    if (speed > 0) {
      speed -= 1; // 逐渐减小速度
    }
  }

  // 限制速度在一定范围内
  if (speed > 100) {
    speed = 100;
  }
  if (speed < 0) {
    speed = 1;
  }


  if (direction) {
    angle += speed; // 正向运动
    if (angle >= 175) {
      angle = 175;
      direction = false; // 到达180度后改变方向
    }
  } else {
    angle -= speed; // 反向运动
    if (angle <= 5) {
      angle = 5;
      direction = true; // 到达0度后改变方向
    }
  }

  myservo.write(angle);
  delay(80); // 等待一段时间，防止快速连续移动
}
void loop() {
  read();
  speedd();
  sg90();
  ii++;
  if (ii > 50) {
    uploadPHP();
    ii = 0;
    hKM = 1;
  }
  delay(10);
}