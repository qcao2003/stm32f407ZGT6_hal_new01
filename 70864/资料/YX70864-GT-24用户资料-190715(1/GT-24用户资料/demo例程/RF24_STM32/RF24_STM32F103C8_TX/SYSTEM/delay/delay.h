/*
    GT-24 V1.0 (RF24L01) ���Կ�������

    ���ߣ����ƿƼ����޹�˾
    �Ա�����:https://shop532831232.taobao.com
	
    ����оƬ�� STM32f103ϵ�� 
	
   ��·���ӣ�  RF24L01�� VCC        3.3V
			            GND        GND
			            CE         A0
						IRQ        A1
                        CSN        B12
						SCK        B13
						MOSI       B15
						MISO       B14
						
	  		 OLED(IIC): VCC        3.3V
					 	GND        GND
					 	SCL        A5
					 	SDA        A7
						
    ����޸�ʱ�䣻2018��8��1�� 12:00:00
  ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
*/
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"
//********************************************************************************
//V1.2�޸�˵��
//�������ж��е��ó�����ѭ���Ĵ���
//��ֹ��ʱ��׼ȷ,����do while�ṹ!

//V1.3�޸�˵��
//�����˶�UCOSII��ʱ��֧��.
//���ʹ��ucosII,delay_init���Զ�����SYSTICK��ֵ,ʹ֮��ucos��TICKS_PER_SEC��Ӧ.
//delay_ms��delay_usҲ���������ucos�ĸ���.
//delay_us������ucos��ʹ��,����׼ȷ�Ⱥܸ�,����Ҫ����û��ռ�ö���Ķ�ʱ��.
//delay_ms��ucos��,���Ե���OSTimeDly����,��δ����ucosʱ,������delay_usʵ��,�Ӷ�׼ȷ��ʱ
//����������ʼ������,��������ucos֮��delay_ms������ʱ�ĳ���,ѡ��OSTimeDlyʵ�ֻ���delay_usʵ��.

//V1.4�޸�˵�� 20110929
//�޸���ʹ��ucos,����ucosδ������ʱ��,delay_ms���ж��޷���Ӧ��bug.
//V1.5�޸�˵�� 20120902
//��delay_us����ucos��������ֹ����ucos���delay_us��ִ�У����ܵ��µ���ʱ��׼��
////////////////////////////////////////////////////////////////////////////////// 	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























