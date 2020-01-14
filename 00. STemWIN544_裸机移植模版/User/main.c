/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ����ģ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"  
#include "./sram/bsp_sram.h"	  
#include "./lcd/bsp_ili9806g_lcd.h"
#include "./systick/bsp_SysTick.h"
#include "./touch/gt5xx.h"

#include "GUI.h"

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int main(void)
{
  //��ʼ���ⲿSRAM  
  FSMC_SRAM_Init();
  
  /*CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı��������ģ�����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);  
  
  /* LED��ʼ�� */
	LED_GPIO_Config();

	/* ���ô���1Ϊ��115200 8-N-1 */
	Debug_USART_Config();
  
  /* ��ʼ��GUI */
	GUI_Init();
  
  /* ��������ʼ�� */
  GTP_Init_Panel();
  
  /* ��ʼ����ʱ�� */
	SysTick_Init();
  
  GUI_CURSOR_Show();
  
  printf("\r\n ********** emwin DEMO *********** \r\n");
  
  
  while(1)
  {
    MainTask();
  }
}


/*********************************************END OF FILE**********************/

