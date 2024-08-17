///*
//  GT-24 V1.0 (RF24L01) 测试开发程序
//  编者：果云科技有限公司
//淘宝店铺:https://shop532831232.taobao.com
//
//适用芯片：arduino UNO
//
//线路连接：  RF24L01： VCC        3.3V
//                      GND        GND
//                      CE         D8
//                     IRQ         --
//                      CSN        D7
//                      SCK        D13
//                     MOSI        D11
//                     MISO        D12
//          
//      OLED(IIC):  VCC        3.3V
//                  GND        GND
//                  SCL        A5
//                  SDA        A4
//          
//  最后修改时间；2018年8月1日 12:00:00
//*/
#include "U8glib.h"
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
char str1[]="guoyunkeji";
char str2[]="Send the number:";
uint8_t convert[5]={0};
uint8_t num,value;

//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
  uint32_t result=1;  
  while(n--)result*=m;    
  return result;
}
void ShowNum_buff(uint8_t* TxBuffer,uint8_t num,uint8_t len)   //把16进制数组转换为字符
{           
  uint8_t t,temp;
  uint8_t enshow=0;    
  for(t=0;t<len;t++)
  {
    temp=(num/oled_pow(10,len-t-1))%10;
    
    *TxBuffer =  temp+0x30;
    TxBuffer++;
  }
} 
void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void setup()
{
  u8g_prepare();//初始化字体，屏幕参数
  u8g.firstPage();  
    do {
      u8g.drawStr(30,0,(char *)str1);
      u8g.drawStr(0,10,(char *)str2);
   } 
    while( u8g.nextPage() );
    
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"ABCDE"); //设置自己的地址（发送端地址），使用5个字符
    Mirf.payload = sizeof(value);
    Mirf.channel = 90;              //设置所用信道
    Mirf.config();
}
 
void loop()
{
  Mirf.setTADDR((byte *)"FGHIJ");           //设置接收端地址
  value = random(255);                      //0-255的随机数
  Mirf.send((byte *)&value);                //发送指令，发送随机数value
  while(Mirf.isSending()) delay(1);         //直到发送成功，退出循环
  
   ShowNum_buff(convert,num,3);
   u8g.firstPage();  
    do {
      u8g.drawStr(30,0,(char *)str1);
      u8g.drawStr(0,10,(char *)str2);
      u8g.drawStr(100,10,convert);
      } 
    while( u8g.nextPage() );
    num++;
    delay(1000);
}
 
 

