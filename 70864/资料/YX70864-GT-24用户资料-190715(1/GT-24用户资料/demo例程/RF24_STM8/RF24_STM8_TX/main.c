/*
    GT-24 V1.0 (RF24L01) 测试开发程序

    编者：果云科技有限公司
    淘宝店铺:https://shop532831232.taobao.com
	
    适用芯片： STM8S系列 
	
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
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "delay.h"
#include "oled.h"
#include "bmp.h"
#include "spi.h"
#include "RF24L01.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint8_t *content = "yunguokeji";
uint8_t UART_BUFFER[ 10 ] = { 0,1,2,3,4,5,6,7,8,9}; 

void main(void)
{	
        uint8_t  num;
        OLED_Init();			//3?ê??ˉOLED  
        OLED_Clear(); 
        
	OLED_ShowCHinese(0,3,0);     //发
	OLED_ShowCHinese(18,3,1);    //送
	OLED_ShowCHinese(36,3,2);    //次
	OLED_ShowCHinese(54,3,3);    //数
	OLED_ShowCHinese(72,3,4);    //：
        
	drv_spi_init( );
        
       NRF24L01_Gpio_Init( );     //RF24L01引脚初始化
	NRF24L01_check( );	 //检测nRF24L01		
        RF24L01_Init( );
        
	OLED_ShowCHinese(0,0,5);     //果
	OLED_ShowCHinese(16,0,6);    //云
	OLED_ShowCHinese(32,0,7);    //科
	OLED_ShowCHinese(48,0,8);    //技
	OLED_ShowCHinese(64,0,9);    //有
	OLED_ShowCHinese(80,0,10);   //限
	OLED_ShowCHinese(96,0,11);   //公
	OLED_ShowCHinese(112,0,12);  //司	
	while(1) 
	{		
          RF24L01_Set_Mode( MODE_TX );		//发送模式
         NRF24L01_TxPacket( content , 10 );		//模式1发送固定字符,1S一包
//	 NRF24L01_TxPacket( UART_BUFFER, 10 );		//模式1发送固定字符,1S一包
         num++;
         OLED_ShowNum(90,3,num,3,16);
            
         delay_ms( 1000 );
         
          
	}	  
	
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
