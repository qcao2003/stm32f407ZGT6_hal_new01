/**
 ****************************************************************************************************
 * @file        dac.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.2
 * @date        2021-11-03
 * @brief       DAC 驱动代码
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
 * V1.0 20211023
 * 第一次发布
 * V1.1 20211023
 * 新增dac_triangular_wave函数
 * V1.2 20211103
 * 新增dac_dma_wave_init,dac_dma_wave_enable函数
 *
 ****************************************************************************************************
 */

#include "./BSP/DAC/dac.h"
#include "./SYSTEM/delay/delay.h"


DAC_HandleTypeDef g_dac1_handle;       /* DAC句柄 */

/* 正弦波相关图 */


/**
 * @brief       DAC初始化函数
 *   @note      本函数支持DAC1_OUT1/2通道初始化
 *              DAC的输入时钟来自APB1, 时钟频率=42MHz=23.8ns
 *              DAC在输出buffer关闭的时候, 输出建立时间最小值: tSETTLING = 3us(F407数据手册有写)
 *              因此DAC输出的最高速度约为:333KHz, 以10个点为一个周期, 最大能输出33Khz左右的波形
 *
 * @param       outx: 要初始化的通道
 *      @arg    1, 初始化DAC通道1
 *      @arg    2, 初始化DAC通道2
 * @retval      无
 */
void dac_init(uint8_t outx)
{
    __HAL_RCC_DAC_CLK_ENABLE();                                 /* 使能DAC1的时钟 */
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE();                               /* 使能DAC OUT1/2的IO口时钟(都在PA口,PA4/PA5) */
    gpio_init_struct.Pin = (outx==1) ? GPIO_PIN_4 : GPIO_PIN_5; /* STM32单片机, 总是PA4=DAC1_OUT1, PA5=DAC1_OUT2 */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG; 
    gpio_init_struct.Pull = GPIO_NOPULL;      
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    g_dac1_handle.Instance = DAC;
    HAL_DAC_Init(&g_dac1_handle);                               /* 初始化DAC */

    DAC_ChannelConfTypeDef DACCH1_Config;
    DACCH1_Config.DAC_Trigger = DAC_TRIGGER_NONE;               /* 不使用触发功能 */
    DACCH1_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;  /* DAC1输出缓冲关闭 */
    
    switch(outx)
    {
        case 1:
            HAL_DAC_ConfigChannel(&g_dac1_handle, &DACCH1_Config, DAC_CHANNEL_1);  /* DAC通道1配置 */
            HAL_DAC_Start(&g_dac1_handle, DAC_CHANNEL_1);                          /* 开启DAC通道1 */
            break;
        case 2:
            HAL_DAC_ConfigChannel(&g_dac1_handle, &DACCH1_Config, DAC_CHANNEL_2);  /* DAC通道2配置 */
            HAL_DAC_Start(&g_dac1_handle, DAC_CHANNEL_2);                          /* 开启DAC通道1 */
            break;
        default : break;
    }
}


/**
 * @brief       设置通道1/2输出电压
 * @param       outx: 1,通道1; 2,通道2
 * @param       vol : 0~3300,代表0~3.3V
 * @retval      无
 */
void dac_set_voltage(uint8_t outx, uint16_t vol)
{
    double temp = vol;
    temp /= 1000;
    temp = temp * 4096 / 3.3;

    if (temp >= 4096)temp = 4095;   /* 如果值大于等于4096, 则取4095 */

    if (outx == 1)  /* 通道1 */
    {
        HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp);    /* 12位右对齐数据格式设置DAC值 */
    }
    else            /* 通道2 */
    {
        HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, temp);    /* 12位右对齐数据格式设置DAC值 */
    }
}

/**
 * @brief       设置DAC_OUT1输出三角波
 *   @note      输出频率 ≈ 1000 / (dt * samples) Khz, 不过在dt较小的时候,比如小于5us时, 由于delay_us
 *              本身就不准了(调用函数,计算等都需要时间,延时很小的时候,这些时间会影响到延时), 频率会偏小.
 *
 * @param       maxval : 最大值(0 < maxval < 4096), (maxval + 1)必须大于等于samples/2
 * @param       dt     : 每个采样点的延时时间(单位 : us)
 * @param       samples: 采样点的个数, samples必须小于等于(maxval + 1) * 2, 且maxval不能等于0
 * @param       n      : 输出波形个数,0~65535
 * @retval      无
 */
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n)
{
    uint16_t i, j;
    float incval;   /* 递增量 */
    float Curval;   /* 当前值 */
    
    if((maxval + 1) <= samples)return ;     /* 数据不合法 */
        
    incval = (maxval + 1) / (samples / 2);  /* 计算递增量 */
    
    for(j = 0; j < n; j++)
    { 
        Curval = 0;
        HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);  /* 先输出0 */
        for(i = 0; i < (samples / 2); i++)  /* 输出上升沿 */
        {
            Curval  +=  incval;             /* 新的输出值 */
            HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);
            delay_us(dt);
        } 
        for(i = 0; i < (samples / 2); i++)  /* 输出下降沿 */
        {
            Curval  -=  incval;             /* 新的输出值 */
            HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);
            delay_us(dt);
        }
    }
}

/***************************************DAC输出正弦波程序*****************************************/

DAC_HandleTypeDef g_dac_dma_handle;             /* DAC句柄 */
DMA_HandleTypeDef g_dma_dac_handle;             /* 与DAC关联的DMA句柄 */

extern uint16_t g_dac_sin_buf[4096];            /* 发送数据缓冲区 */

/**
 * @brief       DAC DMA输出波形初始化函数
 *   @note      本函数支持DAC1_OUT1/2通道初始化
 *              DAC的输入时钟来自APB1, 时钟频率=42MHz=23.8ns
 *              DAC在输出buffer关闭的时候, 输出建立时间最小值: tSETTLING = 3us(F407数据手册有写)
 *              因此DAC输出的最高速度约为:333KHz, 以10个点为一个周期, 最大能输出33Khz左右的波形
 *
 * @param       outx    : 要初始化的通道. 1,通道1; 2,通道2
 * @retval      无
 */
void dac_dma_wave_init(uint8_t outx)
{
    DAC_ChannelConfTypeDef dac_ch_conf={0};

    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOA_CLK_ENABLE();               /* DAC通道引脚端口时钟使能 */
    __HAL_RCC_DAC_CLK_ENABLE();                 /* DAC外设时钟使能 */
    __HAL_RCC_DMA1_CLK_ENABLE();                /* DMA时钟使能 */

    gpio_init_struct.Pin = (outx == 1) ? GPIO_PIN_4 : GPIO_PIN_5;   /* PA4/5 */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;   /* 模拟 */
    gpio_init_struct.Pull = GPIO_NOPULL;        /* 不带上下拉 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);    /* 初始化DAC引脚 */

    g_dac_dma_handle.Instance = DAC1;
    HAL_DAC_Init(&g_dac_dma_handle);            /* DAC初始化 */
    
    g_dma_dac_handle.Instance = (outx == 1) ? DMA1_Stream5 : DMA1_Stream6;  /* 使用的DAM1 Stream5/6 */
    g_dma_dac_handle.Init.Channel = DMA_CHANNEL_7;
    g_dma_dac_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;                 /* 存储器到外设模式 */
    g_dma_dac_handle.Init.PeriphInc = DMA_PINC_DISABLE;                     /* 外设地址禁止自增 */
    g_dma_dac_handle.Init.MemInc = DMA_MINC_ENABLE;                         /* 存储器地址自增 */
    g_dma_dac_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* 外设数据长度:16位 */
    g_dma_dac_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       /* 存储器数据长度:16位 */
    g_dma_dac_handle.Init.Mode = DMA_CIRCULAR;                              /* 循环模式 */
    g_dma_dac_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                   /* 中等优先级 */
    g_dma_dac_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;                  /* 不使用FIFO */
    HAL_DMA_Init(&g_dma_dac_handle);                                        /* 初始化DMA */

    __HAL_LINKDMA(&g_dac_dma_handle, DMA_Handle1, g_dma_dac_handle);        /* DMA句柄与DAC句柄关联 */

    dac_ch_conf.DAC_Trigger = DAC_TRIGGER_T7_TRGO;                          /* 采用定时器7触发 */
    dac_ch_conf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;                 /* 使能输出缓冲 */
    HAL_DAC_ConfigChannel(&g_dac_dma_handle, &dac_ch_conf, DAC_CHANNEL_1);  /* DAC通道输出配置 */
}

/**
 * @brief       DAC DMA使能波形输出
 *   @note      TIM7的输入时钟频率(f)来自APB1, f = 42 * 2 = 84Mhz.
 *              DAC触发频率 ftrgo = f / ((psc + 1) * (arr + 1))
 *              波形频率 = ftrgo / ndtr;
 * @param       outx        : DAC通道1/2
 * @param       ndtr        : DMA通道单次传输数据量
 * @param       arr         : TIM7的自动重装载值
 * @param       psc         : TIM7的分频系数
 * @retval      无
 */
void dac_dma_wave_enable(uint8_t outx, uint16_t ndtr, uint16_t arr, uint16_t psc)
{
    TIM_HandleTypeDef tim7_handle = {0};
    TIM_MasterConfigTypeDef master_config = {0};

    __HAL_RCC_TIM7_CLK_ENABLE();                                        /* TIM7时钟使能 */

    tim7_handle.Instance = TIM7;                                        /* 选择定时器7 */
    tim7_handle.Init.Prescaler = psc;                                   /* 分频系数 */
    tim7_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                  /* 向上计数 */
    tim7_handle.Init.Period = arr;                                      /* 重装载值 */
    tim7_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* 自动重装 */
    HAL_TIM_Base_Init(&tim7_handle);                                    /* 初始化定时器7 */

    master_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
    master_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&tim7_handle, &master_config);/* 配置TIM7 TRGO */
    HAL_TIM_Base_Start(&tim7_handle);                                   /* 使能定时器7 */

    HAL_DAC_Stop_DMA(&g_dac_dma_handle, (outx == 1)  ? DAC_CHANNEL_1 : DAC_CHANNEL_2); /* 先停止之前的传输 */
    HAL_DAC_Start_DMA(&g_dac_dma_handle, (outx == 1) ? DAC_CHANNEL_1 : DAC_CHANNEL_2, (uint32_t *)g_dac_sin_buf, ndtr, DAC_ALIGN_12B_R);
}

