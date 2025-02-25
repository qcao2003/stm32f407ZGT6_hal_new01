/**
 ***************************************************************************************************
 * 实验简介
 * 实验名称：USB U盘(HOST) 实验
 * 实验平台：正点原子 探索者F407开发板
 * 实验目的：学习STM32的USB HOST功能使用,实现对U盘的读写

 ***************************************************************************************************
 * 硬件资源及引脚分配
 * 1 LED灯
     DS0（RED）     : LED0 - PF9
     DS1（GREEN）   : LED1 - PF10
 * 2 独立按键
     KEY0    - PE4
     KEY1    - PE3
     KEY2    - PE2
     KEY_UP  - PA0  (程序中的宏名:WK_UP)
 * 3 串口1 (PA9/PA10连接在板载USB转串口芯片CH340上面)
 * 4 ALIENTEK 2.8/3.5/4.3/7/10寸TFTLCD模块(仅限MCU屏，16位8080并口驱动)
 * 5 外部SRAM芯片,通过FSMC驱动
 * 6 nor flash(W25QXX，通过SPI1驱动)
 * 7 USB_HOST接口(D-/D+连接在PA11/PA12上)
 
 ***************************************************************************************************
 * 实验现象
 * 1 本实验代码，开机后，检测字库，然后初始化USB HOST，并不断轮询。当检测并识别U盘后，
     在LCD上面显示U盘总容量和剩余容量，此时便可以通过USMART调用FATFS相关函数，来测试
     U盘数据的读写了，方法同FATFS实验一模一样。
 * 2 LED0闪烁，提示程序运行。LED1指示USB连接状态: USB连接，LED1常亮，USB未连接，LED1灭。

 ***************************************************************************************************
 * 注意事项
 * 1 电脑端串口调试助手波特率必须是115200
 * 2 请使用XCOM/SSCOM串口调试助手,其他串口助手可能控制DTR/RTS导致MCU复位/程序不运行
 * 3 串口输入字符串以回车换行结束
 * 4 请用USB线连接在USB_UART,找到USB转串口后测试本例程
 * 5 P10的PA9/PA10必须通过跳线帽连接在RXD/TXD上
 * 6 本例程仅支持MCU屏，不支持RGB屏
 * 7 4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
 * 8 本实验如果需要用串口功能需要另外一根线插到USB_UART接口，另一端接电脑USB
 * 9 U盘插在USB接口上，USB SLAVE 口不要插 USB 线到电脑，否则会干扰

 ***********************************************************************************************************
 * 公司名称：广州市星翼电子科技有限公司（正点原子）
 * 电话号码：020-38271790
 * 传真号码：020-36773971
 * 公司网址：www.alientek.com
 * 购买地址：zhengdianyuanzi.tmall.com
 * 技术论坛：http://www.openedv.com/forum.php
 * 最新资料：www.openedv.com/docs/index.html
 *
 * 在线视频：www.yuanzige.com
 * B 站视频：space.bilibili.com/394620890
 * 公 众 号：mp.weixin.qq.com/s/y--mG3qQT8gop0VRuER9bw
 * 抖    音：douyin.com/user/MS4wLjABAAAAi5E95JUBpqsW5kgMEaagtIITIl15hAJvMO8vQMV1tT6PEsw-V5HbkNLlLMkFf1Bd
 ***********************************************************************************************************
 */