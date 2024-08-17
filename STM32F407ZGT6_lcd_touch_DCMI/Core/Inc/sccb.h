#ifndef __SCCB_H
#define __SCCB_H
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 

//OV系列摄像头 SCCB 驱动代码	   
							  
////////////////////////////////////////////////////////////////////////////////// 


//IO方向设置
#define SCCB_SDA_IN()  {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=0<<7*2;}	//PD7 输入
#define SCCB_SDA_OUT() {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=1<<7*2;} 	//PD7 输出


//IO操作函数	 
#define SCCB_SCL_H HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_SET)
#define SCCB_SCL_L HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_RESET)
#define SCCB_SDA_H HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,GPIO_PIN_SET)
#define SCCB_SDA_L HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,GPIO_PIN_RESET)

#define SCCB_READ_SDA    	HAL_GPIO_ReadPin(SDA_GPIO_Port,SDA_Pin)  		//输入SDA    
#define SCCB_ID   			0X60  			//OV2640的ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WR_Byte(uint8_t dat);
uint8_t SCCB_RD_Byte(void); 
#endif

