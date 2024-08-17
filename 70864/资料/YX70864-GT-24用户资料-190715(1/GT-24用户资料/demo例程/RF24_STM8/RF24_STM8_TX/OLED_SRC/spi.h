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


#ifndef __SPI_H__
#define __SPI_H__


//#ifndef __USE_SOFT_SPI_INTERFACE__

//#endif
#include "stm8s.h"
#include "stm8s_spi.h"
#include "stm8s_clk.h"


//SPIӲ���ӿ�IO����
#define SPI_CLK_GPIO_PORT			GPIOC
#define SPI_CLK_GPIO_PIN			GPIO_PIN_5

#define SPI_MISO_GPIO_PORT			GPIOC
#define SPI_MISO_GPIO_PIN			GPIO_PIN_7

#define SPI_MOSI_GPIO_PORT			GPIOC
#define SPI_MOSI_GPIO_PIN			GPIO_PIN_6

#define SPI_NSS_GPIO_PORT			GPIOE
#define SPI_NSS_GPIO_PIN			GPIO_PIN_5


#define spi_set_nss_high( )			GPIO_WriteHigh( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN )		//NSS�øߣ�ȡ��Ƭѡ
#define spi_set_nss_low( )			GPIO_WriteLow( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN )	        //NSS�õͣ�Ƭѡ���豸

//#ifdef __USE_SOFT_SPI_INTERFACE__					
///** ���ģ��SPI������������ */
//
//#define spi_set_clk_high( )			GPIO_SetBits( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN )		//ʱ���ø�
//#define spi_set_clk_low( )			GPIO_ResetBits( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN )	//ʱ���õ�
//
//#define spi_set_mosi_hight( )		GPIO_SetBits( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN )	//����øߣ�������
//#define spi_set_mosi_low( )			GPIO_ResetBits( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN )	//����õͣ�������
//
//#define spi_get_miso( )				( 0 == GPIO_ReadInputDataBit( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN )) ? 0 : 1 // ��ȡ����״̬
//
//#endif
void drv_spi_init( void );
uint8_t drv_spi_read_write_byte( uint8_t TxByte );
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );


#endif

