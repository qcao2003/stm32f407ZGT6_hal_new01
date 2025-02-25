/**
 ****************************************************************************************************
 * @file        remote.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-27
 * @brief       红外遥控解码 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211027
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __REMOTE_H
#define __REMOTE_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* 红外输入引脚及定时器 定义 */

#define REMOTE_IN_GPIO_PORT                     GPIOA
#define REMOTE_IN_GPIO_PIN                      GPIO_PIN_8
#define REMOTE_IN_GPIO_AF                       GPIO_AF1_TIM1                               /* AF功能选择 */
#define REMOTE_IN_GPIO_CLK_ENABLE()             do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PA口时钟使能 */


#define REMOTE_IN_TIMX                          TIM1                       
#define REMOTE_IN_TIMX_IRQn                     TIM1_UP_TIM10_IRQn
#define REMOTE_IN_TIMX_IRQHandler               TIM1_UP_TIM10_IRQHandler
#define REMOTE_IN_TIMX_CHY                      TIM_CHANNEL_1                               /* 通道Y,  1<= Y <=2*/ 
#define REMOTE_IN_TIMX_CCRY                     REMOTE_IN_TIMX->CCR1
#define REMOTE_IN_TIMX_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)  /* TIMX 时钟使能 */

/* TIM1 / TIM8 有独立的捕获中断服务函数,需要单独定义,对于TIM2~5/TIM12/TIM15等, 则不需要以下定义 */
#define REMOTE_IN_TIMX_CC_IRQn                  TIM1_CC_IRQn
#define REMOTE_IN_TIMX_CC_IRQHandler            TIM1_CC_IRQHandler

/******************************************************************************************/


#define RDATA           HAL_GPIO_ReadPin(REMOTE_IN_GPIO_PORT, REMOTE_IN_GPIO_PIN)           /* 红外数据输入脚 */


/* 红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
 * 我们选用的遥控器识别码为0
*/
#define REMOTE_ID       0

extern uint8_t g_remote_cnt;    /* 按键按下的次数 */
    
void remote_init(void);         /* 红外传感器接收头引脚初始化 */
uint8_t remote_scan(void);

#endif















