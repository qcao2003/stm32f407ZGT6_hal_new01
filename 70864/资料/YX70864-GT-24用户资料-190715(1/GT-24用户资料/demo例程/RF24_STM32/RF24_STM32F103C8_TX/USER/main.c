/*
    GT-24 V1.0 (RF24L01) ���Կ�������

    ���ߣ����ƿƼ����޹�˾
    �Ա�����:https://shop532831232.taobao.com
	
    ����оƬ�� STM32f103ϵ�� 
	
   ��·���ӣ�  RF24L01�� VCC        3.3V
			            GND        GND
			            CE         A0
						IRQ        A1
                        CSN        B12
						SCK        B13
						MOSI       B15
						MISO       B14
						
	  		 OLED(IIC): VCC        3.3V
					 	GND        GND
					 	SCL        A5
					 	SDA        A7
						
    ����޸�ʱ�䣻2018��8��1�� 12:00:00
  ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "spi.h"
#include "RF24L01.h"

uint8_t *content = "yunguokeji";
uint8_t UART_BUFFER[ 10 ] = { 0,1,2,3,4,5,6,7,8,9}; 

int main(void)
{  	
	    uint8_t num;
		delay_init();	    	 //��ʱ������ʼ��	  
		OLED_Init();			//��ʼ��OLED  
		OLED_Clear()  	;
	  
	    OLED_ShowCHinese(0,0,0);//��
	    
	    drv_spi_init( );
	    NRF24L01_Gpio_Init( );     //RF24L01���ų�ʼ��
	    NRF24L01_check( );	 //���nRF24L01		
        RF24L01_Init( );
	
		OLED_Clear();
		OLED_ShowCHinese(0,3,0);     //��
		OLED_ShowCHinese(18,3,1);    //��
		OLED_ShowCHinese(36,3,2);    //��
		OLED_ShowCHinese(54,3,3);    //��
		OLED_ShowCHinese(72,3,4);    //��
		
		OLED_ShowCHinese(0,0,5);     //��
		OLED_ShowCHinese(16,0,6);    //��
		OLED_ShowCHinese(32,0,7);    //��
		OLED_ShowCHinese(48,0,8);    //��
		OLED_ShowCHinese(64,0,9);    //��
		OLED_ShowCHinese(80,0,10);   //��
		OLED_ShowCHinese(96,0,11);   //��
		OLED_ShowCHinese(112,0,12);  //˾
		while(1)
		{
			 RF24L01_Set_Mode( MODE_TX );		//����ģʽ
	  	     NRF24L01_TxPacket( content , 10 );		//ģʽ1���͹̶��ַ�,1Sһ��
//		     NRF24L01_TxPacket( UART_BUFFER, 10 );		//ģʽ1���͹̶��ַ�,1Sһ��
		     num++;
		     OLED_ShowNum(90,3,num,3,16);
			
		     delay_ms( 1000 );
		}

	
}  

