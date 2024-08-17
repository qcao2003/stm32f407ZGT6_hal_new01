/*
    GT-24 V1.0 (RF24L01) 测试开发程序

    编者：果云科技有限公司
    淘宝店铺:https://shop532831232.taobao.com
	
    适用芯片： STM8S105    
	
   线路连接：  RF24L01： VCC        3.3V
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
						
    最后修改时间；2018年8月1日 12:00:00
  本程序只供学习使用，未经作者许可，不得用于其它任何用途
*/


#include "delay.h"



/**
  * @brief :1MS延时函数
  * @param :
  * @note  :12MHz 下1MS延时
  * @retval:无
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
  * @brief :MS延时函数
  * @param :
  *			@Ms:延时的MS数
  * @note  :无
  * @retval:无
  */
void delay_ms( uint16_t Ms )
{
	while( Ms-- )
	{
		delay_1ms( );
	}
}

