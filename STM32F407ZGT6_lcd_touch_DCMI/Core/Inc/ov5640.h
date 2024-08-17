#ifndef _OV5640_H
#define _OV5640_H
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 

//OV5640 驱动代码	   

////////////////////////////////////////////////////////////////////////////////// 

#define OV5640_PWDN_H  	HAL_GPIO_WritePin(PWDN_GPIO_Port,PWDN_Pin,GPIO_PIN_SET)			//POWER DOWN控制信号
#define OV5640_PWDN_L  	HAL_GPIO_WritePin(PWDN_GPIO_Port,PWDN_Pin,GPIO_PIN_RESET)			//POWER DOWN控制信号
#define OV5640_RST_H  	HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_SET)			//复位控制信号 
#define OV5640_RST_L  	HAL_GPIO_WritePin(RESET_GPIO_Port,RESET_Pin,GPIO_PIN_RESET)			//复位控制信号

//#define OV5640_LED_light  	PFout(8)			//补光LED灯控制引脚

//#define OV5640_LED_light_H  	HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_SET)			//补光LED灯控制引脚
//#define OV5640_LED_light_L  	HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_RESET)			//补光LED灯控制引脚
////////////////////////////////////////////////////////////////////////////////// 
#define OV5640_ID               0X5640  	//OV5640的芯片ID
 

#define OV5640_ADDR        		0X78		//OV5640的IIC地址
 
//OV5640相关寄存器定义  
#define OV5640_CHIPIDH          0X300A  	//OV5640芯片ID高字节
#define OV5640_CHIPIDL          0X300B  	//OV5640芯片ID低字节
 

								
u8 OV5640_WR_Reg(u16 reg,u8 data);
u8 OV5640_RD_Reg(u16 reg);
void OV5640_PWDN_Set(u8 sta);
u8 OV5640_Init(void);  
void OV5640_JPEG_Mode(void);
void OV5640_RGB565_Mode(void);
void OV5640_Exposure(u8 exposure);
void OV5640_Light_Mode(u8 mode);
void OV5640_Color_Saturation(u8 sat);
void OV5640_Brightness(u8 bright);
void OV5640_Contrast(u8 contrast);
void OV5640_Sharpness(u8 sharp);
void OV5640_Special_Effects(u8 eft);
void OV5640_Test_Pattern(u8 mode);
void OV5640_Flash_Ctrl(u8 sw);
u8 OV5640_OutSize_Set(u16 offx,u16 offy,u16 width,u16 height);
u8 OV5640_ImageWin_Set(u16 offx,u16 offy,u16 width,u16 height); 
u8 OV5640_Focus_Init(void);
u8 OV5640_Focus_Single(void);
u8 OV5640_Focus_Constant(void);
#endif

