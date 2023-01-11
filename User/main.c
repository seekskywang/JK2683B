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
	
	SetSystemStatus(SYS_STATUS_POWER);//开机上电状态
    while(1)
    {  		
		switch(GetSystemStatus())

		{
			case SYS_STATUS_POWER:
				
				Power_Process();//开机上电处理
				break;
			case SYS_STATUS_TEST:   //测量显示

                
				Test_Process();//测试处理
				break;
//			case SYS_STATUS_RANGE:
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//				Range_Process();//挡号显示
//				break;
//			case SYS_STATUS_RANGECOUNT: //档计数显示
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//			    Range_CountProcess();	
//				break;
//			case SYS_STATUS_ITEM:	 //列表扫描显示
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//				ItemProcess();
//				break;
//			case SYS_STATUS_FILE:   //还没有文件管理的子程序
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//				File_Process();//文件管理
//				break;
//			case SYS_STATUS_DATASTORE://还没有文件管理的子程序
//				Data_StoreProcess();//数据保存
//				break;

			case SYS_STATUS_SETUPTEST:
                
				Setup_Process();//设置处理
				break;
			case SYS_STATUS_USERDEBUG: //用户校正界面
                
				Use_DebugProcess();
				break;
//            case SYS_STATUS_LIMITSET:  //极限扫描设置
////                lcd_Clear(LCD_COLOR_TEST_BACK);
////                Use_LimitSetProcess();
//                break;
//            case SYS_STATUS_ITEMSET: //列表扫描设置
//                lcd_Clear(LCD_COLOR_TEST_BACK);
//                Use_ITEMSetProcess();
//                break;
            case SYS_STATUS_SYSSET : //系统设置
                
                Use_SysSetProcess();
                break;
//			case SYS_STATUS_TOOL://工具
//				Soft_Turnon();
//				
//			break;
			case SYS_STATUS_SYS://系统信息显示
				
				Sys_Process();
				break;
            case SYS_STATUS_IDEM://等待测试
                Idem_Process();
                
                break;
            case SYS_STATUS_ABORT://暂停测试
                Abroat_Process();
                break;
            case SYS_STATUS_FINISH://测试完成
                Finsh_Process();
                break;
			default:
//				Disp_Clr( );//清屏
//				Error_Process();//错误处理
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

