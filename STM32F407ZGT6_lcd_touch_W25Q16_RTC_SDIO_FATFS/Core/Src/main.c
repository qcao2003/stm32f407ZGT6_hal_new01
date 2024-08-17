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
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "touch.h"
#include "w25qxx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
UINT fnum; 
BYTE ReadBuffer[1024] = {0};
BYTE WriteBuffer[] ="This is STM32 working with FatFs\r\n";
const char Filename[] = "test.txt";

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
RTC_TimeTypeDef GetTime;
RTC_DateTypeDef GetData;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void testFATFS(const char* Filename,BYTE write_buff[],uint8_t bufSize, UINT bw)
{
	 retSD=f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);//����
		if(retSD==FR_OK)
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"����SD���ɹ�!\r\n",15,100);
		}
		//д���ļ�����
		HAL_UART_Transmit(&huart1,(uint8_t*)"д���ļ�����!\r\n",15,100);
//		retSD = f_open(&SDFile,(const char*)"����.txt",FA_CREATE_ALWAYS|FA_WRITE);
		retSD = f_open(&SDFile,Filename,FA_CREATE_ALWAYS|FA_WRITE);
		if(retSD==FR_OK)
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"���ļ�\"����.txt\"�ɹ�!\r\n",25,100);
		}
		retSD = f_write(&SDFile,write_buff,bufSize,&bw);	
		if(retSD==FR_OK)
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"д���ļ��ɹ�!\r\n",15,100);
			printf("bw is %d",bw);
		}
		retSD = f_close(&SDFile);
		if(retSD==FR_OK)
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"�ر��ļ��ɹ�!\r\n\r\n",17,100);
		}
		//��ȡ�ļ�����
		HAL_UART_Transmit(&huart1,(uint8_t*)"��ȡ�ļ�����!\r\n",15,100);
		retSD = f_open(&SDFile,Filename,FA_OPEN_EXISTING|FA_READ);//���ļ�
		if(retSD==FR_OK)
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"���ļ�\"����.txt\"�ɹ�!\r\n",25,100);
		}
		retSD = f_read(&SDFile,write_buff,bufSize,&bw);
		if(retSD==FR_OK)
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"��ȡ�ļ��ɹ�!�ļ����ݣ�",23,100);
			HAL_UART_Transmit(&huart1,write_buff,bw,100);
			printf("br is %d",bw);
		}
		retSD = f_close(&SDFile);
		if(retSD==FR_OK)
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)"�ر��ļ��ɹ�!\r\n",15,100);
		}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char Data[50],Time[50];
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
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	W25QXX_Init();
	GBK_Lib_Init();//Ӳ��GBK�ֿ��ʼ��--(���ʹ�ò����ֿ��Һ�����汾���˴��������Σ������ֿ��ʼ���� 
	tp_dev.init();				//��������ʼ��
	// �������ڽ����ж�
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1Recv.recvData, 1);
//	Demo_Menu();
	printf("W25Q16id is 0X%X",W25QXX_ReadID());
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		if(GetTime.Seconds == 59)
		{
			HAL_Delay(1000);
			LCD_Clear(WHITE);
			/* Get the RTC current Time */
			HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);//������59��ʱ���ˢ��
			/* Get the RTC current Date */
			HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);

		}else
		{
			/* Get the RTC current Time */
			HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);//������59��ʱ���ˢ��
			/* Get the RTC current Date */
			HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);

		}
			
		sprintf(Data,"%s%d%s%d%s%d%s","20",GetData.Year,"��", GetData.Month,"��",GetData.Date,"��");
		sprintf(Time,"%s%d%s%d%s%d","ʱ��",GetTime.Hours,":",GetTime.Minutes,":",GetTime.Seconds);
		GBK_Show_Str(0,80,240,32,(u8*)Data,32,BLUE,WHITE,0);
		GBK_Show_Str(0,150,240,32,(u8*)Time,32,BLUE,WHITE,0);
		if(TP_Scan(1))	
			printf("x is 0x%X,y is 0x%X\r\n",tp_dev.x[0],tp_dev.y[0]);
		if(usart1Recv.recvFlag == 1)
		{
			usart1Recv.recvFlag = 0;
			if(strcmp((const char*)usart1Recv.recvBuff, "LED\r") == 0)
			{
				// ��������
				printf("%s",usart1Recv.recvBuff);
				printf("temp is %d\r\n",temp);
				HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
				HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
			}
			if(strcmp((const char*)usart1Recv.recvBuff, "Write\r") == 0)
			{
				// ��������
				printf("%s",usart1Recv.recvBuff);
				testFATFS(Filename,WriteBuffer,sizeof(WriteBuffer), fnum);
			}
			if(strcmp((const char*)usart1Recv.recvBuff, "Read\r") == 0)
			{
				// ��������
				printf("%s",usart1Recv.recvBuff);
			}
			if(strcmp((const char*)usart1Recv.recvBuff, "InitSD\r") == 0)
			{

			}
			// ��������
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
