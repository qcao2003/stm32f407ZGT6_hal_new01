/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcmi.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "touch.h"
#include "ov5640.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define FrameWidth 320
#define FrameHeight 240
#define totalpoint (FrameWidth*FrameHeight)/3
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t jpeg_buf[totalpoint];
volatile uint32_t DCMI_FrameIsReady = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//void rgb565_test(uint16_t* jpeg_buf);
void rgb565_test(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	u8 ret = 0;
	u32 i,index = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_DCMI_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	// 开启串口接收中断
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1Recv.recvData, 1);
//	Demo_Menu();
	ret = OV5640_Init();  //初始化
	printf("This OV5640_init result is %d\r\n",ret);
	//自动对焦初始化
	OV5640_RGB565_Mode();	//RGB565模式 
	OV5640_Focus_Init();
	
	OV5640_Light_Mode(0);	//自动模式
	OV5640_Color_Saturation(3);//色彩饱和度0
	OV5640_Brightness(4);	//亮度0
	OV5640_Contrast(3);		//对比度0
	OV5640_Sharpness(33);	//自动锐度
	OV5640_Focus_Constant();//启动持续对焦
	OV5640_OutSize_Set(4,0,320,240);
//	__HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);  
	
//	HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)&LCD->LCD_RAM,1);
//	HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)&jpeg_buf,totalpoint);
//	rgb565_test();
//	LCD_Display_Dir(2); //横屏方向
//	LCD_Set_Window(0,0,lcddev.width,lcddev.height);
//	LCD_SetCursor(0,0);	//设置光标位置 
//	LCD_WriteRAM_Prepare();     //开始写入GRAM
//	totalpoint=lcddev.width * lcddev.height; 			//得到总点数
//	for(index=0;index<40000;index++)
//	{
//		LCD->LCD_RAM=GREEN;
//	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		if(DCMI_FrameIsReady)
//		{
//			DCMI_FrameIsReady = 0;
//			LCD_Set_Window(0,0,320,240);//设置一个自动换行显示区域窗口

//			LCD_WriteRAM_Prepare();     	//开始写入GRAM	
//							
//			for(i=0;i<320*240;i++)
//			{
//				LCD->LCD_RAM=jpeg_buf[i]; 
//			}
//			
//		}
//		if(TP_Scan(1))	
//			printf("x is 0x%X,y is 0x%X\r\n",tp_dev.x[0],tp_dev.y[0]);
		if(usart1Recv.recvFlag == 1)
		{
			usart1Recv.recvFlag = 0;
			if(strcmp((char*)usart1Recv.recvBuff, "LED\r") == 0)
			{
				// 发送数据
				printf("%s",usart1Recv.recvBuff);
				printf("temp is %d\r\n",temp);
				HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
				HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
//				for(i=0;i<totalpoint;i++)
//				{
//					printf("jpeg_buf[%d] is 0x%X\r\n",i,jpeg_buf[i]);
//				}
				LCD_Display_Dir(2); //横屏方向
				LCD_Set_Window(0,0,lcddev.width,lcddev.height);
				LCD_SetCursor(0,0);	//设置光标位置 
				LCD_WriteRAM_Prepare();     //开始写入GRAM
				__HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);
				HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)&jpeg_buf,totalpoint);
				for(index = 0;index < totalpoint; index++)
				{
					LCD->LCD_RAM = jpeg_buf[index] >> 16;
					LCD->LCD_RAM = jpeg_buf[index] & 0xFFFF;
				}
			}
			// 发送数据
			printf("UART_FLAG\r\n");
			memset(usart1Recv.recvBuff,0,sizeof(usart1Recv.recvBuff));
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */
//RGB565测试
//RGB数据直接显示在LCD上面
//void rgb565_test(uint16_t* jpeg_buf)
void rgb565_test(void)
{ 

	LCD_Clear(WHITE);
  POINT_COLOR=RED; 
	
	//自动对焦初始化
	OV5640_RGB565_Mode();	//RGB565模式 
	OV5640_Focus_Init();
	
	OV5640_Light_Mode(0);	//自动模式
	OV5640_Color_Saturation(3);//色彩饱和度0
	OV5640_Brightness(4);	//亮度0
	OV5640_Contrast(3);		//对比度0
	OV5640_Sharpness(33);	//自动锐度
	OV5640_Focus_Constant();//启动持续对焦
	LCD_Display_Dir(1); //横屏方向
	LCD_Set_Window(0,0,320,240);
	OV5640_OutSize_Set(4,0,320,240);
  LCD_SetCursor(0,0);  
	LCD_WriteRAM_Prepare();		//开始写入GRAM
//	__HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);  
	
//	HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)&jpeg_buf,FrameWidth * FrameHeight * 2 / 4);
	
	HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)LCD->LCD_RAM,FrameWidth * FrameHeight * 2 / 4);


}

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	DCMI_FrameIsReady++;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
