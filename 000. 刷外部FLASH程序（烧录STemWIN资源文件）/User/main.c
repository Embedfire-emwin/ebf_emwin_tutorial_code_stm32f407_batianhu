/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ڴ�SD�����������ļ���FLASH�Ĺ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/**
  ******************************************************************************
  *                              ͷ�ļ�
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./Bsp/led/bsp_led.h" 
#include "./BSP/key/bsp_key.h" 
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./BSP/flash/bsp_spi_flash.h"
#include "ff.h"
#include "./res_mgr/res_mgr.h"


/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
extern FATFS sd_fs;													/* Work area (file system object) for logical drives */
extern FATFS flash_fs;

//Ҫ���Ƶ��ļ�·������aux_data.c�޸�
extern char src_dir[];


/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int main(void)
{ 	
  FRESULT res = FR_OK;
  
	/* ��ʼ��LED */
  LED_GPIO_Config();
  
  Key_GPIO_Config();

  /* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
  Debug_USART_Config();

  SPI_FLASH_Init();
  
  res = FR_DISK_ERR;
       
  //����SD��
  res = f_mount(&sd_fs,SD_ROOT,1);

  //����ļ�ϵͳ����ʧ�ܾ��˳�
  if(res != FR_OK)
  {
    BURN_ERROR("f_mount ERROR!������������SD��Ȼ�����¸�λ������!");
    LED_RED;
    while(1);
  }    
    
  printf("\r\n ��һ��KEY1��ʼ��д�ֿⲢ�����ļ���FLASH�� \r\n"); 
  printf("\r\n ע��ò������FLASH��ԭ���ݻᱻɾ������ \r\n"); 
  
  while(Key_Scan(GPIOA,GPIO_Pin_0)==0);
  
  printf("\r\n ���ڽ�����Ƭ������ʱ��ܳ��������ĵȺ�...\r\n");
  SPI_FLASH_BulkErase();    
  
  /* ������¼Ŀ¼��Ϣ�ļ� */
  Make_Catalog(src_dir,0);
  
  /* ��¼ Ŀ¼��Ϣ��FLASH*/
  Burn_Catalog();  
  /* ���� Ŀ¼ ��¼������FLASH*/
  Burn_Content();
  /* У����¼������ */
  Check_Resource();
  
//  printf("��ȡ��Դλ�ò���\r\n");
//  printf("xinsongti18.xbf offset = %d\r\n", 
//                  GetResOffset("xinsongti18.xbf"));

  while(1);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
