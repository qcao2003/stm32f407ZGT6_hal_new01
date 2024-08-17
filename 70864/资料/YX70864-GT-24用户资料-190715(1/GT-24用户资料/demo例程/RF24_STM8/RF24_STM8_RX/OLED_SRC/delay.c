/*
    GT-24 V1.0 (RF24L01) ���Կ�������

    ���ߣ����ƿƼ����޹�˾
    �Ա�����:https://shop532831232.taobao.com
	
    ����оƬ�� STM8S105    
	
   ��·���ӣ�  RF24L01�� VCC        3.3V
			GND        GND
			 CE        B3
			IRQ        C4
                        CSN        E5
			SCK        C5
			MOSI        C6
			MISO        C7
						
	    OLED(IIC):  VCC        3.3V
			GND        GND
			SCL        D6
			SDA        D7
						
    ����޸�ʱ�䣻2018��8��1�� 12:00:00
  ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*/


#include "delay.h"



/**
  * @brief :1MS��ʱ����
  * @param :
  * @note  :12MHz ��1MS��ʱ
  * @retval:��
  */
static void delay_1ms( void )
{
	uint16_t Ms = 1;
	uint32_t j = 30;
	
	while( Ms-- )
	{
		while( j-- );
	}
}

/**
  * @brief :MS��ʱ����
  * @param :
  *			@Ms:��ʱ��MS��
  * @note  :��
  * @retval:��
  */
void delay_ms( uint16_t Ms )
{
	while( Ms-- )
	{
		delay_1ms( );
	}
}

