#include <WiFi.h>
#include <WiFiClientSecure.h>  //下達HTTP GET用
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <TFT_eSPI.h> 
TFT_eSPI tft=TFT_eSPI(); //240*135

int sensorValue;

const char ssid[] = "Bananananana";      // change to your WIFI SSID
const char password[] = "52695269";      // change to your WIFI Password
IPAddress server_addr(192, 168, 0, 11);  // change to you server ip, note its form split by "," not "."
int MYSQLPort = 3306;                    //mysql port default is 3306
char user[] = "rain";                    // Your MySQL user login username(default is root),and note to change MYSQL user root can access from local to internet(%)
char pass[] = "a0970018607";             // Your MYSQL password
WiFiClient client;
MySQL_Connection conn((Client *)&client);


void read_iR(){
  sensorValue = analogRead(37);
  Serial.println(sensorValue);
  tft.fillScreen(TFT_BLACK); // 用全黑清除螢幕
  tft.drawString(String(sensorValue),50,2,4); //FONT2-16pixel FONT4-26pixel FONT6-48 7-48 8-75 1-8
}

void uploadMYSQL() {
  if (conn.connect(server_addr, 3306, user, pass)) {
    Serial.println("Connection sucess.");
    delay(10);
  } else {
    Serial.println("Connection failed.");
  }
  delay(10);

  //加入SQL字串
  String INSERT_iR = "INSERT INTO  infrared.speed_datalog (result) VALUES ('" + String(sensorValue) + "')";
  MySQL_Cursor *cur_mem_iR = new MySQL_Cursor(&conn);
  cur_mem_iR->execute(INSERT_iR.c_str());  //execute SQL
  delete cur_mem_iR;
  conn.close();  // close the connection
  Serial.println("Data Saved.");
  delay(10);
}

void setup() {
  Serial.begin(115200);
  pinMode(37,INPUT);

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

  tft.begin();               // 初始化LCD
  tft.setRotation(1);  // landscape
  tft.fillScreen(TFT_BLACK); // 用全黑清除螢幕
  tft.setFreeFont(&FreeSerifBold24pt7b);//設定字型
}

void loop() {
  read_iR();
  uploadMYSQL();
}