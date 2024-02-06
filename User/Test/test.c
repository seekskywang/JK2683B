#include "test.h"
#include <stdlib.h>
#include "LCD/lpc177x_8x_lcd.h"
#include "Globalvalue/GlobalValue.h"
#include "use_disp.h"
#include "EX_SDRAM/EX_SDRAM.H"
#include "stdint.h"
#include "key/key.h"
//#include "uart.h"
#include "timer.h"
#include "lpc177x_8x_eeprom.h"
#include "math.h"
#include  "usbhost_fat.h"
#include "lpc177x_8x_clkpwr.h"
#include "LCD/logo.h"
//#include "LCD/lpc177x_8x_lcd.h"
#include "debug_frmwrk.h"
#include <string.h>
#include "open.h"
//#include "lpc177x_8x_rtc.h"
const uint8_t USB_dISPVALUE[][9]=
{
	"RH_FAIL ",
	"RL_FAIL ",
	"VH_FAIL ",
	"VL_FAIL ",
	"ALL_FAIL",
	"ALL_PASS"
};

uint32_t keynum;
float risitvalue;
const u8 DOT_POS[6]=
{	
	2,
	1,
	3,
	2,
	0
};
//#include "debug_frmwrk.h"
const vu8 Uart_Ordel[]={0x60,0x70,0x71,0x80,0x90,0xa0,0xb0,0xc0,0xe0};
const vu8 READDATA[7]={0xAB,0x01,0x06,0x03,0x08,0xbf,'\0'};
const vu8 Disp_Main_Ord[][3]={
	{1,1,0},
	{1,1,1},
	{1,1,2},
	{1,1,3},//Cp
	
	{0,1,0},
	{0,1,1},
	{0,1,3},//Cs
	{1,0,0},
	{1,0,0},
	{1,0,0},
	{1,0,1},//Lp
	{1,0,3},
	{1,0,4},
	{1,0,0},
	{0,0,0},//Ls
	{0,0,1},
	{0,0,5},
	{0,0,4},
	{0,3,6},//Z
	{0,3,7},
	{0,4,6},//Y
	{0,4,7},
	{0,2,8},//R
	{1,2,1},
	{0,2,1},
	{1,1,0},
	{1,5,9},//GB
	//{1,1,0},
	};


//==========================================================
//函数名称：Power_Process
//函数功能：上电处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.26 08:53
//备注说明：开机长按SET进入校准调试模式
//==========================================================

void Power_Process(void)
{

	u8 rc;
    Pc_Sendflag=0;
	HW_keyInt();
	GPIO_Led_Configuration();
	debug_frmwrk_init();
	
	SDRAM_Init();
	GLCD_Ctrl (FALSE);
	GLCD_Init (LogoPic.pPicStream, NULL);
	GLCD_Ctrl (TRUE);
	lcd_image((uint8_t *)gImage_open);
    GPIO_Led_Configuration();
    GPIO_LPC_Configration();
	Beep_on();
	HW_Sendvalueto164(0);
	Bais_LedOff();
	Lock_LedOff();
	Pass_Led();
//	Power_Off_led();
	
//	lcd_image((uint8_t *)gImage_open);
	
	InitGlobalValue();//初始化全局变量
	
	
	
	Uart_Send_Flag=0;
	EEPROM_Init();
//	Set_Compbcd_float();
	ReadSavedata();
    Uart3_init(Tft_5520.Sys_Set.Buad);
	SetData_High_Low_Comp();
    init_timer(0, 20);//定时器初始化
    enable_timer(0);
	//if(Save_Res.Sys_Setvalue.U_store)
	{
		Host_Init();               /* Initialize the lpc17xx host controller                                    */
    rc = Host_EnumDev();       /* Enumerate the device connected                                            */
    if (rc == OK) {
		/* Initialize the mass storage and scsi interfaces */
        rc = MS_Init( &blkSize, &numBlks, inquiryResult );
        if (rc == OK) {
            rc = FAT_Init();   /* Initialize the FAT16 file system */    
		Write_Usbdata ( "电压     时间      电阻      分选\r\n" ,35);			
			} 
		} 
	}

	Delay(1000);
    Send_UartStop();
    Delay(1000);
    //HW_KeyScsn();
//	NVIC_EnableIRQ(GPIO_IRQn);
	if(HW_KeyScsn()==10)
		SetSystemStatus(SYS_STATUS_USERDEBUG);
	Beep_Off();
//UART_TxCmd(LPC_UART3, ENABLE);
	while(GetSystemStatus()==SYS_STATUS_POWER)
	{
//		i++;
//		if(i>POWERON_DISP_TIME)//延时20*100mS=2s后自动退出
			SetSystemStatus(SYS_STATUS_IDEM);//待测状态

//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);//等待按键(100*10ms/10=100ms)
//		switch(key)
//		{
////			case KEY_SET:		//设置键
//			case L_KEY_SET:		//长按设置键
//				SetSystemStatus(SYS_STATUS_DEBUG);//调试状态
//				Beep_One();//蜂鸣器响一声
//				break;
//	
//			case KEY_UP:		//上键
//			case L_KEY_UP:		//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;
//	
//			case KEY_LEFT:		//左键
//			case L_KEY_LEFT:	//长按左键
//				break;
//
//			case KEY_RIGHT:		//右键
//			case L_KEY_RIGHT:	//长按右键
//				break;
//
//			case KEY_ENTER:		//确认键
//			case L_KEY_ENTER:	//长按确认键
//				break;
//	
//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				break;
//	
//			case KEY_RESET:		//复位键
//			case L_KEY_RESET:	//长按复位键
//				i=POWERON_DISP_TIME;//显示延时结束
//				break;
//			
//			default:
//				break;
//		}
		
		//Delay_1ms(50);
		//Range_Control(3);
	}
//    Send_Set_Value();
//    Send_Set_Value();
    //Send_Set_Value();
}
vu16 ParameterLimit[][2]=
{
    {0,1},
    {0,GROUP_MAX-1},
    {0,1},
    {10,1000},
    {0,9999},
    {0,1},
    {0,1},
    {0,RANGE_MAX},
    {0,2},
    {0,9999},
    {0,9999},
    {0,1},
};
//待测界面
void Idem_Process(void)
{
	Button_Page_Typedef Button_Page;
	vu32 keynum=0;
    vu8 rc;
	vu8 key,i;
	vu8 Disp_Flag=1;
//	vu8 lock_flag=0;
    vu16 *pt;

	Button_Page.page=0;
	Button_Page.index=0;//
	Button_Page.third=0xff;
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_Test_Item();
	All_LedOff1();
	Delay_Key();
    
	clear_flag=0;
    Send_Set_Value();
    Beep_Off();
    Disp_Teststatue(0);
	Disp_R_X();
	while(GetSystemStatus()==SYS_STATUS_IDEM)
	{
        
		if(Pc_Sendflag)//如果上位机发送设置数据，侧保存并转发到下位机
        {
            Pc_Sendflag=0;
            Send_Set_Value();
            Savetoeeprom();//保存设置数据
            Disp_Flag=1;
        
        }
		if(Disp_Flag==1)
		{
			Disp_Test_value(&Button_Page);
			
			Disp_Flag=0;
		
		}
		
		Colour.black=LCD_COLOR_TEST_MID;
        
        for(i=0;i<6;i++)
        DispBuf[i]=0;
        for(i=0;i<5;i++)
        DispBuf[i]=Send_Testvalue[0].dianzu[i];
        Tft_5520_Dispvalue.Range=Send_Testvalue[0].range;
        if(Send_Testvalue[0].king==0xf2)
        {
            WriteString_Big(60+40, 92, DispBuf);
            Disp_Range(0);
            Disp_Teststatue(9);                      
        }
        else
        {
            Disp__();
            if(Send_Testvalue[0].king==5)
            {
                Send_Testvalue[0].king=0;
                Disp_Flag=1;
                Disp_Teststatue(0);           
            }            
        }
        
	  key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	
			Key_Beep();
			switch(key)
			{
				case Key_FAST:
                     //SetSystemStatus(SYS_STATUS_TEST);
                    if(Tft_5520.Sys_Set.U_Switch)
                     {
                         Host_Init();               /* Initialize the lpc17xx host controller                                    */
                        rc = Host_EnumDev();       /* Enumerate the device connected                                            */
                        if (rc == OK) {
                            /* Initialize the mass storage and scsi interfaces */
                            rc = MS_Init( &blkSize, &numBlks, inquiryResult );
                            if (rc == OK) {
                                rc = FAT_Init();   /* Initialize the FAT16 file system */    
                            Write_Usbdata ( "电压     时间      电阻      分选\r\n" ,35);			
                                } 
                            } 
                        }
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
               
                case Key_VSET:
                    Disp_Flag=1;
                    if(Button_Page.index==5)
                        Button_Page.index=0;
                    else
                        Button_Page.index=5;
                        pt=(vu16* )&Tft_5520.Set_Value[Tft_5520.Group] + Button_Page.index-4;
                        NumBox.Num=*pt;
                        NumBox.Max=ParameterLimit[Button_Page.index-2][1];//Max
                        NumBox.Min=ParameterLimit[Button_Page.index-2][0];//Min
                        NumBox.Dot=0;//Dot
                    
                    break;
				case Key_EXIT:
                    Disp_Flag=1;
                    Button_Page.index=0;
                    //清零
                    Send_UartClear();
                    
				break;
				case Key_RIGHT:
                    Disp_Flag=1;
                    switch(Button_Page.index)
                    {
                        case 0:
                            SetSystemStatus(SYS_STATUS_SYS);
                            
                            break;
                        case 1:
                            if(Tft_5520.Group<1)
                                 Tft_5520.Group=4;
                            else
                            
                                Tft_5520.Group--;
                            Send_Set_Value();
                            break;
                        case 2:
                            LcdAddr.x=LIST1+88;
                            LcdAddr.y=FIRSTLINE+SPACE1*1;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            Send_Set_Value();
                            break;
                        case 3:
                            LcdAddr.x=LIST1+88;
                            LcdAddr.y=FIRSTLINE+SPACE1*2;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            Send_Set_Value();
                        case 4:
                            break;
                        case 5:
                            LcdAddr.x=LIST2+88+10;
                            LcdAddr.y=FIRSTLINE+SPACE1*1;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            Send_Set_Value();
                            break;
                        case 6:
                            break;
                        default:
                            break;
                    }
				break;
				case Key_LEFT:
                     Disp_Flag=1;
                    switch(Button_Page.index)
                    {
                        case 0:
                            SetSystemStatus(SYS_STATUS_SETUPTEST);
                            break;
                        case 1:
                            if(Tft_5520.Group>=4)
                                 Tft_5520.Group=0;
                            else
                            
                                Tft_5520.Group++;
                            
                            Send_Set_Value();
                            break;
                        case 2:
                            LcdAddr.x=LIST1+88;
                            LcdAddr.y=FIRSTLINE+SPACE1*1;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            Send_Set_Value();
                            break;
                        case 3:
                            LcdAddr.x=LIST1+88;
                            LcdAddr.y=FIRSTLINE+SPACE1*2;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            Send_Set_Value();
                            break;
                        case 4:
                            break;
                        case 5:
                            LcdAddr.x=LIST2+88+10;
                            LcdAddr.y=FIRSTLINE+SPACE1*1;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            Send_Set_Value();
                            break;
                        case 6:
                            break;
                        default:
                            break;
                    }
				break;
				case Key_DOWN:
					Disp_Flag=1;
					if(Button_Page.index>0)
						Button_Page.index=0;
					else
						Button_Page.index=1;					
				break;
				case Key_UP:
					Disp_Flag=1;
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
						Button_Page.index=0;
				break;
				case Key_PAGE://翻页
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_STOP:
                    SetSystemStatus(SYS_STATUS_IDEM);
				break;
                case Key_START:
                    SetSystemStatus(SYS_STATUS_TEST);
                
                break;
				case Key_REST:
					Power_Off_led();
				break;
                
                case Key_COMP://7 8
                    Disp_Flag=1;
                    if(Button_Page.index==2)
                        Button_Page.index=3;
                    else if(Button_Page.index==3)
                        Button_Page.index=0;
                    else
                        Button_Page.index=2;
                        
                    pt=(vu16* )&Tft_5520.Set_Value[Tft_5520.Group] + Button_Page.index+5;
                    NumBox.Num=*pt;
                    NumBox.Max=ParameterLimit[Button_Page.index+7][1];//Max
                    NumBox.Min=ParameterLimit[Button_Page.index+7][0];//Min
                    NumBox.Dot=0;//Dot
                    
                break;
                
                
				default:
				break;
					
			}
		
		}
        Disp_Usbflag(usb_oenflag);
	}
    if(GetSystemStatus()==SYS_STATUS_TEST)
        Send_UartStart();

}

//暂停界面
void Abroat_Process(void)
{

	vu32 keynum=0;
//	float ddd,eee;
	vu8 key;
//	vu8 return_flag=0;
//	vu8 Disp_Flag=1;
//	vu8 uart_count;
//	vu8 trip_flag=0;
//	vu8 lock_flag=0,rc;

//	Send_Ord_Typedef Uart;

	Delay_Key();
//	uart_count=0;
	clear_flag=0;
//	Send_UartStart();//开始时的串口发送数据
	while(GetSystemStatus()==SYS_STATUS_ABORT)
	{
		
	
		
		Colour.black=LCD_COLOR_TEST_MID;
		

		Uart_Process();

		Colour.Fword=LCD_COLOR_WHITE;
		
	  key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	
			
			Key_Beep();
			switch(key)
			{
				
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    //SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
                
                case Key_COMP:
                   
                    
                    
                    break;
                case Key_VSET:
                   
                    break;
				case Key_EXIT:
                    
                    
				break;
				case Key_RIGHT:
					

				break;
				case Key_LEFT:
					

						
				break;
				case Key_DOWN:
					
					
				break;
				case Key_UP:
					
				break;
				

				case Key_PAGE://翻页
                   
					
				break;
				case Key_LOCK:
					
				break;
				case Key_STOP:
                    Send_UartStop();
                    SetSystemStatus(SYS_STATUS_IDEM);
					
				break;
                case Key_START:
                    Send_UartStart();
                    SetSystemStatus(SYS_STATUS_TEST);
                    
                break;
				case Key_REST:
					Power_Off_led();
				break;
				case Key_TRIG:
//					trip_flag=1;
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
    Send_UartStop();

}
//测试完成显示
void Finsh_Process(void)
{

	vu32 keynum=0;
//	float ddd,eee;
	vu8 key;
//	vu8 return_flag=0;
//	vu8 Disp_Flag=1;
//	vu8 uart_count;
//	vu8 trip_flag=0;
//	vu8 lock_flag=0,rc;

//	Send_Ord_Typedef Uart;

	Delay_Key();
//	uart_count=0;
	clear_flag=0;
//	Send_UartStart();//开始时的串口发送数据
	while(GetSystemStatus()==SYS_STATUS_FINISH)
	{
		
	
		if(Tft_5520.Sys_Set.Uart&&(Uart3_returnflag==1))//串口打开，往上位机发送数据
		{
				Uart3_returnflag=0;
				Uart3_Send();
		}
		Colour.black=LCD_COLOR_TEST_MID;
		

//		Uart_Process();

		Colour.Fword=LCD_COLOR_WHITE;
		
	  key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	
			
			Key_Beep();
			switch(key)
			{
				
				case Key_Disp:
                    //SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    //SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
                
                case Key_COMP:
                   
                    
                    
                    break;
                case Key_VSET:
                   
                    break;
				case Key_EXIT:
                    
                    
				break;
				case Key_RIGHT:
					

				break;
				case Key_LEFT:
					

						
				break;
				case Key_DOWN:
					
					
				break;
				case Key_UP:
					
				break;
				

				case Key_PAGE://翻页
                   
					
				break;
				case Key_LOCK:
					
				break;
				case Key_STOP:
                    //Send_UartStop();
                    SetSystemStatus(SYS_STATUS_IDEM);
					
				break;
                case Key_START:
                    //Send_UartStart();
                    SetSystemStatus(SYS_STATUS_TEST);
                    
                break;
				case Key_REST:
					Power_Off_led();
				break;
				case Key_TRIG:
//					trip_flag=1;
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
    if(GetSystemStatus()==SYS_STATUS_TEST)
        Send_UartStart();
    else
        if(GetSystemStatus()==SYS_STATUS_IDEM)
            Send_UartStop();
    All_LedOff1();

}
//将数据转换为发送给上位机的格式
void DataConvertSendToPC(void)
{
	u8 i;
	SendPC_Testvalue.start = UART_SEND_BEGIN;
	SendPC_Testvalue.end = UART_SEND_END;
//	SendPC_Testvalue.addr = Tft_5520.Sys_Set.Addr;
	if(Tft_5520.Set_Value[Tft_5520.Group].voltage < 100)
	{
		SendPC_Testvalue.voltage[0] = Tft_5520.Set_Value[Tft_5520.Group].voltage/10+0x30;
		SendPC_Testvalue.voltage[1] = Tft_5520.Set_Value[Tft_5520.Group].voltage%10+0x30;
		SendPC_Testvalue.voltage[2] = '.';
		SendPC_Testvalue.voltage[3] = '0';
		SendPC_Testvalue.voltage[4] = '0';
	}else{
		if(Tft_5520.Set_Value[Tft_5520.Group].voltage < 1000)
		{
			SendPC_Testvalue.voltage[0] = Tft_5520.Set_Value[Tft_5520.Group].voltage/100+0x30;
			SendPC_Testvalue.voltage[1] = Tft_5520.Set_Value[Tft_5520.Group].voltage%100/10+0x30;
			SendPC_Testvalue.voltage[2] = Tft_5520.Set_Value[Tft_5520.Group].voltage%10+0x30;
			SendPC_Testvalue.voltage[3] = '.';
			SendPC_Testvalue.voltage[4] = '0';
		}else{			
			memcpy(&SendPC_Testvalue.voltage[0],"1000",4);
			SendPC_Testvalue.voltage[4]=0;
		}
	}
//	SendPC_Testvalue.voltage[0]='0';
//	memcpy(&SendPC_Testvalue.voltage[1],(char *)Send_Testvalue[0].dianya,4);
	if(Tft_5520_Dispvalue.Range == 0)
	{
		memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[1],5);
	}else if(Tft_5520_Dispvalue.Range == 1){
		memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[2],4);
		SendPC_Testvalue.risistence[4] = '0';
	}else if(Tft_5520_Dispvalue.Range == 2){
		if(risitvalue< 100)
		{
			memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[2],4);
			SendPC_Testvalue.risistence[4] = '0';
		}else{
			memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[1],5);
		}
	}else if(Tft_5520_Dispvalue.Range == 3){
		if(risitvalue < 1000)
		{
			memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[2],3);
			SendPC_Testvalue.risistence[3] = '.';
			SendPC_Testvalue.risistence[4] = '0';
		}else{
			memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[1],4);
			SendPC_Testvalue.risistence[4] = '.';
		}
	}
//	if(risitvalue >= 100 && risitvalue < 1000)
//	{
//		memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[1],3);
//		SendPC_Testvalue.risistence[3] = '.';
//		SendPC_Testvalue.risistence[4] = '0';
//	}else if(risitvalue >= 10 && risitvalue < 100){
//		memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[2],4);
//		SendPC_Testvalue.risistence[4] = '0';
//	}else if(risitvalue >= 1 && risitvalue < 10){
//		memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[2],4);
//		SendPC_Testvalue.risistence[4] = '0';
//	}else if(risitvalue >= 1000){
//		memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[1],5);
//		SendPC_Testvalue.risistence[4] = '.';
//	}else{
//		memcpy(&SendPC_Testvalue.risistence[0],(char *)&Send_Testvalue[0].dianzu[1],5);
//	}
	
		if(SendPC_Testvalue.risistence[0] == 0x5e)//开路
		{
			for(i=0;i<5;i++)
			{
				SendPC_Testvalue.risistence[i] = 0xd0;
			}
		}else if(SendPC_Testvalue.risistence[0] == 0x56 || Tft_5520_Dispvalue.comp == 0x94){//短路
			for(i=0;i<5;i++)
			{
				SendPC_Testvalue.risistence[i] = 0xb0;
			}
		}
	memcpy(&SendPC_Testvalue.time[0],(char *)Send_Testvalue[0].time,3);
	SendPC_Testvalue.time[3] = Send_Testvalue[0].time[4];
	SendPC_Testvalue.risistence[5] = 0x4d;//电阻单位固定为MΩ
	SendPC_Testvalue.current[5] = 0x75;//电流单位固定为uA
	for(i=0;i<5;i++)
	{
		if(SendPC_Testvalue.current[i] == 0x2d)//开路
			SendPC_Testvalue.current[i] = 0xb0;
	}
	SendPC_Testvalue.comp = Send_Testvalue[0].comp;
	if(SendPC_Testvalue.comp > 0x91)
		SendPC_Testvalue.comp = 0x92;
}

//测试程序
void Test_Process(void)
{
	Button_Page_Typedef Button_Page;
	vu32 keynum=0;
	vu8 key,i;
	float Res,Vol,Current;
 //   vu8 test_end=0;
    vu8 *pt,*ppt;
	vu8 Disp_Flag=1;
    pt=(vu8 *)&Send_Testvalue[0];
    for(i=0;i<sizeof(Send_Testvalue[0]);i++)
    *(pt+i)=0;
	Button_Page.page=0;
	Button_Page.index=0;
	Button_Page.third=0xff;
    lcd_Clear(LCD_COLOR_TEST_BACK);
    Pro_PlcOn();
	Disp_Test_Item();	
	Delay_Key();
	clear_flag=0;
    Bais_LedOn();//高压灯亮
    Savetoeeprom();
	Disp_R_X();//显示名称
    ppt=(vu8 *)&Tft_5520_Dispvalue;
    for(i=0;i<sizeof(Tft_5520_Dispvalue);i++)
    {
        *(ppt+i)='-';
    }
    if(Tft_5520.Sys_Set.U_Switch)
    for(i=0;i<USBSENDLENTH;i++)
    {
        UserBuffer[i]='-';
    
    
    }
	while(GetSystemStatus()==SYS_STATUS_TEST)
	{
          //短路检测打开
        if(Tft_5520.Set_Value[Tft_5520.Group].Short)
        {
            if(Tft_5520_Dispvalue.comp==0x94)
            {
                Fail_led();
                Disp_Teststatue(6);
                SetSystemStatus(SYS_STATUS_FINISH);
                break;
            }
        
        
        }
		//if(Tft_5520_Dispvalue.comp==)
        //高端检测打开
        if(Tft_5520.Set_Value[Tft_5520.Group].Hi_Open)
        {
            if(Tft_5520_Dispvalue.comp==0x95)
            {
                Fail_led();
                Disp_Teststatue(7);
                SetSystemStatus(SYS_STATUS_FINISH);
                break;
            }
        
        
        }
        //低端检测打开
        if(Tft_5520.Set_Value[Tft_5520.Group].Lo_Open)
        {
            if(Tft_5520_Dispvalue.comp==0x96)
            {
                Fail_led();
                Disp_Teststatue(8);
                SetSystemStatus(SYS_STATUS_FINISH);
                break;
            }
        
        
        }
        
      
		if(Disp_Flag==1)
		{
			Disp_Test_value(&Button_Page);
			
			Disp_Flag=0;
		
		}//
		
		Colour.black=LCD_COLOR_TEST_MID;

		//Uart_Process();
        Tft_5520_Dispvalue.comp=Send_Testvalue[0].comp;
        if(Tft_5520.Set_Value[Tft_5520.Group].Delay_Time==0)//设定时间为0
        {
            if(Tft_5520_Dispvalue.comp==0x91)//合格
            {
                Pass_Led();
                Disp_Teststatue(3);
                if(Tft_5520.Set_Value[Tft_5520.Group].beep_Mode==1)//合格响
                {
                    Beep_on();
                
                
                }
                else
                     Beep_Off();

                    
            }
                
                
            else
            {
                if(Tft_5520_Dispvalue.comp==0x98)//下限失败
                {
                    Fail_led();
                    Disp_Teststatue(4);
                    
                }
                else if(Tft_5520_Dispvalue.comp==0x97)//上限失败
                    {
                        Fail_led();
                        Disp_Teststatue(5);
                    }
                if(Tft_5520.Set_Value[Tft_5520.Group].beep_Mode==2)//不合格响
                {
                    Beep_on();
                
                }
                 else
                     Beep_Off();
            }
                    

        }
        else//设定时间不为0
        {
            Disp_Teststatue(1);
            if(Tft_5520_Dispvalue.comp==0x91)
            {
//                test_end=1;
                SetSystemStatus(SYS_STATUS_FINISH);
                Pass_Led();
                Disp_Teststatue(3);
                if(Tft_5520.Set_Value[Tft_5520.Group].beep_Mode==1)//合格响
                {
                    Beep_on();


                }
                else
                    Beep_Off();


            }
            else
            {
                if(Tft_5520_Dispvalue.comp==0x98)//下限失败
                {
//                    test_end=1;
                    SetSystemStatus(SYS_STATUS_FINISH);
                    Fail_led();
                    Disp_Teststatue(4);
                    if(Tft_5520.Set_Value[Tft_5520.Group].beep_Mode==2)//不合格响
                    {
                        Beep_on();
                    
                    }
                     else
                         Beep_Off();

                }
                else if(Tft_5520_Dispvalue.comp==0x97)//上限失败
                    {
                        Fail_led();
//                        test_end=1;
                        Disp_Teststatue(5);
                        SetSystemStatus(SYS_STATUS_FINISH);
                        if(Tft_5520.Set_Value[Tft_5520.Group].beep_Mode==2)//不合格响
                        {
                            Beep_on();
                        
                        }
                         else
                             Beep_Off();
                    }
                
            }
                
        
        
        
        }
        //ComBuf3.send.buf[13]=Tft_5520_Dispvalue.comp;
            
		{

           Tft_5520_Dispvalue.Range= Send_Testvalue[0].range;
			Disp_Range(Tft_5520.Set_Value[Tft_5520.Group].Range);//显示档位
            //Disp_R_X();//显示名称

            for(i=0;i<7;i++)
            DispBuf[i]=0;
            for(i=0;i<6;i++)
            {
                DispBuf[i]=Send_Testvalue[0].dianzu[i];
                UserBuffer[13+i]=Send_Testvalue[0].dianzu[i];
                
            }
			Res=atof((char*)DispBuf);//,Vol,I,Current
						risitvalue = Res;
            UserBuffer[19]='M';
            UserBuffer[20]=' ';
            WriteString_Big(90+8,92 ,DispBuf);
            if(DispBuf[5]==0)
                WriteString_Big(90+8+32*5,92 ," ");
             UserBuffer[4]='V';
            UserBuffer[5]=' ';
             for(i=0;i<7;i++)
            DispBuf[i]=0;
            for(i=0;i<4;i++)
            {
                DispBuf[i]=Send_Testvalue[0].dianya[i];
                UserBuffer[i]=Send_Testvalue[0].dianya[i];//U盘数据复制
                
            }
			//Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].voltage , 0 , 4 , 0);
			//LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*1-2,SELECT_2END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;
			Colour.black=LCD_COLOR_TEST_BACK;
			WriteString_16(LIST2+88+10, FIRSTLINE+SPACE1*1, DispBuf,  0);
           Vol=atof((char*)DispBuf);
			if(Res>0.0001)
			{
				
				Current=Vol/Res;//
				if(Current>1800)
					Current=1800;
				if(Current>999)
				{
					sprintf((char*)DispBuf,"%4.0f",Current);
					strcat((char*)DispBuf," ");
					
				}
				else 
					if(Current>99)
						sprintf((char*)DispBuf,"%5.1f",Current);
				else
					if(Current>9)
						sprintf((char*)DispBuf,"%5.2f",Current);
					else
						sprintf((char*)DispBuf,"%5.3f",Current);
			}
			else
			{
				for(i=0;i<5;i++)
					DispBuf[i]='-';
			
			}
			memcpy(&SendPC_Testvalue.current[0],&DispBuf,5);
			DataConvertSendToPC();
      WriteString_Big(90+40,92+53 ,DispBuf);
            
             for(i=0;i<7;i++)
            DispBuf[i]=0;
            for(i=0;i<5;i++)
            {
                DispBuf[i]=Send_Testvalue[0].time[i];
                UserBuffer[6+i]=Send_Testvalue[0].time[i];
                
            }
            UserBuffer[6+5]='s';
            UserBuffer[12]=' ';
            WriteString_Big(90+40,92+53+53 ,DispBuf);
            sprintf((char*)DispBuf,"%5f",Current);
	
		}
		
		Colour.Fword=LCD_COLOR_WHITE;
    
	  key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	
			//Disp_Flag=1;
			Key_Beep();
			switch(key)
			{
				
				case Key_Disp:
                   // SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    //SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:

				break;
				case Key_RIGHT:

						
				break;
				case Key_DOWN:

				break;
				case Key_UP:
				break;
				
				case Key_STOP:
                    //Send_UartStop();
                    SetSystemStatus(SYS_STATUS_ABORT);
                 Disp_Teststatue(2);
				case Key_BACK:
					
				break;
				case Key_LOCK:

				break;

				case Key_REST:
					Power_Off_led();
				break;
				case Key_TRIG:
//					trip_flag=1;
				break;
				default:
				break;
					
			}
		
		
		}
        if(Tft_5520.Sys_Set.Uart&&(Uart3_returnflag==1))//串口打开，往上位机发送数据
        {
            Uart3_returnflag=0;
            Uart3_Send();
        }
	}
    Bais_LedOff();
    if(Tft_5520_Dispvalue.comp==0x91)
    {
        UserBuffer[21]='P';
        UserBuffer[22]='A';
        UserBuffer[23]='S';
        UserBuffer[24]='S';
        
    }
    else
    {
        UserBuffer[21]='F';
        UserBuffer[22]='A';
        UserBuffer[23]='I';
        UserBuffer[24]='L';
    
    
    }
    UserBuffer[25]='\r';
    UserBuffer[26]='\n';
      
    if(Tft_5520.Sys_Set.U_Switch)//U盘打开
    Write_Usbdata (UserBuffer,27);
    Disp_Usbflag(usb_oenflag);
    if(GetSystemStatus()==SYS_STATUS_ABORT)
        Send_UartStop();
    Pro_Plcoff();
}
//文件管理程序
void File_Process(void)
{
	 vu32 keynum=0;
	 vu8 key;
	Delay_Key();
  	while(GetSystemStatus()==SYS_STATUS_FILE)
	{
	 key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
			switch(key)
			{
				case Key_F1:
				break;
				case Key_F2:
				break;
				case Key_F3:
				break;
				case Key_F4:
				break;
				case Key_F5:
				break;
				case Key_Disp:
				break;
				case Key_SETUP:
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	
	}

}

//参数设置程序
void Setup_Process(void)
{

	Button_Page_Typedef Button_Page;
//	Send_Ord_Typedef Uart;
    vu16 *pt;
	
	vu32 keynum=0;
	vu8 key;
	vu8 Disp_Flag=1;
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Button_Page.index=0;
	Button_Page.page=0;
    Disp_Test_Set_Item();
	Delay_Key();
    
 	while(GetSystemStatus()==SYS_STATUS_SETUPTEST)
	{
	    
		if(Disp_Flag==1)
		{
			DispSet_value(&Button_Page);
			Disp_Flag=0;
			Delay_Key();
            if(Button_Page.index>1)
            {
                pt=(vu16* )&Tft_5520.Set_Value[Tft_5520.Group] + Button_Page.index-2;
                NumBox.Num=*pt;
                NumBox.Max=ParameterLimit[Button_Page.index][1];//Max
                NumBox.Min=ParameterLimit[Button_Page.index][0];//Min
                NumBox.Dot=0;//Dot
            }
		
		}

		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==5)
		{	
			Disp_Flag=1;
			Key_Beep();
			switch(key)
			{
				
				
				case Key_STOP:
				SetSystemStatus(SYS_STATUS_IDEM);
                break;
				
				case Key_Disp:
                        SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                        SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_RIGHT:
                    switch(Button_Page.index)
                    {
                        case 0:
                            SetSystemStatus(SYS_STATUS_IDEM);
                            
                            break;
                        case 1:
                            if(Tft_5520.Group <1)
                                Tft_5520.Group =GROUP_MAX-1;
                            else
                                Tft_5520.Group --;
                            break;
                        case 2:
                            //if(Tft_5520.Set_Value[Tft_5520.Group].Speed)
                                Tft_5520.Set_Value[Tft_5520.Group].Speed=Tft_5520.Set_Value[Tft_5520.Group].Speed>0? 0:1;
                            
                            break;
                        case 3:
                            LcdAddr.x=LIST1+88+8;
                            LcdAddr.y=FIRSTLINE+SPACE1*3;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                        case 4:
                            LcdAddr.x=LIST1+88+10;
                            LcdAddr.y=FIRSTLINE+SPACE1*4;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=1;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                        
                            break;
                        case 5:
                            Tft_5520.Set_Value[Tft_5520.Group].Hi_Open=Tft_5520.Set_Value[Tft_5520.Group].Hi_Open>0? 0:1;
                            break;
                        case 6:
                            Tft_5520.Set_Value[Tft_5520.Group].Lo_Open=Tft_5520.Set_Value[Tft_5520.Group].Lo_Open>0? 0:1;
                            break;
                        case 7:
                            if(Tft_5520.Set_Value[Tft_5520.Group].Range<1)
                                Tft_5520.Set_Value[Tft_5520.Group].Range=RANGE_MAX;
                            else
                                Tft_5520.Set_Value[Tft_5520.Group].Range--;
                            break;
                        case 8:
                            if(Tft_5520.Set_Value[Tft_5520.Group].beep_Mode<1)
                                Tft_5520.Set_Value[Tft_5520.Group].beep_Mode=2;
                            else
                                Tft_5520.Set_Value[Tft_5520.Group].beep_Mode--;
                            break;
                        case 9:
                            LcdAddr.x=LIST2+88;
                            LcdAddr.y=FIRSTLINE+SPACE1*3;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                        case 10:
                            LcdAddr.x=LIST2+88;
                            LcdAddr.y=FIRSTLINE+SPACE1*4;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                        case 11:
                             Tft_5520.Set_Value[Tft_5520.Group].Short=Tft_5520.Set_Value[Tft_5520.Group].Short>0? 0:1;
                            break;
                        default:
                            break;
                                                              
                    }						
				break;
				case Key_LEFT:
                    switch(Button_Page.index)
                    {
                        case 0:
                            SetSystemStatus(SYS_STATUS_SYSSET);
                            
                            break;
                        case 1:
                            if(Tft_5520.Group>=GROUP_MAX-1)
                                Tft_5520.Group =0;
                            else
                                Tft_5520.Group ++;
                            break;
                        case 2:
                            //if(Tft_5520.Set_Value[Tft_5520.Group].Speed)
                                Tft_5520.Set_Value[Tft_5520.Group].Speed=Tft_5520.Set_Value[Tft_5520.Group].Speed>0? 0:1;
                            
                            break;
                        case 3:
                            LcdAddr.x=LIST1+88+8;
                            LcdAddr.y=FIRSTLINE+SPACE1*3;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                        case 4:
                            LcdAddr.x=LIST1+88+10;
                            LcdAddr.y=FIRSTLINE+SPACE1*4;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=1;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                        case 5:
                            Tft_5520.Set_Value[Tft_5520.Group].Hi_Open=Tft_5520.Set_Value[Tft_5520.Group].Hi_Open>0? 0:1;
                            break;
                        case 6:
                            Tft_5520.Set_Value[Tft_5520.Group].Lo_Open=Tft_5520.Set_Value[Tft_5520.Group].Lo_Open>0? 0:1;
                            break;
                        case 7:
                            if(Tft_5520.Set_Value[Tft_5520.Group].Range>=RANGE_MAX)
                                Tft_5520.Set_Value[Tft_5520.Group].Range=0;
                            else
                                Tft_5520.Set_Value[Tft_5520.Group].Range++;
                            break;
                        case 8:
                            if(Tft_5520.Set_Value[Tft_5520.Group].beep_Mode>1)
                                Tft_5520.Set_Value[Tft_5520.Group].beep_Mode=0;
                            else
                                Tft_5520.Set_Value[Tft_5520.Group].beep_Mode++;
                            break;
                        case 9:
                            LcdAddr.x=LIST2+88;
                            LcdAddr.y=FIRSTLINE+SPACE1*3;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                        case 10:
                            LcdAddr.x=LIST2+88;
                            LcdAddr.y=FIRSTLINE+SPACE1*4;
                            Colour.black=LCD_COLOR_SELECT;
                            
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                        case 11:
                             Tft_5520.Set_Value[Tft_5520.Group].Short=Tft_5520.Set_Value[Tft_5520.Group].Short>0? 0:1;
                            break;
                        default:
                            break;
                                                              
                    }	
				break;
				case Key_DOWN:
					if(Button_Page.index>SET_MAX-1)
						Button_Page.index=0;
					else
						Button_Page.index++;
					
					
				break;
				case Key_UP:
					if(Button_Page.index<1)
						Button_Page.index=SET_MAX;
					else
						Button_Page.index--;
					
				break;
				case Key_DOT:
//					
					break;

				case Key_PAGE://翻页
                    SetSystemStatus(SYS_STATUS_SYSSET);
				break;
				case Key_LOCK:
				break;
//				case Key_BIAS:
//				break;
				case Key_EXIT:
                    SetSystemStatus(SYS_STATUS_IDEM);
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	
	
	}
	Savetoeeprom();
    Send_Set_Value();
}
//数据保存程序
void Data_StoreProcess(void)
{
	vu32 keynum=0;
	vu8 key;
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_DATASTORE)
	{
	 key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
			switch(key)
			{
				case Key_F1:
				break;
				case Key_F2:
				break;
				case Key_F3:
				break;
				case Key_F4:
				break;
				case Key_F5:
				break;
				case Key_Disp:
				break;
				case Key_SETUP:
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	}
}
//档号显示
void Range_Process(void)
{
	
	vu32 keynum=0;
	vu8 key;
	Send_Ord_Typedef Uart;
//    vu8 page=1;
	vu8 Disp_flag=1;
//	vu8 index=0;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_Range_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_RANGE)
	{
		if(Disp_flag==1)
		{
			Disp_RangeDispValue(&Button_Page);
			Disp_flag=0;	
		}
		Uart_Process();
		
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
//					Send_Request();
					break;
				case 1:
					Send_Main_Ord();
					break;
				case 2:
					Send_Freq(&Uart);
					break;
				default:
//					Send_Request();
					break;
			
			}
			Uart_Send_Flag=0;
			
			timer0_counter=0;
		
		
		}
		if(SaveData.Limit_Tab.Comp)
			Test_Comp(&Comp_Change);
		Disp_RangeTestvalue();
	 	key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			switch(key)
			{
				case Key_F1:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_TEST);
					}
					else
						SaveData.Limit_Tab.Comp=0;
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGE);
					}
					else
						SaveData.Limit_Tab.Comp=1;
				break;
				case Key_F3:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_RANGECOUNT);
					}
				break;
				case Key_F4:
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==0)
//							SetSystemStatus(SYS_STATUS_ITEM);
//					}
				break;
				case Key_F5:
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==1)
//							Button_Page.page=0;
//						else
//							Button_Page.page=1;
//						Disp_Button_value1(Button_Page.page);
//					}
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
					
//				break;
				case Key_LEFT:
//				break;
				case Key_RIGHT:
//				break;
				case Key_UP:
//				break;
				case Key_DOWN:
					if(Button_Page.index)
						Button_Page.index=0;
					else
						Button_Page.index=1;
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
//档计数显示
void Range_CountProcess(void)
{
   	 vu32 keynum=0;
	vu32 uart_count=0;
	 vu8 key;
	u8 i;
	vu8 Disp_flag=1;
//	 vu8 page=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_Range_Count_Item();
	Delay_Key();
	Set_Compbcd_float();
	for(i=0;i<=10;i++)
		Count_buff[i]=0;
 	while(GetSystemStatus()==SYS_STATUS_RANGECOUNT)
	{
		key=HW_KeyScsn();
		if(Disp_flag==1)
		{
			Disp_Range_ComDispValue(&Button_Page);
			Disp_flag=0;
			
		}
		uart_count=Uart_Process();
		if(timer0_counter>0)//请求数据
		{
			switch(Uart_Send_Flag)
			{
				case 0:
//					Send_Request();
					break;
				case 1:
					Send_Main_Ord();
					break;
				case 2:
					//Send_Freq(&Uart);
					break;
				default:
//					Send_Request();
					break;
			
			}
			Uart_Send_Flag=0;
			
			timer0_counter=0;
		
		
		}
		if(SaveData.Limit_Tab.Comp&&uart_count==1)
			Test_Comp(&Comp_Change);
		//Disp_RangeTestvalue();
		if(SaveData.Limit_Tab.Param)//附属比较
			;
		Disp_RangeCount();//增加分选结果计数
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{	Disp_flag=1;
			switch(key)
			{
				case Key_F1:
                    if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_TEST);
					}
					else
						SaveData.Limit_Tab.count=0;
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
                    if(Button_Page.page==0)
                        SetSystemStatus(SYS_STATUS_RANGE);
					}
					else
						SaveData.Limit_Tab.count=1;
				break;
				case Key_F3:
					if(Button_Page.index==0)
                    if(Button_Page.page==0)
                        SetSystemStatus(SYS_STATUS_RANGECOUNT);
				break;
				case Key_F4:
//					if(Button_Page.index==0)
//                    if(Button_Page.page==0)
//                        SetSystemStatus(SYS_STATUS_ITEM);
				break;
				case Key_F5:
//                    if(Button_Page.page==1)
//						Button_Page.page=0;
//					else
//						Button_Page.page=1;
//                    Disp_Button_value1(Button_Page.page);
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				//break;
				case Key_RIGHT:
				//break;
				case Key_UP:
				//break;
				case Key_DOWN:
					if(Button_Page.index==0)
						Button_Page.index=1;
					else
						Button_Page.index=0;
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
// 列表显示
void ItemProcess(void)
{
	
	vu32 keynum;
	vu8 key;
	vu8 Disp_flag=1;
//    vu8 page=1;
	
	Button_Page_Typedef Button_Page;
	SaveData.Limit_ScanValue.num=0;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_List_Count_Item();
	Delay_Key();
    All_LedOff1();
 	while(GetSystemStatus()==SYS_STATUS_ITEM)
	{
		if(Disp_flag)
		{
			Disp_flag=0;
			Disp_LIMIT_ComDispValue(&Button_Page);
		}
		
		Disp_Scan_Compvalue(0);
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
			Disp_flag=1;
			switch(key)
			{
				case Key_START:
                    Send_UartStart();
                break;
                //Senf_UartStop();
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
					if(Button_Page.index>0)
						Button_Page.index--;
                    
				break;
				case Key_DOWN:
					if(Button_Page.index<2)
						Button_Page.index++;
				break;
			
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
//				case Key_BIAS:
//				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
//列表扫描设置
void Use_ITEMSetProcess(void)
{
	Disp_Coordinates_Typedef  Coordinates;
	vu32 keynum=0;
	vu8 key;
//    vu8 page=0;
	vu8 Disp_flag=1;
	Button_Page_Typedef Button_Page;
	
	Button_Page.index=0;
	Button_Page.page=0;
	Button_Page.third=0;
	Button_Page.force=0;
	SaveData.Limit_ScanValue.num=0;
	Disp_ListScan_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_ITEMSET)
	{
	  	
		if(Disp_flag)
		{
			Disp_flag=0;
		
			Disp_Scan_SetCompvalue(&Button_Page);
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			switch(key)
			{
				case Key_F1:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_SETUPTEST);
						else
							SetSystemStatus(SYS_STATUS_FILE);
					}
						
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_USERDEBUG);
						else
							SetSystemStatus(SYS_STATUS_SYSSET);
					}
				break;
				case Key_F3://
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_LIMITSET);
						else
							SetSystemStatus(SYS_STATUS_TOOL);
					}
				break;
				case Key_F4:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_ITEMSET);
					}else 
					if(Button_Page.index>1)
					{
						if(Button_Page.force>0)
							Button_Page.force--;
					
					}
					
				break;
				case Key_F5:
					if(Button_Page.index==0)
					{
						if(Button_Page.page)
							Button_Page.page=0;
						else
							Button_Page.page=1;
						Disp_Button_TestSet(Button_Page.page);
					}else
					if(Button_Page.force<20)
						Button_Page.force++;
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
					if(Button_Page.third>0)
						Button_Page.third--;
						
					
				break;
				case Key_RIGHT:
					if(Button_Page.third<5)
						Button_Page.third++;
					else
						Button_Page.third=5;
				break;
				case Key_UP:
					if(Button_Page.index>0)
						Button_Page.index--;
					else
						Button_Page.page=0;
						
				break;
				case Key_DOWN:
					if(Button_Page.index<12)
						Button_Page.index++;
					else
						Button_Page.index=12;
				break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				//break;
				case Key_DOT:
				//break;
				
					if(Button_Page.index>2)
					{
						if(Button_Page.third==0)
						{
							Coordinates.xpos=40;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=FIRSTLINE+SPACE1+3+(Button_Page.index-2)*16;
							
							SaveData.Limit_ScanValue.freq[Button_Page.force*10+
							(Button_Page.index-3)]=Freq_Set_Num(&Coordinates);
						}else 
						if(Button_Page.third==1)
						{
						
						}else
						if(Button_Page.third==2)
						{
						
						}else
						if(Button_Page.third==3)
						{
						
						}else
						if(Button_Page.third==4)
						{
							Coordinates.xpos=420;
							Coordinates.ypos=FIRSTLINE+SPACE1+3+(Button_Page.index-2)*16;
							Coordinates.lenth=60;
							SaveData.Limit_ScanValue.time[Button_Page.force*10+
							(Button_Page.index-3)]=Disp_Set_Num(&Coordinates);
						
						
						}
					
					}
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
//				case Key_REST:
//				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
//极限设置 
void Use_LimitSetProcess(void)
{
	
	vu32 keynum=0;
	vu8 key,i;
	Disp_Coordinates_Typedef Coordinates;
//    vu8 page=1;
	vu8 Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_LimitList_Item();
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_LIMITSET)
	{
	  	if(Disp_flag)
		{
			Disp_flag=0;
			Disp_LimitSEt_value(& Button_Page);
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			switch(key)
			{
				case Key_F1:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_SETUPTEST);
						else
							SetSystemStatus(SYS_STATUS_FILE);
					
					}else if(Button_Page.index==1)
					;
					else if(Button_Page.index==2)
						;
					else if(Button_Page.index==3)
					{
						SaveData.Limit_Tab.Mode=0;
					
					}
					else if(Button_Page.index==4)
					{
						SaveData.Limit_Tab.Param=0;
					}
					else if(Button_Page.index==5)
					{
						SaveData.Limit_Tab.Comp=0;
					}else
					{
						if(Button_Page.index>15)
						{
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].high.Num=0;
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].low.Num=0;	
						}else if(Button_Page.index>5)
						{
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].high.Num=0;
						SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low.Num=0;	
						
						}
					
					
					}
						
				break;
				case Key_F2:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_USERDEBUG);
						else
							SetSystemStatus(SYS_STATUS_SYSSET);
					}
					else if(Button_Page.index==1)
					;
					else if(Button_Page.index==2)
						;
					else if(Button_Page.index==3)
					{
						SaveData.Limit_Tab.Mode=1;
					
					}
					else if(Button_Page.index==4)
					{
						SaveData.Limit_Tab.Param=1;
					}
					else if(Button_Page.index==5)
					{
						SaveData.Limit_Tab.Comp=1;
					}
				break;
				case Key_F3:
					if(Button_Page.index==0)
					{
						if(Button_Page.page==0)
							SetSystemStatus(SYS_STATUS_LIMITSET);
						else
							SetSystemStatus(SYS_STATUS_TOOL);
					}
				break;
				case Key_F4:
//					if(Button_Page.index==0)
//					{
//						if(Button_Page.page==0)
//							SetSystemStatus(SYS_STATUS_ITEMSET);
//					}
					
				break;
				case Key_F5:
					if(Button_Page.index==0)
					{
//						if(Button_Page.page)
//							Button_Page.page=0;
//						else
//						Button_Page.page=1;
//						Disp_Button_TestSet(Button_Page.page);
					}else if(Button_Page.index>5)
					{
						for(i=0;i<10;i++)
						{
							SaveData.Limit_Tab.Comp_Value[i].low.Num=0;
							SaveData.Limit_Tab.Comp_Value[i].high.Num=0;
							
						}
						Button_Page.index=0;
					
					
					}
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
					if(Button_Page.index!=0)
					{
						if(Button_Page.index<=5)
						{
							Button_Page.index--;
						
						}
						else
						{
							if(Button_Page.index>15)
								Button_Page.index-=10;
							else
								Button_Page.index+=9;
						}
					
					
					}
					
				break;
				case Key_RIGHT:
					if(Button_Page.index!=0)
					{
//						if(Button_Page.index<=5)
//						{
//							Button_Page.index++;
//						
//						}
//						else
						//if(Button_Page.index)
						{
							if(Button_Page.index>15)
								Button_Page.index-=9;
							else
								//if(Button_Page.index<)
								Button_Page.index+=10;
						}
					
					
					}
				break;
				case Key_UP:
					if(Button_Page.index>16)
						Button_Page.index--;
					else if(Button_Page.index==16)
						Button_Page.index=5;
					else
						
					if(Button_Page.index>0)
						Button_Page.index--;
				break;
				case Key_DOWN:
					if(Button_Page.index<15)
						Button_Page.index++;
					else if(Button_Page.index==15)
						Button_Page.index=0;
					else
						
					if(Button_Page.index<25)
						Button_Page.index++;
					else
						Button_Page.index=0;
						
				break;
				case Key_NUM1:
//				break;
				case Key_NUM2:
//				break;
				case Key_NUM3:
//				break;
				case Key_NUM4:
//				break;
				case Key_NUM5:
//				break;
				case Key_NUM6:
//				break;
				case Key_NUM7:
//				break;
				case Key_NUM8:
//				break;
				case Key_NUM9:
//				break;
				case Key_NUM0:
//				break;
				case Key_DOT:
					if(Button_Page.index==2)//LIST2-24, FIRSTLINE,
					{
							Coordinates.xpos=LIST2-24;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=FIRSTLINE;
							Coordinates.lenth=66;
							SaveData.Limit_Tab.Nom=Disp_Set_InputNum(&Coordinates);
					}else if(Button_Page.index>5&&Button_Page.index<=15)
						{
							//LIST2-90, 76+(i-6)*15
							Coordinates.xpos=LIST2-90;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=76+(Button_Page.index-6)*15;
							Coordinates.lenth=60;
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low=Disp_Set_InputpreNum(&Coordinates);
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].high=SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low;
							//Sort_TypeDef Disp_Set_InputpreNum(Disp_Coordinates_Typedef *Coordinates)
						}else if(Button_Page.index>15)
						{
							Coordinates.xpos=LIST2+70;//FIRSTLINE+SPACE1+3+ (i-2)*16
							Coordinates.ypos=76+(Button_Page.index-16)*15;
							Coordinates.lenth=60;
							SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].high=Disp_Set_InputpreNum(&Coordinates);
						
						}
						do{
							key=HW_KeyScsn();
						}
						while(key!=0xff);
						
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
//				case Key_REST:
//				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
	Set_Compbcd_float();
	Savetoeeprom();
}
void Sys_Process(void)
{
	vu32 keynum=0;
	vu8 key,rc;
	vu8 Disp_flag=1;
//	Button_Page_Typedef Button_Page;
//	Button_Page.index=0;
//	Button_Page.page=0;
//	Disp_Sys_Screen();
//	Colour.Fword=White;
//	Colour.black=LCD_COLOR_TEST_BAR;
//	WriteString_16(0, 4, All_TopName[8],  0);
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_Sys_Item(3);
    
	Delay_Key();
	while(GetSystemStatus()==SYS_STATUS_SYS)
	{
		if(Disp_flag==1)
		{
			Disp_Sys();
			Disp_flag=0;	
			
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
            Key_Beep();
			switch(key)
			{
                case Key_RIGHT:
                    SetSystemStatus(SYS_STATUS_SYSSET);
                break;
				case  Key_LEFT:
                    //SetSystemStatus(SYS_STATUS_SYSSET);
				//break;
				case Key_PAGE://翻页
                    SetSystemStatus(SYS_STATUS_SYS);
				
				
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_IDEM);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
                case Key_STOP:
				case Key_EXIT:
                    SetSystemStatus(SYS_STATUS_IDEM);
                break;
				default:
				break;
					
			}
		
		
		}

	
	}
    if(Tft_5520.Sys_Set.U_Switch)
    {
        Host_Init();               /* Initialize the lpc17xx host controller                                    */
        rc = Host_EnumDev();       /* Enumerate the device connected                                            */
        if (rc == OK) 
        {
            /* Initialize the mass storage and scsi interfaces */
                rc = MS_Init( &blkSize, &numBlks, inquiryResult );
            if (rc == OK) 
            {
                rc = FAT_Init();   /* Initialize the FAT16 file system */    
                Write_Usbdata ( "电压     时间      电阻      分选\r\n" ,25);			
            } 
        } 
    }

}
//软件关机程序，放在工具状态里面
void Soft_Turnon(void)
{
	uint32_t i;
	Set_Compbcd_float();
	ReadSavedata();
	SetData_High_Low_Comp();
	Savetoeeprom();
	
	Power_On_led();
//	Lock_Control_Off();
//	Lock_LedOff()
	All_LedOff();
	Beep_Off();
	while(GetSystemStatus()==SYS_STATUS_TOOL)
	{
		for(i=0xfffff;i>0;i--)
		;
		NVIC_EnableIRQ(GPIO_IRQn);
		
//		if(Save_Res.softswitch)
//		{
//			SetSystemStatus(SYS_STATUS_POWER);
//		
//		
//		}
	
	
	}
	Power_Off_led();

}
//系统设置
void Use_SysSetProcess(void)
{
	vu16 *pt;
	vu32 keynum=0;
	vu8 key;
	vu8 Disp_flag=1;
	Button_Page_Typedef Button_Page;
	Button_Page.index=0;
	Button_Page.page=0;
    lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_Sys_Item(2);
	Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_SYSSET)
	{
	  	
		if(Disp_flag==1)
		{
			Disp_Sys_value(&Button_Page);
			Disp_flag=0;	
		}
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			Key_Beep();
			switch(key)
			{
				
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
                    switch(Button_Page.index)
                    {
                        case 0:
                            SetSystemStatus(SYS_STATUS_SYS);
                            break;
                        case 1:
                            Tft_5520.Sys_Set.Uart=Tft_5520.Sys_Set.Uart>0? 0:1;
                            break;
                        case 2:
                            if(Tft_5520.Sys_Set.Buad>4)
                                Tft_5520.Sys_Set.Buad=0;
                            else
                                Tft_5520.Sys_Set.Buad++;
                            break;
                        case 3:
                            Tft_5520.Sys_Set.U_Switch=Tft_5520.Sys_Set.U_Switch>0? 0:1;
                        break ;
                        case 4:
                            Tft_5520.Sys_Set.I_O=Tft_5520.Sys_Set.I_O>0? 0:1;
                            break;
                        case 5:
                            Tft_5520.Sys_Set.Lang=Tft_5520.Sys_Set.Lang>0? 0:1;
														lcd_Clear(LCD_COLOR_TEST_BACK);
														Disp_Sys_Item(2);
                            break;
												case 6:
                            LcdAddr.x=LIST1+100;
                            LcdAddr.y=FIRSTLINE+SPACE1*5+2;
                            Colour.black=LCD_COLOR_SELECT;
                            
														pt=(vu16* )&Tft_5520.Sys_Set.Addr;
														NumBox.Num=*pt;
														NumBox.Max=255;//Max
														NumBox.Min=1;//Min
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                    
                    
                    }
                    
                    
				break;
				case Key_RIGHT:
                    switch(Button_Page.index)
                    {
                        case 0:
                            SetSystemStatus(SYS_STATUS_SETUPTEST);
                            break;
                        case 1:
                            Tft_5520.Sys_Set.Uart=Tft_5520.Sys_Set.Uart>0? 0:1;
                            break;
                        case 2:
                            if(Tft_5520.Sys_Set.Buad==0)
                                Tft_5520.Sys_Set.Buad=4;
                            else
                                Tft_5520.Sys_Set.Buad--;
                            break;
                        case 3:
                            Tft_5520.Sys_Set.U_Switch=Tft_5520.Sys_Set.U_Switch>0? 0:1;
                        break ;
                        case 4:
                            Tft_5520.Sys_Set.I_O=Tft_5520.Sys_Set.I_O>0? 0:1;
                            break;
                        case 5:
                            Tft_5520.Sys_Set.Lang=Tft_5520.Sys_Set.Lang>0? 0:1;
														lcd_Clear(LCD_COLOR_TEST_BACK);
														Disp_Sys_Item(2);
                            break;
												case 6:
                            LcdAddr.x=LIST1+100;
                            LcdAddr.y=FIRSTLINE+SPACE1*5+2;
                            Colour.black=LCD_COLOR_SELECT;
                            
														pt=(vu16* )&Tft_5520.Sys_Set.Addr;
														NumBox.Num=*pt;
														NumBox.Max=255;//Max
														NumBox.Min=1;//Min
                            NumBox.Dot=0;
                            Number_Setup(&NumBox);//数值框设置
                
                            *pt=NumBox.Num;//更新设置值
                            break;
                    
                    
                    }
				break;
				case Key_DOWN:
					if(Button_Page.index>SYS_MAX-1)
						Button_Page.index=0;
					else
						Button_Page.index++;
						
				break;
				case Key_UP:
					if(Button_Page.index<1)
						Button_Page.index=SYS_MAX;
					else
						Button_Page.index--;
				break;
				
				case Key_PAGE://翻页
                    SetSystemStatus(SYS_STATUS_SYS);
				case Key_LOCK:
				break;
//				case Key_BIAS:
//				break;
				case Key_EXIT:
                case Key_STOP:
                    SetSystemStatus(SYS_STATUS_IDEM);
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
	Savetoeeprom();
}

//用户校正
void Use_DebugProcess(void)
{
	
	vu32 keynum=0;
	vu8 key,start_flag=0,return_flag=0;
    vu16 *pt;
//	float ddd,eee;
//	Disp_Coordinates_Typedef Coordinates;
//    vu8 page=1;
	vu8 Disp_flag=1,i;
	Button_Page_Typedef Button_Page;
//    if(!Password_Check())//密码校验,TRUE有效
//    {
//        SetSystemStatus(SYS_STATUS_IDEM);//待机状态
//        return;
//    }
    Send_Request(0xf6);//进入校正
    lcd_Clear(LCD_COLOR_TEST_BACK);
    All_LedOff1();
    debug_saveflag=0;
	Button_Page.index=0;
	Button_Page.page=0;
	Disp_UserCheck_Item();
	Delay_Key();
    for(i=0;i<5;i++)
    {
        if(Tft_5520.Debugvalue[i]>1200||Tft_5520.Debugvalue[i]<800)
            Tft_5520.Debugvalue[i]=1000;
        
    }
 	while(GetSystemStatus()==SYS_STATUS_USERDEBUG)
	{
		//Uart_Process();
		if(Disp_flag==1)
		{

			Disp_flag=0;	
            
            if(Button_Page.index)
            {
                pt=(vu16* )&Tft_5520.Debugvalue[Button_Page.index-1];
                NumBox.Num=*pt;
                NumBox.Max=1200;//ParameterLimit[Button_Page.index][1];//Max
                NumBox.Min=800;//ParameterLimit[Button_Page.index][0];//Min
                NumBox.Dot=0;//Dot
            }
            Disp_Debug_value(&Button_Page);
            
		}

        if(Button_Page.index&&start_flag==1)
		
            Disp_Debug_Reference(&Button_Page);
        if(return_flag==1)
        {
            return_flag=2;
            debug_saveflag=0;
           // Delay(100);
            if(Button_Page.index)
                Send_Debug_Value(Tft_5520.Debugvalue[Button_Page.index-1]);
            Savetoeeprom();
            
            
//            if(Button_Page.index)
//            Send_Request(0xf4);
        }

		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			Disp_flag=1;
			Key_Beep();
			switch(key)
			{
//                case Key_STOP:
//                    Send_Request(0xf3);//退出校正
//                    SetSystemStatus(SYS_STATUS_POWER);
                    
                //break;
				case Key_START:
                    
                    
                    if(start_flag==0&&Button_Page.index!=0)
                    {
 
                        Send_Sendstaetdebug(Button_Page.index-1);
                        Delay(100);
                        Send_Sendstaetdebug(Button_Page.index-1);
                        start_flag=1;
                        Pass_Led();
                    }
                break;
                case Key_STOP:
                case Key_EXIT:
                    Send_Request(0xf5);
                    Delay(100);
                    Send_Request(0xf5);
                    
               
                    start_flag=0;
                    All_LedOff1();
                    
                break;
                case Key_PAGE:
                    
                break;
				case Key_FAST://OK
                    if(return_flag==0)
                    {
                        Send_Request(0xf5);

                    }
                    else
                    {                        
//                        Send_Request(0xf4);
                        return_flag=0;
                    }
                    start_flag=0;
                    All_LedOff1();
                    
                    break;
				case Key_LEFT:
                case Key_RIGHT:
                    if(start_flag)
                    {
                         LcdAddr.x=LIST1+160;
                        LcdAddr.y=FIRSTLINE+SPACE1*Button_Page.index;
                        Colour.black=LCD_COLOR_SELECT;
                        
                    
                     //FIRSTLINE+SPACE1*(i+1)
                        switch(Button_Page.index)
                        {
                            case 1:
                            case 5:
                            NumBox.Dot=0;
                            break;
                            case 2:
                                NumBox.Dot=3;
                            break;
                            case 3:
                                NumBox.Dot=2;
                            break;
                            case 4:
                                NumBox.Dot=1;
                            break;
                            
                            default:
                                
                                NumBox.Dot=0;
                            break;
                        
                        }
                        
                        return_flag=Number_Setup(&NumBox);//数值框设置
            
                        *pt=NumBox.Num;//更新设置值
                        start_flag=0;
                        All_LedOff1();
                        //return_flag=1;
                   
                    }
				
				break;
				case Key_UP:
                    if(start_flag==0)
                    {
						if(Button_Page.index>0)
							Button_Page.index--;
						else
							Button_Page.index=DEBUG_RANGE;
                    }
							
				break;
				case Key_DOWN:
                    if(start_flag==0)
                    {
    //					Save_Res.Debug_Value[Button_Page.index].standard=
    //					Save_Res.Debug_Value[Button_Page.index].ad_value=
                        if(Button_Page.index<DEBUG_RANGE)
                            Button_Page.index++;
                    }
					
				break;
                    
					
				
				default:
				break;
					
			}
		
		
		}
	
	
	}
}

//公司校正
void Fac_DebugProcess(void)
{
	vu32 keynum=0;
	vu8 key;
 	while(GetSystemStatus()==SYS_STATUS_FACRDEBUG)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==2)
		{
			switch(key)
			{
				case Key_F1:
				break;
				case Key_F2:
				break;
				case Key_F3:
				break;
				case Key_F4:
				break;
				case Key_F5:
				break;
				case Key_Disp:
				break;
				case Key_SETUP:
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
				break;
				case Key_NUM2:
				break;
				case Key_NUM3:
				break;
				case Key_NUM4:
				break;
				case Key_NUM5:
				break;
				case Key_NUM6:
				break;
				case Key_NUM7:
				break;
				case Key_NUM8:
				break;
				case Key_NUM9:
				break;
				case Key_NUM0:
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}	
//==========================================================
//函数名称：Uart_Process
//函数功能：串口处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26 
//修改日期：2015.10.26 10:02
//备注说明：无
//==========================================================
//u8 Uart_Process(void)
//{
//	vu8 i;
//#if HW_UART_SUPPORT
//	u8 kind=0xff;
//	u8 data=0;
//	u8 str[(FRAME_LEN_MAX-FRAME_LEN_MIN)+1];//收发数据缓冲
////	if(SaveData.Sys_Setup.Bus_Mode==0)//串口有效
//	{
//		if (ComBuf.rec.end)//接收数据结束
//		{data=1;
//			//memset(str,'\0',(FRAME_LEN_MAX-FRAME_LEN_MIN+1));//清空缓冲
//			{
//				//memcpy(str,&ComBuf.rec.buf[PDATASTART-1],ComBuf.send.len-FRAME_LEN_MIN);//数据包
//				kind=ComBuf.rec.buf[PFRAMEKIND];//命令字
//                switch(kind)
//                {
//                    case 0xfa:
////                        if(GetSystemStatus()!=SYS_STATUS_TEST)
////                            SetSystemStatus(SYS_STATUS_TEST);
//                        break;
//                    case 0xfb:
//                       // Tft_5520_Dispvalue.comp=0x93;
//                        break;
//                    case 0:
//                        Tft_5520_Dispvalue.Range=ComBuf.rec.buf[2];
//                        Tft_5520_Dispvalue.Test_R_Dot=ComBuf.rec.buf[3];
//                        Tft_5520_Dispvalue.comp=ComBuf.rec.buf[4];
//                        Tft_5520_Dispvalue.Test_R=ComBuf.rec.buf[6];
//                        Tft_5520_Dispvalue.Test_R<<=8;
//                        Tft_5520_Dispvalue.Test_R+=ComBuf.rec.buf[5];
//                        
//                        Tft_5520_Dispvalue.Test_V=ComBuf.rec.buf[8];
//                        Tft_5520_Dispvalue.Test_V<<=8;
//                        Tft_5520_Dispvalue.Test_V+=ComBuf.rec.buf[7];
//                        
//                        Tft_5520_Dispvalue.Test_T=ComBuf.rec.buf[10];
//                        Tft_5520_Dispvalue.Test_T<<=8;
//                        Tft_5520_Dispvalue.Test_T+=ComBuf.rec.buf[9];    
//                    break;
//                    case 1://
//                        break;
//                    case 2://
//                        break;
//                    case 3://
//                        break;
//                        
//            
//                }

//			}
//            
//			//准备接收下一帧数据sprintf
//			ComBuf.rec.end=0;//接收缓冲可读标志复位
//			ComBuf.rec.ptr=0;//接收指针清零
//		}
//	}
////	WriteString_Big(0, 150, (uint8_t *)&ComBuf.rec.buf[1]);
//	ComBuf.rec.end=0;

////	switch(kind)
////	{
////		case FRAME_READ_RESULT://读取结果
////			//串口发送测试数据:电压(5)+电阻(6)+时间(4)+分选(1)=16字节
////			switch (GetSystemMessage())//系统信息
////			{
////				case MSG_ABORT:
////					kind=0x9B;//测试中止
////					break;
////				case MSG_PASS:
////					kind=0x91;//测试通过
////					break;
////				case MSG_HIGH:
////					kind=0x92;//上限报警
////					break;
////				case MSG_LOW:
////					kind=0x92;//下限报警
////					break;
////				default:
////					kind=0x90;//正常测试
////					break;
////			}		
////			ComBuf.send.buf[1+5+6+4]=kind;
////			ComBuf.send.begin=0;
////			ComBuf.send.len=PackStandFrame(ComBuf.send.buf , &ComBuf.send.buf[1] , 16  );
//////			if(SendDataToCom()==0)//发送成功判别
//////			{
//////			//	Delay_1ms(100);//延时
//////			//	SendDataToCom();//发送
//////			}
////			break;
////		
////		case FRAME_START://启动
////			SetSystemStatus(SYS_STATUS_TEST);//系统状态-启动测试
////			break;

////		case FRAME_RESET://复位
////			//SetSystemStatus(SYS_STATUS_IDLE);//系统状态-待机
////			break;

////		case FRAME_WRITE_SN://写序列号
////			break;
////		
////		case FRAME_CLR_BOOT_NUM://清开机次数
////			break;
////		
////		case FRAME_DATA://数据帧
////			break;

////		default:
////			break;
////	}
//	return data;
//#endif
//}

////全局变量
//u8 WaitRecTimeOver;

//==========================================================
//函数名称：PackStandFrame
//函数功能：将数据打包成帧
//入口参数：*framebuf:帧数据缓冲
//			*datbuf:数据缓冲
//			len:数据长度
//出口参数：一帧数据长度
//创建日期：2014.04.11
//修改日期：2014.04.11 10:28
//备注说明：旧的发送协议
//开始(0xAB)，电压(5) 电阻(6) 时间(4)，分选(1)，结束(0xAF)=18字节
//==========================================================
int8_t PackStandFrame(int8_t * framebuf, int8_t * datbuf, int8_t len)
{
	if(len>(SEND_LEN_MAX-2))//数据的最大长度
		len=(SEND_LEN_MAX-2);
	framebuf[0]=UART_SEND_BEGIN;//帧头
	memcpy(&framebuf[1], datbuf, len);//数据
	framebuf[len+1]=UART_SEND_END;//帧尾
	return (len+1);//返回一帧数据长度
}

const u32 UNIT_VALUE[15]=
{
	1,1E3,1E6

};
Sort_TypeDef Input_compvalue(Disp_Coordinates_Typedef *Coordinates)
{
	u8 key;
	u8 page=0;
	u8 disp_flag=1;
//	u8 index=0;
	u8 disp_cound=0;
	u8 disp_pow=0;
//	u8 input_flag=0;
//	u32 unit_c;
	u8 dot=5;//小数点
	u8 dot_num=0;
	vu8 While_flag=1;
	vu32 keynum=0;
	vu8 Disp_buff[10];
	vu8 key_count;
	vu8 dot_num1;
	vu8 del_flag=0;
	Sort_TypeDef   Sort_set;
	
//	u8 unit=0;//单位
	u32 Word;
	
	float conp_value=0;
//	float old_value;
	key_count=0;
//	old_value=SaveData.SetParameter.Nominal_value.comp_value;
//	if(SaveData.SysParameter.language==0)
//		Disp_Inputback("输入数值");//背景色
//	else
//		Disp_Inputback("INPUTNUM");//背景色
	LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , 
	Coordinates->ypos+16 , Red );
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			disp_flag=1;
			switch(key)
			{
				case Key_F1:
					//unit_c=UNIT_VALUE[DispBuf[disp_cound]-'0'];
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=0;
						else
							Sort_set.Unit=4;
							
						While_flag=0;
					break;
				case Key_F2:
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=1;
						else
							Sort_set.Unit=5;
						While_flag=0;
					break;
				case Key_F3:
					conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						if(page==0)
							Sort_set.Unit=2;
						else
							Sort_set.Unit=6;
						While_flag=0;
					break;
				case Key_F4:
						if(page==0)
						{
						conp_value*=pow(10,5-(disp_cound-dot_num));
						//Sort_set.Num=	conp_value *unit_c/pow(10,dot);
						Sort_set.Num=conp_value;
						Sort_set.Dot=dot;
						Sort_set.Unit=3;
						While_flag=0;
						}
					break;
				case Key_F5:
					if(page)
						page=0;
					else
						page=1;
				
					break;
				case Key_Disp:
					SetSystemStatus(SYS_STATUS_TEST);
					While_flag=0;
					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
					While_flag=0;
					Sort_set.Updata_flag=0;
				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='1';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
							
					}
				break;
				case Key_NUM2:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM3:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM4:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
					
				break;
				case Key_NUM5:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM6:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM7:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='7';
						
						Word=Disp_buff[disp_cound]-'0';
						key_count++;
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM8:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM9:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_NUM0:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
						Word=Disp_buff[disp_cound]-'0';
						conp_value=conp_value*10+Word;
						disp_pow++;
						disp_cound++;
						if(dot_num==0)
							dot--;
					}
				break;
				case Key_DOT:
					if(dot_num==0)
					{
						dot_num1=key_count;
						Disp_buff[key_count]='.';
						dot_num=1;
						key_count++;
						disp_cound++;
					}
					break;
				case Key_BACK:
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
						del_flag=1;
					
					}
//					else
//					{
//						if(Disp_buff[key_count]==0)
//								Disp_buff[key_count]='-';
//							else if(Disp_buff[key_count]=='-')
//								Disp_buff[key_count]='+';
//							else
//								Disp_buff[key_count]='-';
//							key_count++;
//					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
			if(disp_flag)
			{
				disp_flag=0;
				Disp_button_Num_Input(page);
				Colour.Fword=White;
				Colour.black=Red;
				if(del_flag)
				{
					PutChar( Coordinates->xpos+(key_count+1)*10, Coordinates->ypos, 
					Disp_buff[key_count], Colour.Fword, Colour.black ); 
					del_flag=0;
					
				}
				else if(key_count>0)				
					PutChar( Coordinates->xpos+key_count*10, Coordinates->ypos, 
					Disp_buff[key_count-1], Colour.Fword, Colour.black ); 
			
			}
				
				
			
			
		}
			
	}
		
		
	return Sort_set;
}
	
	

//数字键输入显示
Sort_TypeDef Disp_NumKeyboard_Set(Disp_Coordinates_Typedef *Coordinates )
{
	vu8 While_flag=1;
	vu8 Disp_buff[10];
	vu8 key,i;
	vu8 dispflag=1;
	vu8 dot_num=0,dot_num1=0;
//	vu8 page=0;
	vu32 keynum=0;
	vu8 key_count=0;
//	vu32 Num[6]={1,10,100,1e3,1e4,1e5};
	Sort_TypeDef   Sort_set;
	Sort_set.Dot=0;
	Sort_set.Num=0;
	Sort_set.Unit=0;
	Sort_set.Num=0;
	for(i=0;i<6;i++)
	Disp_buff[i]=' ';
	Disp_buff[7]=0;
	
	while(While_flag)
	{
		key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
		if(keynum==KEY_NUM)
		{
			dispflag=1;
			Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					Sort_set.Unit=0;
					
					While_flag=0;
					if(key_count<NUM_LENTH)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				case Key_F2:
					Sort_set.Unit=1;
					
					While_flag=0;
					if(key_count<NUM_LENTH)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				case Key_F3:
					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
					
				break;
				case Key_Disp:
					SetSystemStatus(SYS_STATUS_TEST);
					While_flag=0;
					Sort_set.Updata_flag=0;
				break;
				case Key_SETUP:
					While_flag=0;
					Sort_set.Updata_flag=0;
				SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				case Key_FAST:
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
				break;
				case Key_DOWN:
				break;
				case Key_NUM1:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<NUM_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
					
					if(key_count<NUM_LENTH&&key_count>0)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_BACK:
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
					
					}
					else
					{
						if(Disp_buff[key_count]==0)
								Disp_buff[key_count]='-';
							else if(Disp_buff[key_count]=='-')
								Disp_buff[key_count]='+';
							else
								Disp_buff[key_count]='-';
							key_count++;
					}
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
			LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=Red;
			WriteString_16(Coordinates->xpos, Coordinates->ypos, Disp_buff,  0);
			//dispflag=0;
		}
	
	}
	for(i=key_count;i<NUM_LENTH;i++)
		Disp_buff[i]='0';
	for(i=0;i<NUM_LENTH;i++)
	{
		if(Disp_buff[0]>='0'&&(Disp_buff[0]<='9'))
		{
			if(Disp_buff[i]>='0'&&(Disp_buff[i]<='9'))
			{
			
				if(dot_num1>i)
				{
					Sort_set.Num*=10;
					Sort_set.Num+=Disp_buff[i]-'0';
				
				}
				else
				{
					Sort_set.Num*=10;
					Sort_set.Num+=Disp_buff[i]-'0';
				
				
				}
			}
			
			
			//Sort_set.Num+=(Disp_buff[key_count-1]-'0');
		
		
		}
	
	
	
	}
	Sort_set.Dot=dot_num1;
//	if(Disp_buff[0]>='0'&&(Disp_buff[0]<'9'))
//		{
//			if(Disp_buff[key_count-1]!='.')		
//			{
//				Sort_set.Num*=Num[key_count-dot_num-1];
//				Sort_set.Num+=(Disp_buff[key_count-1]-'0');//*Num[key_count-dot_num-1];
//				
//			}				
//			//*(Disp_buff[key_count-1]-'0'))+=Num[key_count-dot_num-1];
//			else
//			{
//			
//			}
//		
//		
//		}
//			
//		else 
//			;//(Disp_buff[key_count-1]-'0')*Sort_set.Num+=Num[key_count-dot_num-2];
	return Sort_set;

}
//电阻设置
Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;

}
//电压设置
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_Freq();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Input_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;

}
Sort_TypeDef Disp_Set_InputNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num1;
	Disp_button_Num_Input(0);
	Sort_num1=Input_compvalue(Coordinates);
	//if(SaveData.Limit_Tab.Mode==0)
	Sort_num1=Input_Set_Cov(&Sort_num1);
	//else
	//Sort_num1=Input_Set_CovPre(&Sort_num1);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;
	
	

}
Sort_TypeDef Disp_Set_InputpreNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num1;
	Disp_button_Num_Input(0);
	Sort_num1=Input_compvalue(Coordinates);//Input_Set_Cov
	if(SaveData.Limit_Tab.Mode==0)
		Sort_num1=Input_Set_Cov(&Sort_num1);
	else
		Sort_num1=Input_Set_CovPre(&Sort_num1);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;
	
	

}


vu16 Freq_Set_Num(Disp_Coordinates_Typedef *Coordinates)//频率设置
{
	Sort_TypeDef Sort_num;
//	vu8 i;
	vu16 num;
	Disp_button_Num_Freq();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	num= Debug_Value(&Sort_num);
	//Sort_num1=Freq_Set_Cov(&Sort_num);
	
	
		
	return num;
	
}
vu8 Avg_Set_Num(Disp_Coordinates_Typedef *Coordinates)//平均数设置
{
	Sort_TypeDef Sort_num,Sort_num1;
	vu8 i;
	vu8 num;
	Disp_button_Num_Avg();
	Sort_num=Disp_NumKeyboard_Set(Coordinates);
	Sort_num1=Freq_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
	if(Sort_num1.Dot==0)
	{
		if(Sort_num1.Num>32)
			Sort_num1.Num=32;
	
	} else
	if(Sort_num1.Dot==1)
	{
		for(i=0;i<5;i++)
		{
			if(Sort_num1.Num>0&&Sort_num1.Num<=9)
				break;
			else
				Sort_num1.Num/=10;
		
		
		}
	
	}else if(Sort_num1.Dot==2)
	{
		for(i=0;i<5;i++)
		{
			if(Sort_num1.Num>10&&Sort_num1.Num<=99)
				break;
			else
				Sort_num1.Num/=10;
		
		
		}
		if(Sort_num1.Num>32)
			Sort_num1.Num=32;
	
	
	
	}else
	{
		Sort_num1.Num=32;
	
	
	}
	num=Sort_num1.Num;
	if(num==0)
		num=1;
		
	return num;
	
}
void Set_daot(vu8 *buff,vu8 dot)
{
	vu8 i;
	for(i=0;i<dot;i++)
	{
		buff[5-i]=buff[5-i-1];
	
	
	}
	if(dot==0)
		buff[5]=' ';
	else
	buff[5-dot]='.';
	buff[6]=0;


}
int8_t V_Test_Comp(int32_t value)
{
	vu8 data;
	
	if(value>Save_Res.Set_Data.V_high.mid_data)
		data=VH_FAIL;
	else if(value<Save_Res.Set_Data.V_low.mid_data)
		data=VL_FAIL;
	else
		data=ALL_PASS;
	return data;
		


}
int8_t R_Test_Comp(int32_t value)
{
	vu8 data;
	
	if(value>Save_Res.Set_Data.High_Res.mid_data)
		data=RH_FAIL;
	else if(value<Save_Res.Set_Data.Res_low.mid_data)
		data=RL_FAIL;
	else
		data=ALL_PASS;
	return data;

}
void Comp_prompt(int8_t value)
{
	if(value==ALL_PASS)
	{
		Pass_Led();
	
	}
	else
	{
		Fail_led();
	
	}
	switch(Save_Res.Set_Data.beep)
	{
		case 0://蜂鸣器关闭
			Beep_Off();
			break;
		case 1://合格讯响
			if(value==ALL_PASS)
				Beep_on();
			else
				Beep_Off();
			break;
		case 2://不合格讯响
			if(value==ALL_PASS)
				Beep_Off();
			else
				Beep_on();
				
			
			break;
		default:
			Beep_Off();
			break;
	
	}



}
void Test_Comp(All_Compvalue_Typedef *pt)
{
	float value;
	float data;
	vu8 i;
	for(i=0;i<5;i++)
	{
		data=pt->all[0].buff[i];
		value+=(pow(10,4-i)*data);//从下位机接收来的数据
	}
	value=value*pow(1000,pt->all[0].Unit);
	value/=(pow(10,pt->all[0].Dot));
	
	if(SaveData.Limit_Tab.Mode)//百分比比较
	{
		for(i=0;i<9;i++)
		{
			if(SaveData.Limit_Tab.Comp_Value[i].low.Num!=0)
			{
				if(value>Comp_Testvalue.comp_highvalue[i])//大于上限
					Count_buff[10]++;
				else if(value<Comp_Testvalue.comp_lowvalue[i])//小于下限 加合格标志位
									//计数
					Count_buff[10]++;
				else Count_buff[i]++;
			
			
			}
		
		
		}
	
	
	
	}
//	else
//	{
//	
//	
//	}//ABS比较

}//Comp_Testvalue  Comp_Testvalue
float Inttofloat(Sort_TypeDef *pt)//int转换为float  INT包含小数点和单位
{
	float value;
//	vu8 i;
	value=pt->Num;
	value=value*pow(1000,pt->Unit);
	value/=pow(10,pt->Dot);
	return value;
}
void Set_Compbcd_float(void)//把设置比较数据转换为float数据  把这个数据与标称值进行运算，得到
	//可以直接比较的值  这个分为2种，一种是ABS一种是%
{
	vu8 i;
	float value;
	for(i=0;i<9;i++)
	{
		Comp_Testvalue.comp_highvalue[i]=Inttofloat( &SaveData.Limit_Tab.Comp_Value[i].high);
		Comp_Testvalue.comp_lowvalue[i]=Inttofloat( &SaveData.Limit_Tab.Comp_Value[i].low);
	
	
	}
	Comp_Testvalue.comp_highvalue[9]=Inttofloat( &SaveData.Limit_Tab.Comp_Value_2nd.high);
	Comp_Testvalue.comp_lowvalue[9]=Inttofloat( &SaveData.Limit_Tab.Comp_Value_2nd.low);
	value=Inttofloat(&SaveData.Limit_Tab.Nom);
	if(SaveData.Limit_Tab.Mode)//百分比比较
	{
		for(i=0;i<9;i++)
		{
			Comp_Testvalue.comp_highvalue[i]=(100000+Comp_Testvalue.comp_highvalue[i])*value;
			Comp_Testvalue.comp_highvalue[i]/=100000;
			Comp_Testvalue.comp_lowvalue[i]=(100000-Comp_Testvalue.comp_lowvalue[i])*value;
			Comp_Testvalue.comp_lowvalue[i]/=100000;
			
		}
	
	
	
	}
	else
	{
		
		for(i=0;i<9;i++)
		{
			Comp_Testvalue.comp_highvalue[i]+=value;
			Comp_Testvalue.comp_lowvalue[i]-=value;
			
		}
		//Comp_Testvalue.comp_highvalue[9]=
		
	
	
	}//ABS比较
	



}
int32_t Input_int(int8_t *pt)
{
	u32 value=0;
	u8 i,j=0;
//	u8 dot=0;
	for(i=0;i<5;i++)
	{
		if(*(pt+i+1)>='0')
		{
			value*=10;
			value+=*(pt+i+1)-'0';
			j++;
			
		}
//		else
//			dot=4-i;
	
	}
	return value;


}
//电阻BCD转换为INT
int32_t BCDtoInt(int8_t *pt)
{
	u32 value=0;
	u8 i,j=0;
	u8 dot=0;
	if(*(pt+1)=='-')
	{
		value=0xfffffff;
		return value;
	}
	for(i=0;i<5;i++)
	{
		if(*(pt+1+i)>='0')
		{
			value*=10;
			value+=*(pt+1+i)-'0';
			j++;
			
		}
		else
			dot=4-i;
	
	}
	if(Test_Dispvalue.Unit[0])
		value*=1e6;
	else
		value*=1e3;
	value/=pow(10,dot);
	
	return value;
}
//电压BCD转换为INT
int32_t VBCDtoInt(int8_t *pt)
{
	u32 value=0;
	
	u8 i,j=0;
	u8 dot=0;
	for(i=0;i<5;i++)
	{
		if(*(pt+1+i)>='0')
		{
			value*=10;
			value+=*(pt+1+i)-'0';
			j++;
			
		}
		else
			dot=4-i;
		
	
	}
	Test_Unit.V_dot=dot;
	value*=1000;
	value/=pow(10,dot);
	if(*pt=='-')
		Test_Unit.V_Neg=0;
	else
		Test_Unit.V_Neg=1;
	
	return value;
}
void V_BCD_Int(int32_t data)
{
	u8 i;
	vu16 word;
	if(data>60e3)
	{
		for(i=0;i<5;i++)
		DispBuf[i]='-';
	}
	else 
		if(data>6e3)
		{
			word=data/10;
			Hex_Format((word),2,4,FALSE);
		
		
		}
		else
			Hex_Format((data),3,4,FALSE);
			


}
void BCD_Int(int32_t data)
{
	u8 i;
	vu16 word;
	if(data>30e6)
	{
		Test_Unit.Res_dot=1;
		for(i=0;i<5;i++)
		DispBuf[i]='-';
	
	}else
		if(data>=3e6)//xx.xx
		{
			Test_Unit.Res_dot=1;
			word=data/10e3;
			Hex_Format((word),2,4,FALSE);
				
		}else
			if(data>=3e5)//x.xxx
			{
				Test_Unit.Res_dot=1;
				word=data/10e2;
				Hex_Format((word),3,4,FALSE);
			}
			else
				if(data>=3e4)//xxx.xm
				{
					Test_Unit.Res_dot=0;
					word=data/10e1;
					Hex_Format((word),1,4,FALSE);
				}else
				if(data>=3e3)
				{
					Test_Unit.Res_dot=0;
					word=data/10;
					Hex_Format((word),2,4,FALSE);
				}
				else
				{
					Test_Unit.Res_dot=0;
					word=data/10;
					Hex_Format((word),2,4,FALSE);
				
				
				}


}
float Debug_Res(float a,float b,float c)
{
	//float d;
	return a*b/c;
}
//void StrToFloat(u8 *str)
//{


//}
void  Write_Usbdata (volatile uint8_t  *buffer,uint32_t num)
{
    int32_t  fdw;
//    uint32_t  bytes_written;
        fdw = FILE_Open(FILENAME_W, RDWR);
        if (fdw > 0) 
		{
			usb_oenflag=1;

			//bytes_written = FILE_Write(fdw, buffer, num);//MAX_BUFFER_SIZE);
            FILE_Write(fdw, buffer, num);

			FILE_Close(fdw);
            
        } 
		else
			usb_oenflag=0;

    } 

void  Main_Read (void)
{
    int32_t  fdr,i;
//    uint32_t  bytes_read;
    

    fdr = FILE_Open("data.txt", RDONLY);
    if (fdr > 0) {
        //PRINT_Log("Reading from %s...\n", FILENAME_R);
//        do {
           // bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
        FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
 //       } while (bytes_read);

        FILE_Close(fdr);
//        PRINT_Log("Read Complete\n");
		for(i=0;i<7;i++)
		{
			Save_Res.Production_Data.buff[i]=*(UserBuffer+i);
			Save_Res.Number.buff[i]=*(UserBuffer+8+i);
			
		}
		Save_Res.Production_Data.flag=0x55;
		Save_Res.Number.flag=0x55;
		Savetoeeprom();
    } 

}

