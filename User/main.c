/**********************************************************************
* $Id$		Gpio_LedBlinky.c		2011-06-02
*//**
* @file		Gpio_LedBlinky.c
* @brief	This example describes how to use GPIO interrupt to drive
* 			LEDs
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_clkpwr.h"
#include "EX_SDRAM/EX_SDRAM.H"
#include "LCD/lpc177x_8x_lcd.h"
#include "LCD/logo.h"
#include "TouchPanel/TouchPanel.h"
#include "UseDisp/Use_Disp.h"
#include "Key/key.h"
#include "stdio.h"
#include "TypeDefine.h"	
#include "test/Test.h"
#include "GlobalValue.h"
#include "lpc177x_8x_eeprom.h"
#include "usbhost_main.h"
//#include  "image.h"

/** @defgroup GPIO_LedBlinky	GPIO Blinky
 * @ingroup GPIO_Examples
 * @{
 */
/* Private variables ---------------------------------------------------------*/


/************************** PRIVATE DEFINITIONS *************************/

/************************** PRIVATE VARIABLES *************************/
/* SysTick Counter */
//volatile unsigned long SysTickCnt;

/************************** PRIVATE FUNCTIONS *************************/
//void SysTick_Handler (void);

//void Delay (unsigned long tick);

void lcd_test(void);
uint32_t color_table[]={0x000000,0xFFFFFF,0x0000FF,0x00FF00,0xFF0000};

/* Private variables ---------------------------------------------------------*/
extern Matrix matrix ;
extern Coordinate  display ;

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: Main program body
 * @param[in]	None
 * @return 		int700097
 **********************************************************************/
int main(void)
{

	uint32_t cclk = CLKPWR_GetCLK(CLKPWR_CLKTYPE_CPU);
	softswitch=0;
	SysTick_Config(cclk/1000 - 1); 
//	EEPROM_Init();
	HW_keyInt();
	
	SetSystemStatus(SYS_STATUS_POWER);//�����ϵ�״̬
    while(1)
    {  		
		switch(GetSystemStatus())

		{
			case SYS_STATUS_POWER:
				
				Power_Process();//�����ϵ紦��
				break;
			case SYS_STATUS_TEST:   //������ʾ

                
				Test_Process();//���Դ���
				break;
//			case SYS_STATUS_RANGE:
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//				Range_Process();//������ʾ
//				break;
//			case SYS_STATUS_RANGECOUNT: //��������ʾ
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//			    Range_CountProcess();	
//				break;
//			case SYS_STATUS_ITEM:	 //�б�ɨ����ʾ
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//				ItemProcess();
//				break;
//			case SYS_STATUS_FILE:   //��û���ļ�������ӳ���
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//				File_Process();//�ļ�����
//				break;
//			case SYS_STATUS_DATASTORE://��û���ļ�������ӳ���
//				Data_StoreProcess();//���ݱ���
//				break;

			case SYS_STATUS_SETUPTEST:
                
				Setup_Process();//���ô���
				break;
			case SYS_STATUS_USERDEBUG: //�û�У������
                
				Use_DebugProcess();
				break;
//            case SYS_STATUS_LIMITSET:  //����ɨ������
////                lcd_Clear(LCD_COLOR_TEST_BACK);
////                Use_LimitSetProcess();
//                break;
//            case SYS_STATUS_ITEMSET: //�б�ɨ������
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//                Use_ITEMSetProcess();
//                break;
            case SYS_STATUS_SYSSET : //ϵͳ����
                
                Use_SysSetProcess();
                break;
//			case SYS_STATUS_TOOL://����
//				Soft_Turnon();
//				
//			break;
			case SYS_STATUS_SYS://ϵͳ��Ϣ��ʾ
				
				Sys_Process();
				break;
            case SYS_STATUS_IDEM://�ȴ�����
                Idem_Process();
                
                break;
            case SYS_STATUS_ABORT://��ͣ����
                Abroat_Process();
                break;
            case SYS_STATUS_FINISH://�������
                Finsh_Process();
                break;
			default:
//				Disp_Clr( );//����
//				Error_Process();//������
				break;
		}

    }
}



/*
**************************************************************************************************************
*                                      WRITE DATA TO DISK
*
* Description: This function is used by the user to write data to disk
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Main_Write (void)
{
    int32_t  fdw;
    int32_t  fdr;
    uint32_t  tot_bytes_written;
    uint32_t  bytes_written;


    fdr = FILE_Open(FILENAME_R, RDONLY);
    if (fdr > 0) {
        FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
        fdw = FILE_Open(FILENAME_W, RDWR);
        if (fdw > 0) {
            tot_bytes_written = 0;
           // PRINT_Log("Writing to %s...\n", FILENAME_W);
            do {
                bytes_written = FILE_Write(fdw, UserBuffer, MAX_BUFFER_SIZE);
                tot_bytes_written += bytes_written;
            } while (tot_bytes_written < WRITE_SIZE);
            FILE_Close(fdw);
           // PRINT_Log("Write completed\n");
        } else {
            //PRINT_Log("Could not open file %s\n", FILENAME_W);
            return;
        }
        FILE_Close(fdr);
    } else {
       // PRINT_Log("Could not open file %s\n", FILENAME_R);
        return;
    }
}

