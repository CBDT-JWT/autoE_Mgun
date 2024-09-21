#include <WiFi.h>
#include<Arduino.h>
#include<HardwareSerial.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<U8g2lib.h>
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//#include<wl_definitions.h>

const char* ssid = "esp8266";
const char* password = "wtgg6666";
const char* host = "192.168.4.1";
const int port = 8080; // ESP8266服务器监听的端口号
const int Rx=32,Ry=33,SW=25;
const int Btn=13;
const int R=12,G=14,B=27;
WiFiClient client;

void draw1(void) {
  u8g2.setFont(u8g2_font_ncenB08_tr); // 设置字体
  u8g2.drawStr(16, 16, "Electromagnetic");
  u8g2.drawStr(40, 28, "Cannon");
}
void draw2(void) {
  u8g2.setFont(u8g2_font_ncenB08_tr); // 设置字体
  u8g2.drawStr(32, 20, "Connected!");//在x=0,y=22的地方打印语句。
}
void draw3(void) {
  u8g2.setFont(u8g2_font_ncenB08_tr); // 设置字体
  //底座1
  u8g2.drawLine(20,32,68,32);
  u8g2.drawLine(20,30,68,30);
  u8g2.drawLine(20,32,20,30);
  u8g2.drawLine(68,32,68,30);
  //底座2
  u8g2.drawLine(22,30,22,28);
  u8g2.drawLine(22,28,64,28);
  u8g2.drawLine(64,30,64,28);
  //底座3
  u8g2.drawLine(40,24,40,28);
  u8g2.drawLine(48,24,48,28);
  u8g2.drawLine(40,24,48,24);
  //斜支架
  u8g2.drawLine(40,18,42,26);
  u8g2.drawLine(44,18,46,26);
  //小方块
  //炮管
  u8g2.drawLine(30,18,71,8);
  u8g2.drawLine(30,17,62,9);
  u8g2.drawLine(30,19,62,11);
  //隔板
  u8g2.drawLine(29,16,30,20);
  u8g2.drawLine(34,15,34,19);
  u8g2.drawLine(37,14,38,18);
  u8g2.drawLine(41,13,42,17);
  u8g2.drawLine(45,12,46,16);
  u8g2.drawLine(49,11,50,15);
  u8g2.drawLine(53,10,54,14);
  u8g2.drawLine(57,9,58,13);
  u8g2.drawLine(61,8,62,12);
  //电磁效果
  /*u8g2.drawLine(78,3,76,7);
  u8g2.drawLine(76,7,79,10);
  u8g2.drawLine(77,6,80,6);
  u8g2.drawLine(77,8,80,6);
  u8g2.drawLine(78,6,80,4);
  u8g2.drawLine(78,7,80,9);
  u8g2.drawLine(80,6,85,4);
  u8g2.drawLine(80,6,86,7);
  u8g2.drawLine(84,3,86,4);
  u8g2.drawLine(86,4,87,6);
  u8g2.drawLine(87,6,85,9);
  */
  //炮弹
  u8g2.drawLine(88,4,90,6);
  u8g2.drawLine(90,6,96,5);
  u8g2.drawLine(96,5,99,2);
  u8g2.drawLine(99,2,94,2);
  u8g2.drawLine(94,2,88,4);
  //闪电图案
  u8g2.drawLine(12,8,9,18);
  u8g2.drawLine(9,18,13,16);
  u8g2.drawLine(13,16,12,24);
  u8g2.drawLine(12,24,15,14);
  u8g2.drawLine(15,14,11,16);
  u8g2.drawLine(11,16,12,8);
  //文字
  u8g2.drawStr(76,24,"Electrify!");
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  pinMode(SW,INPUT_PULLUP);//摇杆上拉电
  pinMode(Btn,INPUT);
  pinMode(Rx,INPUT);
  pinMode(Ry,INPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  u8g2.begin();
  u8g2.firstPage();
  do {
    draw3();
  } while(u8g2.nextPage());
  delay(3000);
  u8g2.firstPage();
  do {
    draw1();
  } while(u8g2.nextPage());
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

int connection=0;
int btn=0,btn_pre=0,btn_swch=0,btn_init=0;
void loop() {
  if (!client.connect(host, port)) { // 尝试连接到服务器
    connection=0;
    digitalWrite(R,LOW);
    digitalWrite(G,LOW);
    digitalWrite(B,LOW);
    Serial.println("Connection to server failed");

    u8g2.firstPage();
    do {
      draw3();
    } while(u8g2.nextPage());
    delay(3000);

    u8g2.firstPage();
    do {
      draw1();
    } while(u8g2.nextPage());
    delay(2000);
    return;
  }

  //以下是正常情况下运行的代码
  if(connection==0){
    Serial.println("Connected to server");
    u8g2.firstPage();
    do {
      draw2();
    } while(u8g2.nextPage());
  }
  connection=1;
  
  // 等待服务器发送的消息
  int flag=-1,flag2=0;
  int voltage=0,charging=0,active=0;
  if (client.available()) { 
    String line = client.readStringUntil('\n');
    //Serial.print("Received from server: ");
    Serial.println(line);
    flag=sscanf(line.c_str(),"Voltage:%d,Charging:%d,Active:%d",&voltage,&charging,&active);
  }
  if(flag!=-1){
    int progress,progressX,progressWidth,progressHeight;
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB08_tr);
      // 显示"Charging"或"Active"
      u8g2.setCursor(36, 8);
      if (charging == 0 && active == 1) {
        u8g2.print("Active");
      } 
      else if(charging == 1 && active == 0) {
        u8g2.print("Charging");
      }
      else {
        Serial.print("Error");
        flag2=1;
      }
      if(flag2==0){
        // 显示文本"Voltage:"
        u8g2.setCursor(24, 16);
        u8g2.print("Voltage:");

        // 显示电压值
        u8g2.setCursor(70, 16);
        u8g2.print(voltage);
        u8g2.print("V");

        // 绘制进度条
        progress = (int)(voltage / 3.0); // 将电压值映射到进度条的百分比
        progressX = 14; // 进度条开始位置，确保不会顶格
        progressWidth = 100; // 进度条总长度
        progressHeight = 8; // 进度条高度

        // 绘制进度条背景
        u8g2.drawFrame(progressX, 22, progressWidth, progressHeight);

        // 绘制进度条填充
        if (progress > 0) {
          u8g2.drawBox(progressX, 22, progress, progressHeight);
        }
      }
    } while(u8g2.nextPage());
    flag=-1;
    flag2=0;
  }

  // 客户端测试消息
  //client.println("Hello from ESP32 Client");

  //摇杆&按钮
  int x=analogRead(Rx);
  int y=analogRead(Ry);
  int sw=1-digitalRead(SW);//换了个引脚，好了……
  btn=digitalRead(Btn);
  btn_swch=abs(btn-btn_pre);
  btn_init+=btn_swch;
  btn_init%=4;
  //Serial.println(btn);//
  char* chtr=new char[100];
  sprintf(chtr,"Mode:%d,JoyX:%d,JoyY:%d,JoyBtnPressed:%d",btn_init/2,x,y,sw);
  client.println(chtr);
  //client.print("\n");
  Serial.println(chtr);
  //Serial.print("\n");
  digitalWrite(R,LOW);
  digitalWrite(G,LOW);
  digitalWrite(B,LOW);
  if(btn_init==0){
    digitalWrite(G,HIGH);
    delay(100);
  }
  else if(btn_init==2){
    digitalWrite(B,HIGH);
    delay(100);
  }
  else
  {
    digitalWrite(R,HIGH);
  }
/*
  // 等待服务器响应
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 10000) {
      Serial.println("Client timeout");
      client.stop();
      return;
    }
  }

  // 读取服务器发送的第二条消息
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.print("Received from server: ");
    Serial.println(line);
  }
*/

  // 客户端发送确认接收消息
  //client.println("ESP32 Client received your message");

  // 关闭连接
  client.stop();
  //Serial.println("Client disconnected");// 每次都显示太乱了

  btn_pre=btn;

  // 等待一段时间再次发送
  delay(200);
}
