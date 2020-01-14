/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 �̼���ʵ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ������Ӳ��bspͷ�ļ� */
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./key/bsp_key.h"
#include "./lcd/bsp_ili9806g_lcd.h"
//#include "./flash/bsp_spi_flash.h"
#include "./TouchPad/bsp_touchpad.h"
#include "./beep/bsp_beep.h" 
#include "./sram/bsp_sram.h"	  
#include "./touch/gt5xx.h"
/* STemWINͷ�ļ� */
#include "GUI.h"
#include "DIALOG.h"


/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
/* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED������ */
static TaskHandle_t LED_Task_Handle = NULL;
/* Touch������ */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI������ */
static TaskHandle_t GUI_Task_Handle = NULL;


/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */
SemaphoreHandle_t ScreenShotSem_Handle = NULL;

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void LED_Task(void* parameter);/* LED_Task����ʵ�� */
static void GUI_Task(void* parameter);/* GUI_Task����ʵ�� */
static void Touch_Task(void* parameter);
static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  /* ������Ӳ����ʼ�� */
  BSP_Init();
  
  printf("\r\n ********** emwin DEMO *********** \r\n");
  
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	taskENTER_CRITICAL();//�����ٽ���
	
	/* ����ScreenShotSem�ź��� */
	ScreenShotSem_Handle = xSemaphoreCreateBinary();
	if(NULL != ScreenShotSem_Handle)
		printf("ScreenShotSem��ֵ�ź��������ɹ���\r\n");
  
	xReturn = xTaskCreate((TaskFunction_t)LED_Task,/* ������ں��� */
											 (const char*    )"LED_Task",/* �������� */
											 (uint16_t       )128,       /* ����ջ��С */
											 (void*          )NULL,      /* ������ں������� */
											 (UBaseType_t    )4,         /* ��������ȼ� */
											 (TaskHandle_t   )&LED_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����LED1_Task����ɹ���\r\n");
  
  xReturn = xTaskCreate((TaskFunction_t)Touch_Task,/* ������ں��� */
											 (const char*      )"Touch_Task",/* �������� */
											 (uint16_t         )256,     /* ����ջ��С */
											 (void*            )NULL,    /* ������ں������� */
											 (UBaseType_t      )3,       /* ��������ȼ� */
											 (TaskHandle_t     )&Touch_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����Touch_Task����ɹ���\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)GUI_Task,/* ������ں��� */
											 (const char*      )"GUI_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )2,         /* ��������ȼ� */
											 (TaskHandle_t     )&GUI_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����GUI_Task����ɹ���\r\n");
	
	vTaskDelete(AppTaskCreate_Handle);//ɾ��AppTaskCreate����
	
	taskEXIT_CRITICAL();//�˳��ٽ���
}

/**
  * @brief LED��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void LED_Task(void* parameter)
{
	while(1)
	{
		LED3_TOGGLE;
		vTaskDelay(1000);
	}
}

/**
  * @brief ���������������
  * @note ��
  * @param ��
  * @retval ��
  */
static void Touch_Task(void* parameter)
{
  /* �ȴ��ź��� */
	xSemaphoreTake(ScreenShotSem_Handle,/* ��ֵ�ź������ */
								 portMAX_DELAY);/* �����ȴ� */  
	while(1)
	{
		GTP_TouchProcess();//��������ʱɨ��
		vTaskDelay(20);
	}
}

/**
  * @brief GUI��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void GUI_Task(void* parameter)
{
  
	/* ��ʼ��STemWin */
  GUI_Init();
  /* ��������ʼ�� */
  GTP_Init_Panel();
  /* �����ź��� */
  xSemaphoreGive(ScreenShotSem_Handle);
  /* ��LCD����� */
  ILI9806G_BackLed_Control ( ENABLE );
	while(1)
	{
		MainTask();
	}
}

/**
  * @brief �弶�����ʼ��
  * @note ���а����ϵĳ�ʼ�����ɷ��������������
  * @param ��
  * @retval ��
  */
static void BSP_Init(void)
{
  /* SRAM��ʼ�� */
  FSMC_SRAM_Init();
  
  /* CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı���������
   * ����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ�
   */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
  
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
	LED_GPIO_Config();

  /* ��������ʼ�� */
  BEEP_GPIO_Config();
  
	/* ���ڳ�ʼ��	*/
	Debug_USART_Config();
  
  
}


/********************************END OF FILE****************************/
