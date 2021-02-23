#include "wifi.h"
#include "oled.h"
#include "oledfont.h" 

#include <Wire.h>
#include <ClosedCube_SHT3XD.h>
ClosedCube_SHT3XD sht3xd;

int LED1 = A3;
int LED2 = 4;
int net_flag = 0;

float sht30_temp = 0;
float sht30_hum = 0;

int sht30_temp_zen = 0;
int sht30_temp_fu = 0;
int sht30_hum_zen = 0;
int sht30_hum_fu = 0;

void myserialEvent(void);
void key_scan(void);

SoftwareSerial mySerial(9, 10);      // RX, TX

#define  key   6       //重置Wi-Fi按键

void setup()
{
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH); 
  pinMode(key, INPUT_PULLUP);        // 重置Wi-Fi按键初始化
  mySerial.begin(115200);     // 软件串口初始化
  Serial.begin(115200);     
  //Serial.println("serial init successful!");
  OLED_Init();   //oled初始化
  OLED_Clear();  //oled清屏
  OLED_ShowString(10,line3,"connect to net ...",6); 
  sht3xd.begin(0x44);
  Serial.print("SHT3XD Serial #");
  Serial.println(sht3xd.readSerialNumber());     
  wifi_protocol_init();       //wifi协议初始化  
}

void loop() 
{
   wifi_uart_service();     //wifi串口
   myserialEvent();         //串口接收处理
   key_scan();              //重置配网按键检测
   delay(10);
   wifi_stat_led();      //wifi状态指示灯
   if(net_flag == 1)     //net_flag=1配网成功
   {
       printResult("ClockStrech Mode", sht3xd.readTempAndHumidity(REPEATABILITY_LOW, MODE_CLOCK_STRETCH, 50));  
       delay(500);
       printResult("Pooling Mode", sht3xd.readTempAndHumidity(REPEATABILITY_HIGH, MODE_POLLING, 50));
       delay(500);
       OLED_Clear();  //oled清屏
       OLED_ShowString(10,line3,"temp:",6); 
       OLED_ShowNum(40,line3,sht30_temp_zen,2,6); 
       OLED_ShowString(52,line3,".",6); 
       OLED_ShowNum(55,line3,sht30_temp_fu,2,6);
       OLED_ShowString(68,line3,"C",6); 
       OLED_ShowString(10,line5,"hum:",6);
       OLED_ShowNum(33,line5,sht30_hum_zen,2,6);
       OLED_ShowString(45,line5,".",6); 
       OLED_ShowNum(48,line5,sht30_hum_fu,2,6);
       OLED_ShowString(60,line5,"%",6); 
       delay(10);
   }

   all_data_update();
   delay(100);
}

void printResult(String text, SHT3XD result)
{
  int m,n;
  if (result.error == NO_ERROR) {
    Serial.print(text);
    Serial.print(": T=");
    Serial.print(result.t);
    sht30_temp = result.t*10;  //涂鸦上传数据可能有BUG，会/10，所以先*10
    sht30_temp_zen =(int)result.t;
    m = result.t*100;
    m = m % 100;
    sht30_temp_fu = m;
    Serial.print("C, RH=");
    Serial.print(result.rh);
    sht30_hum = result.rh;
    sht30_hum_zen = (int)result.rh;
    n = result.rh*100;
    n = n % 100;
    sht30_hum_fu = n;
    Serial.println("%");
  }
  else {
    Serial.print(text);
    Serial.print(": [ERROR] Code #");
    Serial.println(result.error);
  }
}

void myserialEvent()
{
  if (mySerial.available()) {
    unsigned char ch = (unsigned char)mySerial.read();
    uart_receive_input(ch);
  }
}

void key_scan(void)
{
  unsigned char buttonState  = HIGH;
  buttonState = digitalRead(key);
  if (buttonState == LOW) 
  {
    delay(10);
    buttonState = digitalRead(key);
    if (buttonState == LOW)
    {
       Serial.println("SMART_CONFIG!");
       mcu_set_wifi_mode(SMART_CONFIG);   //智能配网
       //mcu_set_wifi_mode(AP_CONFIG);    //AP配网
    }
  }
}


void wifi_stat_led(void)
{
  switch(mcu_get_wifi_work_state())  //获取wifi工作状态
  {
    case SMART_CONFIG_STATE:  //0x00
      digitalWrite(LED1,HIGH);       //灯灭
      net_flag = 0;
      break;
    case AP_STATE:  //0x01
      digitalWrite(LED1,HIGH);       //灯灭
      net_flag = 0;
      break;

    case WIFI_NOT_CONNECTED:  // 0x02
      digitalWrite(LED1,HIGH);       //灯灭
      net_flag = 0;
      break;
      
    case WIFI_CONNECTED:  // 0x03
      digitalWrite(LED1,HIGH);       //灯灭
      net_flag = 0;
      break;
      
    case WIFI_CONN_CLOUD:  // 0x04  //配网成功，连接上云
      digitalWrite(LED1,LOW);       //灯亮
      net_flag = 1;
      break;
      
    default:
      digitalWrite(LED1,HIGH);       //灯灭
      net_flag = 0;
      break;
  }
}
