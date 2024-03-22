//MySQL連線測試，手動寫入一個溫度及濕度資料做測試
#include <WiFi.h>
//#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>  //下達HTTP GET用
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft=TFT_eSPI();

const char ssid[] = "Bananananana";      // change to your WIFI SSID
const char password[] = "52695269";      // change to your WIFI Password
IPAddress server_addr(192, 168, 0, 11);  // change to you server ip, note its form split by "," not "."
int MYSQLPort = 3306;                    //mysql port default is 3306
char user[] = "rain";                    // Your MySQL user login username(default is root),and note to change MYSQL user root can access from local to internet(%)
char pass[] = "a0970018607";             // Your MYSQL password
WiFiClient client;
MySQL_Connection conn((Client *)&client);

int inPin = 22;
int d;

void read_data() {
  d = digitalRead(inPin);
  Serial.println("data:"+d);
  if (d == LOW) {  //如果沒有物體移動
    Serial.println("no moving");
  } else {
    Serial.println("moving");
  }
}

void LCD() {  
  if (d == LOW) {  //如果沒有物體移動
    Serial.println("no moving");
    tft.fillScreen(TFT_BLACK); // 用全黑清除螢幕
    tft.drawString("no moving",50,51,4);  
  } else {
    Serial.println("moving");
    tft.fillScreen(TFT_BLACK); // 用全黑清除螢幕
    tft.drawString("moving",50,51,4);
  }
}

void uploadMYSQL() {
  if (conn.connect(server_addr, 3306, user, pass)) {
    Serial.println("Connection sucess.");
    delay(500);
  } else {
    Serial.println("Connection failed.");
  }
  delay(2000);

  String INSERT_SQL = "INSERT INTO  infrared.datalog (result) VALUES ('" + String(d) + "')";
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL.c_str());  //execute SQL
  delete cur_mem;
  conn.close();  // close the connection
  Serial.println("Data Saved.");
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  //try to connect to WIFI
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(inPin, INPUT);

  tft.begin();               // 初始化LCD
  tft.setRotation(1);  // landscape
  tft.fillScreen(TFT_BLACK); // 用全黑清除螢幕
  tft.setFreeFont(&FreeSerifBold24pt7b);//設定字型

  }

void loop() {
  read_data();
  if (0 <= d && d <= 1) {
    //LCD();
    //uploadMYSQL();
    delay(100);
  }
}