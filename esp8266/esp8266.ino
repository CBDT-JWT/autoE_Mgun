#include<ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include<Arduino.h>
#include<HardwareSerial.h>
#include<stdio.h>
#include<string.h>

#include<wl_definitions.h>

const char* ssid="esp8266";
const char* password="wtgg6666";
const int port = 8080;

WiFiServer server(port);

void setup() {
  //消息
  Serial.begin(9600);
  WiFi.softAP(ssid,password);
  //WiFi.begin(ssid,password);
  Serial.print("\nAccess Point: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(1000);
  //  Serial.print(".");
  //}

  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());

  server.begin(); // 启动服务器
  Serial.println("Server started");

}
void loop() {
  WiFiClient client = server.available(); // 检查是否有客户端连接
  if (client) { // 如果有客户端连接
    //Serial.println("New client connected");// 每次都显示太乱了
    //串口->server->client
    //delay(2000);
    if(Serial.available()){
      /*Serial.println("Voltage:");
      delay(1000);
      int voltage=Serial.read();
      Serial.println("Charging?:");
      delay(1000);
      bool charging=Serial.read();
      Serial.println("Active?:");
      delay(1000);
      bool active=Serial.read();
      */
      char* chpr=new char[100];
      sprintf(chpr,Serial.readStringUntil('\n').c_str());
      //sprintf(chpr,"Voltage:%d,Charging:%d,Active:%d",voltage,charging,active);
      client.println(chpr);
      Serial.print(chpr);
      delete chpr;
    }
    //client->server(->client)
    //int mode,joyx,joyy,joybtn;
    //int flag=-1;
    while (client.connected()) {  //没懂为什么不能用if
      if (client.available()) { // 如果有数据可读
        String line = client.readStringUntil('\n'); // 读取一行数据
        //Serial.print("Received from client: ");
        Serial.println(line);
        //client.println(line); // 回显给客户端
        //flag=sscanf(line.c_str(),"Mode:%d,JoyX:%d,JoyY:%d,JoyBtnPressed:%d",mode,joyx,joyy,joybtn);
      }
    }
    client.stop(); // 关闭连接
    //Serial.println("Client disconnected");// 每次都显示太乱了
    //delay(1000);
  }
}
