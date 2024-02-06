#include "LCD/lpc177x_8x_lcd.h"
#include"EX_SDRAM/EX_SDRAM.H"
#include "LCD/AsciiLib.h"
#include "use_disp.h"
#include  "stdint.h"
#include  "stdio.h"
#include "Key/key.h"
#include "test/Test.h"
#include  "Globalvalue/globalvalue.h"
#include "lpc177x_8x_eeprom.h"
#include "timer.h"
#include <math.h>
#include "lpc177x_8x_rtc.h"
const uint8_t Num_1[][9]=
    
{"1","2","3","4","5","6","7","8","9"};
const uint8_t TFT5520_state[][2][20+1]=
{
    {{"等待测试"},{"Wait For Test"}},
    {"测试中  ","Testing  "},
    {"暂停测试","Pause Output"},
    {"测试合格","Pass Test"},
    {"小于下限","Less lower"},
    {"大于上限","Greater Than Upper"},
    {"输出短路","Short Output"},
    {"高端开路","High Open"},
    {"低端开路","Low Open"},
    {"开路清零","Clear"},
    
};

const uint8_t Test_Setitem[][2][9+1]=
{
	{{"组    别:"},{"    Step:"}},
	{{"电阻上限:"},{"HighLimt:"}},
	{{"电阻下限:"},{"lowLimit:"}},
	{{"量    程:"},{"   Range:"}},
	{{"输出电压:"},{"OutpVolt:"}},
	{{"判定时间:"},{"JudgTime:"}},
};

//const uint8_t Test_Setitem_E[][9+1]=
//{
//	{"组    别:"},
//	{"电阻上限:"},
//	{"电阻下限:"},
//	{"量    程:"},
//	{"输出电压:"},
//	{"判定时间:"},
//};
const uint8_t Test_Tirpvalue[][6+1]=
{
	{"自动"},
	{"手动"}

};
const uint8_t Test_Speedvalue[][2][6+1]=
{
	
	{{"慢速"},{"Fast"}},
    {{"快速"},{"Slow"}},
};
const uint8_t Test_Compvalue[][2][6+1]=
{
	{{"关闭"},{"Off "}},
	{{"打开"},{"On  "}},

};
const uint8_t Sys_Disp_Main[][12+1]=
{
	{"型号        "},
	{"仪器序列号  "},
	{"仪器程序版本"},
	{"操作系统    "},
	{"OS版本      "},
	{"信号处理器  "},
	{"USB接口     "},
	
};
const uint8_t Sys_config_Disp_Main[][9+1]=
{
	{"语言     "},
//	{"讯响     "},
	{"串口开关 "},
	{"波特率   "},
	{"U盘开关  "},
	
	{""}
};
	
const uint8_t Button_TipPage1[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Cp-..  "},
    {"Cs-..  "},
    {"Lp-..  "},
    {"Ls-..  "},
    {" "},


};
const uint8_t Button_TipPage2[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Z-..  "},
    {"Y-..  "},
    {"R-..  "},
    {"G-B "},
    {" "},


};
const uint8_t Button_TipPage3[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"DCR  "},
    {" "},
    {" "},
    {""},
    {" "},


};
const uint8_t Cp_Button_Tip[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Cp-D"},
    {"Cp-Q"},
    {"Cp-G"},
    {"Cp-Rp"},
    {"RETURN "},


};
const uint8_t Cs_Button_Tip[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Cs-D"},
    {"Cs-Q"},
    {"Cs-Rs"},
    {"    "},
    {"RETURN "},


};
const uint8_t Lp_Button_Tip1[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Lp-Q"},
    {"Lp-Rp"},
    {"Lp-Rd"},
    {"Lp-D"},
    {"RETURN "},


};
const uint8_t Lp_Button_Tip2[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Lp-D"},
    {"Lp-G"},
    {"    "},
    {"    "},
    {"RETURN "},


};
const uint8_t Ls_Button_Tip[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Ls-D"},
    {"Ls-Q"},
    {"Ls-Rs"},
    {"Ls-Rd"},
    {"RETURN "},


};
const uint8_t Z_Button_Tip[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Z-d"},
    {"Z-r"},
    {" "},
    {" "},
    {"RETURN "},


};

const uint8_t Y_Button_Tip[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"Y-d"},
    {"Y-r"},
    {" "},
    {" "},
    {"RETURN "},


};
const uint8_t R_Button_Tip[][7+1]=  //测试参数选择时候的下面的提示符号
{
    {"R-X"},
    {"Rp-Q"},
    {"Rs-Q "},
    {" "},
    {"RETURN "},


};
const uint8_t User_FUNC[][8+1]=
{
	{"Cp-D"},
	{"Cp-Q"},
    {"Cp-G"},
	{"Cp-Rp"},
	{"Cs-D"},
	{"Cs-Q"},
    {"Cs-Rs"},
    {"Lp-Q"},//7
    {"Lp-Rp"},
    {"Lp-Rd"},
    {"Lp-D"},
//    {"Lp-G"},//10
    {"Ls-D"},
    {"Ls-Q"},
    {"Ls-Rs"},
    {"Ls_Rd"},//14
    {"Z-d"},
    {"Z-r"},
    {"Y-d"},
    {"Y-r"},//18
	{"R-X"},
	{"Rp-Q"},
	{"Rs-Q"},
    {"G-B"},
    {"DCR"}
   


};
const uint8_t RangeDisp_Main[][3+1]=
{
	{"Cp:"},{"Cp:"},{"Cp:"},{"Cp:"},
	{"Cs:"},{"Cs:"},{"Cs:"},
	{"Lp:"},{"Lp:"},{"Lp:"},{"Lp:"},
	{"Ls:"},{"Ls:"},{"Ls:"},{"Ls:"},
	{"Z :"},{"Z :"},
	{"Y :"},{"Y :"},
	{"R :"},{"Rp:"},{"Rs:"},
	{"G :"}
	
};
const uint8_t RangeDisp_Second[][3+1]=
{
	{"D :"},{"Q :"},{"G :"},{"Rp:"},
	{"D :"},{"Q :"},{"rS:"},
	{"Q :"},{"Rp:"},{"Rd:"},{"D :"},
	{"D :"},{"Q :"},{"Rs:"},{"Rd:"},
	{"r :"},{"r :"},
	{"r :"},{"r :"},
	{"X :"},{"Q :"},{"Q :"},
	{"B :"}
};
const uint8_t Fun_SelectValue[4][7]=
{
	{0,4,	7,	11,	15,	17,	19},
	{1,5,	8,	12,	16,	18,	20},
	{2,6,	9,	13,	0xff,0xff,21},
	{3,0xff,10,	14,	0xff,0xff,0xff}


};
const uint32_t FreqNum[]=
	{20,25,30,40,50,60,75,100,120,150,200,250,300,400,500,600,750,
	1e3,1200,1500,2e3,2500,3e3,4e3,5e3,6e3,7500,
	1e4,12e3,15e3,2e4,25e3,3e4,4e4,5e4,6e4,75e3,
	1e5};
const uint8_t User_Freq[][10+1]=
{
	{"20.000 Hz"},
	{"25.000 Hz"},
	{"30.000 Hz"},
	{"40.000 Hz"},
    {"50.000 Hz"},
    {"60.000 Hz"},
    {"75.000 Hz"},
    {"100.00 Hz"},
    {"120.00 Hz"},
    {"150.00 Hz"},
    {"200.00 Hz"},
    {"250.00 Hz"},
    {"300.00 Hz"},
    {"400.00 Hz"},
    {"500.00 Hz"},
    {"600.00 Hz"},
    {"750.00 Hz"},
    {"1.0000KHz"},
    {"1.2000KHz"},
    {"1.5000KHz"},
    {"2.0000KHz"},
    {"2.5000KHz"},
    {"3.0000KHz"},
    {"4.0000KHz"},
    {"5.0000KHz"},
    {"6.0000KHz"},
    {"7.5000KHz"},
    {"10.000KHz"},
    {"12.000KHz"},
    {"15.000KHz"},
    {"20.000KHz"},
    {"25.000KHz"},
    {"30.000KHz"},
    {"40.000KHz"},
    {"50.000KHz"},
    {"60.000KHz"},
    {"75.000KHz"},
    {"100.00KHz"},
	{"120.00KHz"},
	{"150.00KHz"},
	{"200.00KHz"},


};
const uint8_t Setup_Beep[][2][6+1]=
{
	{"关闭  ",{"Off "}},
	{"合格  ",{"Pass"}},
	{"不合格",{"Fail"}},

};
const uint8_t *Setup_Valueall[]=
{
	*User_FUNC,
	*User_Freq,



};
const uint8_t FreqButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"DECR --"},
    {"DECR -"},
    {"INCR +"},
    {"INCR ++"},
	{" "}

};
const uint8_t User_Level[][10+1]=
{
    {"0.100 V"},
    {"0.300 V"},
	{"1.000 V"},

};
const uint8_t LevleButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"x1000"},
    {"x100 "},
	{"x10  "},
	{"x1   "},
	{"单位 "},

};

const uint8_t User_Range[][10+1]=
{
    {"AUTO   "},
    {"2MΩ   "},
	{"20MΩ  "},
	{"200MΩ "},
	{"10000MΩ"},

};
const  uint8_t Auto_RangeDisp[][12+1]=
{
	{"AUTO_2M   "},
	{"AUTO_20M  "},
	{"AUTO_200M "},
	{"AUTO_10000M"},

};
const  uint8_t Hand_RangeDisp[][12+1]=
{
	{"HAND_2M   "},
	{"HAND_20M  "},
	{"HAND_200M "},
	{"HAND_10000M"},

};
const uint8_t RangeButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"AUTO"},
    {"HOLD"},
    {"DECR -"},
    {"INCR +"},

};
const uint8_t User_Biad[][10+1]=
{
    {"---- "},
   

};
const uint8_t BiasButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"DECR -"},
    {"INCR +"},

};
const uint8_t User_Speed[][10+1]=
{
    {"FAST "},
    {"MED "},
    {"SLOW "},
   
};
const uint8_t SpeedButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"FAST "},
    {"MED "},
    {"SLOW "},

};
const uint8_t User_Comp[][10+1]=
{
    
    {"OFF "},
	{"ON  "},
   
};
const uint8_t Limit_Scan_Fun[][4+1]=
{
	{"SEQ"},
	{"STEP"},

};
const uint8_t Limit_Scan_Page[][6+1]=
{
	{"上一页"},
	{"下一页"},

};
	
const uint8_t Sys_Bais_Value[][6+1]=
{
	{"INT"},
	{"JK1788"}


};
const uint8_t Sys_Addr_value[][6+1]=
{
	{"RS232C"},
	{"GPIB"},
	{"USBTMC"},
	{"USBCDC"},
	{"RS485"},


};
const uint8_t Switch_Value[][4+1]=
{
	{"OFF"},
	{"ON"}

};
const uint8_t CompButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"ON  "},
    {"OFF "}
};
const uint8_t User_ListMode[][10+1]=
{
    {"SEQ"},
    {"STEP "},
   
};
const uint8_t ListModeButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"SEQ "},
    {"SETP "}
};
const uint8_t Setup_Trig[][10+1]=
{
    {"INT "},
    {"MAN "},
    {"EXT "},
    {"BUS "},
   
};
const uint8_t SetupTrig_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"INT "},
    {"MAN "},
    {"EXT "},
    {"BUS "},
};
const uint8_t Setup_Alc[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t SetupAlc_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Setup_Rsou[][10+1]=
{
    {"100Ω  "},
    {"30Ω   "},
   
};
const uint8_t SetupRsou_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"100Ω"},
    {"30Ω"},
};
const uint8_t SetupTrigDly_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"DECR -"},
    {"INCR +"},
};
const uint8_t Setup_Rev_A[][10+1]=
{
    {"ABS  "},
    {"%   "},
    {"OFF   "},
   
};
const uint8_t SetupRev_A_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"ABS"},
    {"%  "},
    {"OFF"},
};
const uint8_t Setup_Rev_B[][10+1]=
{
    {"ABS  "},
    {"%   "},
    {"OFF   "},
   
};
const uint8_t SetupRev_B_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"ABS"},
    {"%  "},
    {"OFF"},
};
const uint8_t SetupAvg_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"DECR -"},
    {"INCR +"},
};
const uint8_t Setup_Vm[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t SetupVm_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Setup_Dcr[][10+1]=
{
    {"FIX  "},
    {"ALT "},
   
};
const uint8_t SetupDcr_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"FIX  "},
    {"ALT "},
};
const uint8_t SetupRefa_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"MEASURE"},
};
const uint8_t SetupRefB_Button_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"MEASURE"},
};

const uint8_t Correct_Open[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t CorrectOpen_Button_Tip[][7+1]=  //比较界面选择时候的下面的提示符号
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Correct_Short[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t CorrectShort_Button_Tip[][7+1]=  //比较界面选择时候的下面的提示符号
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Correct_Load[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t CorrectLoad_Button_Tip[][7+1]=  //比较界面选择时候的下面的提示符号
{
    {"ON  "},
    {"OFF "},
};
const uint8_t CorrectSpot_Button_Tip[][7+1]=  //比较界面选择时候的下面的提示符号
{
    {"DECR --"},
    {"DECR -"},
    {"INCR --"},
    {"INCR ++"}
};
const uint8_t Correct_Freq[][10+1]=
{
    {"ON  "},
    {"OFF "},
   
};
const uint8_t CorrectFreq_Button_Tip[][7+1]=  //比较界面选择时候的下面的提示符号
{
    {"ON  "},
    {"OFF "},
};
const uint8_t Correct_Cable[][10+1]=
{
    {"0m  "},
    {"1m "},
    {"2m  "},
    {"4m "},
   
};
const uint8_t CorrectCable_Button_Tip[][7+1]=  //比较界面选择时候的下面的提示符号
{
    {"0m "},
    {"1m "},
    {"2m "},
    {"4m "},
};
//const uint8_t User_ListNo[][10+1]=
//{
//    {"SEQ"},
//    {"STEP "},
//   
//};
//const uint8_t ListNoButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
//{
//    {"SEQ "},
//    {"SETP "}
//};
//const uint8_t Test_Setitem_E[][9+1]=
//{
//	{"FUNC    :"},
//	{"FREQ    :"},
//	{"LEVEL   :"},
//	{"RANGE   :"},
//    {"BIAS    :"},
//	{"SPEED   :"},
//	
//	{"Vm:"},
//	{"Im:"},
//	{"CORR:"}
//	
//};
const uint8_t  Range_Item[][7+1]=
{
	{"功能  :"},
	{"频率  :"},
	{"电平  :"},
	{"量程  :"},
	{"偏置  :"},
    {"速度  :"},
    {"比较  :"},
};
const uint8_t  Range_Item_E[][7+1]=
{
	{"FUNC  :"},
	{"FREQ  :"},
	{"LEVEL :"},
	{"RANGE :"},
	{"BIAS  :"},
	{"SPEED :"},
	{"COMP  :"}
};
const uint8_t	Set_testitem[][2][9+1]=
{
	{{"组别    :"},{"  Assort:"}},
	{{"速度    :"},{"   Speed:"}},
	{{"         "},{"         "}},
	{{"设置电压:"},{"VoltSett:"}},
	{{"测试时间:"},{"TestTime:"}},
	{{"         "},{"        "}},
	{{"高端开路:"},{"HighOpen:"}},
	{{"低端开路:"},{"Low Open:"}},
	{{"        "},{" "}},
	
	{{"量程    :"},{"   Range:"}},
	{{"讯响    :"},{"   Alarm:"}},
	{{"         "},{" "}},
	{{"电阻上限:"},{"UpperLim:"}},
	{{"电阻下限:"},{"LowerLim:"}},
	
	{{"         "},{" "}},
	{{"短路检测:"},{"ShortDet:"}},
	{{"         "} ,{" "}},
    {{"         "},{" "}},
};
const uint8_t	Set_testitem_E[][9+1]=
{
	{"FUNC    :"},
	{"FREQ    :"},
	{"LEVEL   :"},
	{"TRIG    :"},
	{"ALC     :"},
	{"Rsou    :"},
	{"TRIG DLY:"},
	{"STEP DLY:"},
	{"DEV A   :"},
	{"RANGE   :"},
    {"BIAS    :"},
	{"SPEED   :"},
	
	{"AVG     :"},
	{"Vm/Im   :"},
	{"DCR POL :"},
	{"DCR RNG :"},
	{"DC LEV  :"},
	{"REF A   :"}
};
const uint8_t Sys_Sys[][2][20+1]=
{
	{{"仪器型号  JK2683B"},{"Model:  JK2683B"}},
	{{"软件版本  Ver:1.03"},{"Sort Ver:1.03"}},//0  表示LPC更新  1STM32更新
	{{"硬件版本  Ver:1.01"},{"Harn Ver:1.01"}},
//	{"生产日期  "},
//	{"仪器编号  "},
//	{"出厂日期"},
//	{"账号    "},



};
//const uint8_t PRU_DATA[]=
//{""};
const uint8_t Sys_Setitem[][2][10+1]=
{
	{{"串口开关:"},{"SerSwitch:"}},
	{{"波特率  :"},{"Baud Rate:"}},
	{{"U盘开关 :"},{"USB Disk :"}},
	{{"I/O口   :"},{"I/O Port:"}},
	{{"显示语言:"},{"Language:"}},
	{{"通讯地址:"},{"CommAddr :"}},
	
//	{"日期    "},
//	{"时间    "},
//	{"账号    "},
	
};
const uint8_t Sys_Setitem_E[][10+1]=
{
	{"MAIN FUNC:"},
	{"PASS BEEP:"},
	{"FAIL BEEP:"},
	{"LANGUAGE :"},
	{"PASS WORD:"},
	{"MENU DISP:"},
	{"DATA     :"},
	{"BAUD RATE:"},
	{"BIAS SRC :"},
	{"BUS MODE :"},
	{"GPIBADDR :"},
	{"TALK ONLY:"},
	{"TIME     :"}
};
const uint8_t   TFT5520_NAME[][2][12+1]=
{
    {{"< 测量显示 >"},{"<MEAS DISP>"}},
    {{"< 参数设置 >"},{"<MEAS SETUP>"}},
    {{"< 系统设置 >"},{"<SYS SETUP>"}},
    {{"< 系统信息 >"},{"<SYS INFO>"}},
};
const uint8_t All_TopName[][21+1]=
{
	{"< 测量显示 >"},
	{"< 档号显示 >"},
	{"< 档计数显示 >  "},
	{"< 列表扫描显示 >"},
	{"< 测量设置 >    "},
	{"< 用户校正 >"},
	{"< 极限列表设置 >"},
	{"< 列表扫描设置 >"},
	{"< 系统设置 >    "},
	{"[ LCR文件列表 ] "},
	{"< 系统显示 >    "},

};
const uint8_t All_TopName_E[][21+1]=
{
	{"< MEAS DISPLAY >     "},
	{"< BIN No. DISP >     "},
	{"< BIN COUNT DISP >   "},
	{"< LIST SWEEP DISP >  "},
	{"< MEASURE SETUP >    "},
	{"< CORRECTION >       "},
	{"< LIMIT TABLE SETUP >"},
	{"< LIST SWEEP SETUP > "},
	{"< SYSTEM SETUP >     "},
	{"[ LCR文件列表 ]      "},

};
const uint8_t Range_Count_Item[][6+1]=
{
	{"参数 :"},
	{"标称 :"},
	{"计数 :"}

};
const uint8_t Range_Count_Item_E[][6+1]=
{
	{"PARAM :"},
	{"NOM.  :"},
	{"COUNT :"}

};
const uint8_t User_Check_main[][12+1]=
{
	{"档号    "},
	{"标准值  "},
	{"测试AD值"},

};
const uint8_t User_Check_Item[][12+1]=
{
	{"1000V   "},
	{"1.000MΩ"},
	{"10.00MΩ"},
	{"100.0MΩ"},
	{"1000 MΩ"},
	
//	{" 开路   A:"},
//	{" 短路   A:"},
//	{" 负载   A:"},
//	{"电缆  :"},
//	{"方式  :"},
//	{"功能  :"},
//	{"       "},
//	{"  B:"},
//	{"  B:"},
//	{"       "},
//	{"  B:"},
//	{"  B:"}



};
const uint8_t User_Check_Item_E[][12+1]=
{
	{"OPEN  :"},
	{"SHORT :"},
	{"LOAD  :"},
	{"SPOT No.  :"},
	{"FREQ      :"},
	{" REF     A:"},
	{" OPEN    A:"},
	{" SHORT   A:"},
	{" LOAD:   A"},
	{"CABLE :"},
	{"MODE  :"},
	{"FUNC  :"},
	{"       "},
	{"  B:"},
	{"  B:"},
	{"       "},
	{"  B:"},
	{"  B:"}



};
const uint8_t User_LimitList_Item[][7+1]=
{
 	{"参数  :"},
	{"标称  :"},
	{"方式  :"},
	{"附属  :"},
	{"比较  :"},
	{""},


};
const uint8_t User_LimitList_Value[][3+1]=
{
	
	"ABS",
	" % ",
};
const uint8_t User_LimitList_Item_E[][7+1]=
{
 	{"PARAM :"},
	{"NOM   :"},
	{"MODE  :"},
	{"AUX   :"},
	{"COMP  :"},
	{""},


};
const uint8_t User_LimitList_Item2[][3+1]=
{
	{"BIN"},
	{" 1"},
	{" 2"},
	{" 3"},
	{" 4"},
	{" 5"},
	{" 6"},
	{" 7"},
	{" 8"},
	{" 9"},
	{"2nd"}


};
const uint8_t User_LimitScan_Item2[][3+1]=
{
	{"No."},
	{"001"},
	{"002"},
	{"003"},
	{"004"},
	{"005"},
	{"006"},
	{"007"},
	{"008"},
	{"009"},
	{"010"}


};
const uint8_t User_ListScan_Item[][8+1]=
{
	{"模式 :"},
	{"频率[Hz]"},
    {"电平[V]"},
	{"LMT"},
	{"下限"},
	{"上限"},
    {"延时[s]"},

};

const uint8_t User_ListScan_Item_E[][8+1]=
{
	{"MODE :"},
	{"FREQ[Hz]"},
    {"LEVEL[V]"},
	{"LMT"},
	{"LOW "},
	{"HIGH"},
    {"DELY[s]"},

};
const uint8_t Sys_Language_Value[][7+1]=
{
	
	{"ENGLISH"},
	{"中文"},

};
const uint8_t Sys_Buard_Value[][6+1]=
{
	{"2400"},
	{"4800"},
	{"9600"},
	{"14400"},
	{"19200"},
	{"38400"},
	{"56000"},
	{"115200"},

};
const uint8_t Set_Unit[][5+1]=
{
	{"p"},
	{"n"},
	{"u"},
	{"m"},
	{"next"},
	{"*1"},
	{"k"},
	{"M"},
	
	{""},
	{"back"},



};
const uint8_t Disp_Unit[][2+1]=
{"p","n","u","m"," ","k","M"};
const uint8_t Disp_Range_Main_NUm[]={0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,3,3,3,2,2,1,1};
const uint8_t Disp_Range_Main_Disp[][2+1]=
{	"F",
	"H",
	"Ω",
	"S",
	"r",
	"°"
};//F,H,Ω，S r °空格
vu32 const Main_Func_Comp[][2]=
{
	{0,21},
	{0,33},
	{0,1},
	{0,3},
	{0,0},
	{0,0},
	{0,60000},
	{0,60000},
	{0,2},
	{0,2},
	{0,7},//量程
	{0,0},
	{0,2},
	{0,16},
	{0,1},
	{0,0},
	{0,0},
	{0,0},
	{0,999999},
	{0,5},
	{0,7},
	{0,999999},
	{0,5},
	{0,7}


};
const vu16 Debug_Comp_ad[][2]=
{
	{800,1500},
	{800,1500},
	{800,1500},
	{800,1500},
	{800,1200},
	


};
const vu16 Debug_Comp_ad1[]=
{
	1000,1000,1000,1000, 1000,3000
//	{800,1500},
//	{800,1500},
//	{800,1500},
//	{800,1500},
//	{1500,5500},
//	{1500,5500},


};
const vu16 Debug_Comp[][2]=
{
	{800,1200},
	{800,1200},
	{800,1200},
	{800,1200},
	{800,1200},
	


};
const vu16 Debug_Comp1[]=
{ 1000,1000,1000,1000, 1000,1000
	

};
//显示 下面的快捷键按键
void Disp_Fastbutton(void)
{
	uint32_t i;
	for(i=0;i<6;i++)
	{
		LCD_DrawRect( 4*i+i*76, 228, 76+i*80, 271 , LCD_COLOR_TEST_BUTON ) ;
		LCD_DrawLine( 80*i+1, 227, 76+i*80, 227 , LCD_COLOR_TEST_LINE );
		//if(i<5)
		LCD_DrawLine( 76+i*80, 227, 76+i*80, 271 , LCD_COLOR_TEST_LINE );
		LCD_DrawLine( 76+i*80+1, 227, 76+i*80+1, 271 , Black );
		LCD_DrawLine( 76+i*80+2, 227, 76+i*80+2, 271 , Black );
		LCD_DrawLine( 76+i*80+3, 227, 76+i*80+3, 271 , Black );
		
	}

}//118
void Disp_button_Num_time(void)
	
{
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(84, 271-30, " mΩ  ",  0);
	WriteString_16(84+80, 271-30, " Ω ",  0);


}
void Disp_button_Num_Input(vu8 page)
	
{
	vu8 i;
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;//
	if(page==0)
	{
		for(i=0;i<5;i++)
		WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Set_Unit[i],  0);
	}
	else
	{
		for(i=0;i<5;i++)
		WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Set_Unit[i+5],  0);
	}
	
//	WriteString_16(84+80, 271-30, "sec",  0);


}
void Disp_button_Num_Freq(void)
	
{
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(84, 271-30, " 确认 ",  0);
	//WriteString_16(84+80, 271-30, " Ω ",  0);


}
void Disp_button_Num_Avg(void)
	
{
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(84, 271-30, "*1",  0);
	//WriteString_16(84+80, 271-30, "KHz",  0);


}
void Disp_Button_value1(uint32_t value)
{
	Disp_Fastbutton();
	if(value==0)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84, 271-40, "测量",  0);
		WriteString_16(84, 271-20, "显示",  0);
		WriteString_16(84+80, 271-40, "测量",  0);
		WriteString_16(84+80, 271-20, "设置",  0);
		WriteString_16(84+80+80, 271-40, "系统",  0);
		WriteString_16(84+80+80, 271-20, "设置",  0);
		WriteString_16(84+80+80+80, 271-40, "系统",  0);
		WriteString_16(84+80+80+80, 271-20, "信息",  0);
//		WriteString_16(84+80+80+80+80, 271-40, "更多",  0);
//		WriteString_16(84+80+80+80+80, 271-20, " 1/2",  0);
	}
    else if(value==1)
    {
        Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84, 271-40, "文件",  0);
		WriteString_16(84, 271-20, "管理",  0);
		WriteString_16(84+80, 271-40, "保存",  0);
		WriteString_16(84+80, 271-20, "数据",  0);
        WriteString_16(84+80+80+80+80, 271-40, "更多",  0);
		WriteString_16(84+80+80+80+80, 271-20, " 2/2",  0);
    
    }

}
void Disp_Button_ItemScan_no(void)
{
	Disp_Fastbutton();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BUTON;
	WriteString_16(84, 271-40, "清除",  0);
	WriteString_16(84, 271-20, "表格",  0);
//				WriteString_16(84+80, 271-40, "自动",  0);
//				WriteString_16(84+80, 271-20, "复制",  0);
//        WriteString_16(84+80+80, 271-30, "工具",  0);
//		WriteString_16(84+80+80, 271-20, "设置",  0);
	WriteString_16(84+80+80+80, 271-30, "上一页",  0);
	WriteString_16(84+80+80+80+80, 271-30, "下一页",  0);
}
void Disp_Button_ItemScanSet(uint32_t value)
{
	Disp_Fastbutton();
 	if(value==0||value==1)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84, 271-30, "删除行",  0);
//		WriteString_16(84, 271-20, "设置",  0);
		WriteString_16(84+80, 271-40, "线性",  0);
		WriteString_16(84+80, 271-20, "自动",  0);
		WriteString_16(84+80+80, 271-40, "对数",  0);
		WriteString_16(84+80+80, 271-20, "自动",  0);
		WriteString_16(84+80+80+80, 271-30, "上一页",  0);
//		WriteString_16(84+80+80+80, 271-20, "设置",  0);
		WriteString_16(84+80+80+80+80, 271-30, "下一页",  0);
//		WriteString_16(84+80+80+80+80, 271-20, " 1/2",  0);
	}
    else 
//		if(value==2)
//		{
//			Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
//				WriteString_16(84, 271-40, "清除",  0);
//				WriteString_16(84, 271-20, "表格",  0);
////				WriteString_16(84+80, 271-40, "自动",  0);
////				WriteString_16(84+80, 271-20, "复制",  0);
//		//        WriteString_16(84+80+80, 271-30, "工具",  0);
//		//		WriteString_16(84+80+80, 271-20, "设置",  0);
//				WriteString_16(84+80+80+80, 271-30, "上一页",  0);
//				WriteString_16(84+80+80+80+80, 271-30, "下一页",  0);
//			
//		
//		}else
				
			{
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				WriteString_16(84, 271-30, "删除行",  0);
		//		WriteString_16(84, 271-20, "管理",  0);
				WriteString_16(84+80, 271-40, "自动",  0);
				WriteString_16(84+80, 271-20, "复制",  0);
		//        WriteString_16(84+80+80, 271-30, "工具",  0);
		//		WriteString_16(84+80+80, 271-20, "设置",  0);
				WriteString_16(84+80+80+80, 271-30, "上一页",  0);
				WriteString_16(84+80+80+80+80, 271-30, "下一页",  0);
			
			
			}


}
void Disp_Button_TestSet(uint32_t value)
{
	Disp_Fastbutton();
 	if(value==0)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84, 271-40, "测量",  0);
		WriteString_16(84, 271-20, "显示",  0);
		WriteString_16(84+80, 271-40, "测量",  0);
		WriteString_16(84+80, 271-20, "设置",  0);
		WriteString_16(84+80+80, 271-40, "系统",  0);
		WriteString_16(84+80+80, 271-20, "设置",  0);
		WriteString_16(84+80+80+80, 271-40, "系统",  0);
		WriteString_16(84+80+80+80, 271-20, "信息",  0);
//		WriteString_16(84+80+80+80+80, 271-40, "更多",  0);
//		WriteString_16(84+80+80+80+80, 271-20, " 1/2",  0);
	}
    else if(value==1)
    {
        Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84, 271-40, "文件",  0);
		WriteString_16(84, 271-20, "管理",  0);
		WriteString_16(84+80, 271-40, "系统",  0);
		WriteString_16(84+80, 271-20, "设置",  0);
        WriteString_16(84+80+80, 271-30, "工具",  0);
//		WriteString_16(84+80+80, 271-20, "设置",  0);
        WriteString_16(84+80+80+80+80, 271-40, "更多",  0);
		WriteString_16(84+80+80+80+80, 271-20, " 2/2",  0);
    
    
    }


}

void Disp_Button_SysSet(void)
{

		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84, 271-40, "测量",  0);
		WriteString_16(84, 271-20, "显示",  0);
		WriteString_16(84+80, 271-40, "测量",  0);
		WriteString_16(84+80, 271-20, "设置",  0);
		WriteString_16(84+80+80, 271-40, "系统",  0);
		WriteString_16(84+80+80, 271-20, "设置",  0);
		WriteString_16(84+80+80+80, 271-40, "系统",  0);
		WriteString_16(84+80+80+80, 271-20, "信息",  0);
//		WriteString_16(84+80+80+80+80, 271-40, "系统",  0);
//		WriteString_16(84+80+80+80+80, 271-20, "复位",  0);


}
void Disp_TopBar_Color(vu16 page)
{
	LCD_DrawRect( 0+page*120, 0, page*120+120, 22 , LCD_COLOR_TEST_BAR);
    LCD_DrawRect( 0, 271-18, 479, 271 , LCD_COLOR_TEST_BAR);


}
void Disp_MidRect(void)
{

	LCD_DrawRect( 0, 92,480 , 271-18 , LCD_COLOR_TEST_MID ) ;
   // LCD_DrawRect( 320, 92,480 , 271-18 , LCD_COLOR_BLACK ) ;
}
void Disp_TestScreen(void)
{
	Disp_TopBar_Color(0);
//	Disp_Fastbutton();//显示快速按键
	Disp_MidRect();//显示中间色块
}
//
//测量显示主菜单的项目显示
//
void Disp_Test_Item(void)
{
	uint32_t i;
	Disp_TestScreen();
	Colour.Fword=White;
	
    for(i=0;i<4;i++)
    {
        if(i)
            Colour.black=LCD_COLOR_TEST_BACK;
        else
            Colour.black=LCD_COLOR_TEST_BAR;
        WriteString_16(0+i*(120), 4, TFT5520_NAME[i][Tft_5520.Sys_Set.Lang],0);
    }
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<6;i++)
	{
		if(i<3)
			WriteString_16(0, 26+i*22, Test_Setitem[i][Tft_5520.Sys_Set.Lang],  0);
		else
			WriteString_16(250, 26+(i-3)*22, Test_Setitem[i][Tft_5520.Sys_Set.Lang],  0);
//		WriteString_16(0, 210, Test_Setitem[6],  0);
//		WriteString_16(120, 210, Test_Setitem[7],  0);
//		WriteString_16(250, 210, Test_Setitem[8],  0);
	
	}
//	Disp_Button_value1(0);
	

}
//档号显示子函数
void Disp_Range_MidRect(void)
{

	LCD_DrawRect( 0, 112,479 , 190 , LCD_COLOR_TEST_MID ) ;
}
void Disp_RangeScreen(void)
{
	Disp_TopBar_Color(0);
	Disp_Fastbutton();//显示快速按键
	Disp_Range_MidRect();//显示中间色块
}
//档号显示主菜单项目显示
void Disp_Range_Item(void)
{
	uint32_t i;
	Disp_RangeScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName[1],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<7;i++)
	{
		if(i<3)
			WriteString_16(0, 26+i*22, Range_Item[i],  0);
		else
			WriteString_16(250, 26+(i-3)*22, Range_Item[i],  0);
//		WriteString_16(0, 210, "R :",  0);
//		WriteString_16(120, 210, Test_Setitem[7],  0);
		
	
	}
	Disp_RangeDispvalue(SaveData.Main_Func.Param.test);
	
	Disp_Button_value1(0);


}
//档计数显示子函数

void Disp_Rang_Count_MidRect(void)
{

	LCD_DrawRect( 0, 86,479 , 160 , LCD_COLOR_TEST_MID ) ;
	LCD_DrawRect( 0, 193,479 , 227 , LCD_COLOR_TEST_MID ) ;
}

void Disp_Range_CountScreen(void)
{
	Disp_TopBar_Color(0);
	Disp_Fastbutton();//显示快速按键
	Disp_Rang_Count_MidRect();//显示中间色块
}
//档计数显示界面
void Disp_Range_Count_Item(void)
{
	uint32_t i;
	Disp_Range_CountScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName[2],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<3;i++)
	{

		WriteString_16(i*160, FIRSTLINE, Range_Count_Item[i],  0);

//		WriteString_16(0, 210, "R :",  0);
//		WriteString_16(120, 210, Test_Setitem[7],  0);//WriteString_12
		
	
	}
	WriteString_16(LIST1+60, FIRSTLINE, User_Freq[SaveData.Main_Func.Freq],  1);//显示参数
	Hex_Format(SaveData.Limit_Tab.Nom.Num , SaveData.Limit_Tab.Nom.Dot , 6 , 0);
	WriteString_16(LIST2-30, FIRSTLINE, DispBuf,  1);//显示标称值  后面要接着显示单位
	WriteString_16(LIST2+40, FIRSTLINE, Disp_Unit[SaveData.Limit_Tab.Nom.Unit],  1);
	Disp_Set_Unit_16(SaveData.Main_Func.Param.test,SELECT_2END-142,FIRSTLINE );//显示单位
//	WriteString_16(LIST2-30, FIRSTLINE, DispBuf,  1);
	
	WriteString_12(0,FIRSTLINE+20,"BIN",0);//Num_1
	WriteString_12(120,FIRSTLINE+20,"LOW [F]",0);
	WriteString_12(240,FIRSTLINE+20,"HIGH[F]",0);
	WriteString_12(400,FIRSTLINE+20,"COUNT",0);
	
	for(i=6;i<15;i++)
	{
		if((i-6>1&&i-6<7)||((i-6>8)))
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;

		
		if(SaveData.Limit_Tab.Comp_Value[i-6].low.Num!=0)//SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[i-6].low.Num ,
			SaveData.Limit_Tab.Comp_Value[i-6].low.Dot , 6 , 1);//加单位
			//SaveData.Limit_Tab.Comp_Value[Button_Page.index-16].high
			WriteString_12(LIST2-130, 58+(i-6)*15, DispBuf,  1);//显示标称值  后面要接着显示单位
		}
		else
		WriteString_12(LIST2-130, 58+(i-6)*15, "------",  1);	
	}
	Colour.black=LCD_COLOR_TEST_MID;
	if(SaveData.Limit_Tab.Comp_Value[8].high.Num!=0)
	{
		Hex_Format(SaveData.Limit_Tab.Comp_Value[8].high.Num, 
			SaveData.Limit_Tab.Comp_Value[8].high.Dot , 6 , 1);//加单位//加单位
		
		WriteString_12(LIST2-130, 196, DispBuf,  1);//显示标称值  后面要接着显示单位
	}
	else
	WriteString_12(LIST2-130, 196, "------",  1);
	
	for(i=16;i<25;i++)
	{
		if((i-16>1&&i-16<7)||(i-16>8))
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
			
		if(SaveData.Limit_Tab.Comp_Value[i-16].high.Num!=0)
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[i-16].high.Num, 
			SaveData.Limit_Tab.Comp_Value[i-16].high.Dot , 6 , 1);//加单位
			
			WriteString_12(LIST2, 58+(i-16)*15, DispBuf,  1);//显示标称值  后面要接着显示单位
		}
		else
			WriteString_12(LIST2, 58+(i-16)*15, "------",  1);
	}
	Colour.black=LCD_COLOR_TEST_MID;
	if(SaveData.Limit_Tab.Comp_Value[8].high.Num!=0)
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[8].low.Num, 
			SaveData.Limit_Tab.Comp_Value[8].low.Dot , 6 , 1);//加单位
		
		WriteString_12(LIST2, 196, DispBuf,  1);//显示标称值  后面要接着显示单位
		}
		else
			WriteString_12(LIST2, 196, "------",  1);
	
	
	for(i=1;i<10;i++)
	{
		if(i>2&&i<8)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
			
		WriteString_12(8,FIRSTLINE+17+15*i,Num_1[i-1],0);
		
	}
	Colour.black=LCD_COLOR_TEST_MID;
	WriteString_12(0,196,"2nd",0);
	WriteString_12(0,212,"AUX:",0);
	WriteString_12(240,212,"OUT:",0);
	
	
	
	Disp_Button_value1(0);


}
//列表显示子函数
void Disp_List_Count_MidRect(void)
{

	LCD_DrawRect( 0, 96,479 , 160 , LCD_COLOR_TEST_MID ) ;
//	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
}

void Disp_List_CountScreen(void)
{
	Disp_TopBar_Color(0);
	Disp_Fastbutton();//显示快速按键
	Disp_List_Count_MidRect();//显示中间色块
}
//列表显示界面
void Disp_List_Count_Item(void)
{
	Disp_List_CountScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName[3],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;

	WriteString_16(LIST1, FIRSTLINE, "方式 :",  0);
	WriteString_16(LIST1+40, FIRSTLINE+SPACE1, "频率[Hz]",  0);	
	WriteString_16(LIST1+140, FIRSTLINE+SPACE1, "电平[V]",  0);	
	WriteString_16(LIST1+140+100, FIRSTLINE+SPACE1, "Cp[F]",  0);
	WriteString_16(LIST1+140+100+100, FIRSTLINE+SPACE1, "D[ ]",  0);
	WriteString_16(LIST1+440, FIRSTLINE+SPACE1, "CMP",  0);
	Disp_Button_value1(0);

}

//列表显示子函数
void Disp_TestSet_MidRect(void)
{

	LCD_DrawRect( 0, 90,479 , 154 , LCD_COLOR_TEST_MID ) ;
//	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
}

void Disp_TestSetScreen(vu8 page)
{
	Disp_TopBar_Color(page);
//	Disp_Fastbutton();//显示快速按键
	Disp_TestSet_MidRect();//显示中间色块
}
//列表显示界面	 
void Disp_Test_Set_Item(void)
{
	uint32_t i;
	Disp_TestSetScreen(1);
//	Colour.Fword=White;
//	Disp_TestScreen();
	Colour.Fword=White;
	
    for(i=0;i<4;i++)
    {
        if(i!=1)
            Colour.black=LCD_COLOR_TEST_BACK;
        else
            Colour.black=LCD_COLOR_TEST_BAR;
        WriteString_16(0+i*(120), 4, TFT5520_NAME[i][Tft_5520.Sys_Set.Lang],0);
    }
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<(sizeof(Set_testitem)/(sizeof(Set_testitem[0])));i++)
	if(i<sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)
	{
		if(i>2&&i<6)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;	
		
		WriteString_16(LIST1, FIRSTLINE+SPACE1*i, Set_testitem[i][Tft_5520.Sys_Set.Lang],  0);

	}
	else
	{
	  	if((i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)>2&&(i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)<6)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;

		WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2), Set_testitem[i][Tft_5520.Sys_Set.Lang],  0);
		
		
	}	
	//Disp_Button_TestSet(0);

}
//测试时档号显示界面的显示
void Disp_RangeDispValue(Button_Page_Typedef *Button_Page)
{
//	vu32 flag;
	vu32 i;
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;

	Colour.black=LCD_COLOR_TEST_BACK;	
	WriteString_16(LIST1+88, FIRSTLINE, User_FUNC[SaveData.Main_Func.Param.test],  1);//增加算法  把顺序改过来
	
//频率	
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1, User_Freq[SaveData.Main_Func.Freq],  1);
	
	
//电平	

	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*2, User_Level[SaveData.Main_Func.Level],  1);
//量程

//	if(SaveData.Main_Func.Range.Auto)
//		WriteString_16(LIST2+88, FIRSTLINE, User_Range[0],  0);
//	else
//	{
		WriteString_16(LIST2+88, FIRSTLINE, User_Range[SaveData.Main_Func.Range.Range],  0);
	
//	}
	
//速度
	
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, User_Speed[SaveData.Main_Func.Speed],  1);
	
//偏置

	WriteString_16(LIST2+88, FIRSTLINE+SPACE1, User_Comp[SaveData.Main_Func.Bias],  1);
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*3-2,SELECT_2END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1//SPACE1
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*3, User_Comp[SaveData.Limit_Tab.Comp],  1);
	Disp_Fastbutton();
	if(Button_Page->index==1)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		for(i=0;i<2;i++)
			WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, User_Comp[i],  0);
	}
	else
	
	Disp_Button_value1(0);
	

}
//测试时档号显示界面的显示
void Disp_Range_ComDispValue(Button_Page_Typedef *Button_Page)
{
//	vu32 flag;
	vu32 i;
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;

	Colour.black=LCD_COLOR_TEST_BACK;	
	
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawRect( 320+60, 26,320+110 , 26+16 , Colour.black ) ;//SPACE1//SPACE1
	WriteString_16(320+60, 26, User_Comp[SaveData.Limit_Tab.count],  1);
	Disp_Fastbutton();
	if(Button_Page->index==1)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		for(i=0;i<2;i++)
			WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, User_Comp[i],  0);
	}
	else
	
	Disp_Button_value1(0);
	

}

//测试时，显示列表扫描的测试值，也就是从串口接收过来的值
void Disp_Scan_Compvalue(vu8 set)
{
	vu8 i;
//	vu32  Black_Select;
//	static vu32 pos[2];
	
//	Black_Select=(SaveData.Limit_ScanValue.xpos)?1:0;//count
//	if(Black_Select)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//	
//	}
//	else
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	}
//	pos[0]
//	if(set)
//	{
//		Colour.black=LCD_COLOR_SELECT;
//		LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;
//	}
//	Colour.black=LCD_COLOR_TEST_BACK;
//	if(pos[0]!=SaveData.Limit_ScanValue.xpos||pos[1]!=SaveData.Limit_ScanValue.ypos)
//	{
//		LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;
//	
//	}
	for(i=1;i<11;i++)
	{
		
		if(i>2&&i<7)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
		
		if(SaveData.Limit_ScanValue.Range_Set_main[SaveData.Limit_ScanValue.num*10+(i-1)].Num==0xffffffff)
			WriteString_12(8+30+108+100,FIRSTLINE+SPACE1+3+ (i)*16, "----",  0);
		else
		{
			Hex_Format(SaveData.Limit_ScanValue.Range_Set_main[SaveData.Limit_ScanValue.num*10+(i-1)].Num ,
			 SaveData.Limit_ScanValue.Range_Set_main[SaveData.Limit_ScanValue.num*10+(i-1)].Dot , 5 , 1);
				WriteString_12(8+30+108+100,FIRSTLINE+SPACE1+3+ (i)*16, DispBuf,  0);
			
		}
		if(SaveData.Limit_ScanValue.Range_Sed_Second[SaveData.Limit_ScanValue.num*10+(i-1)].Num==0xffffffff)
			WriteString_12(8+30+108+100+100,FIRSTLINE+SPACE1+3+ (i)*16, "----",  0);
		else
		{
			
			Hex_Format(SaveData.Limit_ScanValue.Range_Sed_Second[SaveData.Limit_ScanValue.num*10+(i-1)].Num ,
				SaveData.Limit_ScanValue.Range_Sed_Second[SaveData.Limit_ScanValue.num*10+(i-1)].Dot , 5 , 1);
			WriteString_12(8+30+108+100+100,FIRSTLINE+SPACE1+3+ (i)*16, DispBuf,  0);
		
		}
		if(SaveData.Limit_ScanValue.comp[SaveData.Limit_ScanValue.num*10+(i-1)]==0xff)
			WriteString_12(8+30+108+100+100+100,FIRSTLINE+SPACE1+3+ (i)*16, "-",  0);
		else
		{
			
			if(SaveData.Limit_ScanValue.comp[SaveData.Limit_ScanValue.num*10+(i-1)])
				WriteString_12(8+30+108+100+100+100,FIRSTLINE+SPACE1+3+ (i)*16, "P",  0);
			else
				WriteString_12(8+30+108+100+100+100,FIRSTLINE+SPACE1+3+ (i)*16, "F",  0);
		
		}
		//for(i=1;i<11;i++)
		{
			
			if(i>2&&i<7)
				Colour.black=LCD_COLOR_TEST_MID;
			else
				Colour.black=LCD_COLOR_TEST_BACK;	
			if(SaveData.Limit_ScanValue.freq[SaveData.Limit_ScanValue.num*10+(i-1)]>30)
				WriteString_12(8+30,FIRSTLINE+SPACE1+3+ (i)*16, "----",  0);
			else
				{
					WriteString_12(8+30,FIRSTLINE+SPACE1+3+ (i)*16, User_Freq[SaveData.Limit_ScanValue.freq[SaveData.Limit_ScanValue.num*10+(i-1)]],  0);
				
				}
			if(SaveData.Limit_ScanValue.ac_leave[SaveData.Limit_ScanValue.num*10+(i-1)]>2)
				WriteString_12(8+30+108,FIRSTLINE+SPACE1+3+ (i)*16, "----",  0);
			else
				{
					WriteString_12(8+30+108,FIRSTLINE+SPACE1+3+ (i)*16, User_Level[SaveData.Limit_ScanValue.ac_leave[SaveData.Limit_ScanValue.num*10+(i-1)]],  0);
				
				}
				
			Hex_Format(SaveData.Limit_ScanValue.num*10+i , 0 , 3 , 1);
			
			WriteString_12(8,FIRSTLINE+SPACE1+3+ (i)*16, DispBuf,  0);
		}
				
		
	}

}
//const List_Scan[][6][2]=
//{
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{{},{},{},{},{}},
//	{},
//	{},
//	{},
//	{}


//};

//void Disp_List_Select(Button_Page_Typedef* Button_Page,vu8 i)
//{
//	vu8 Black_Select;
////	vu8 i;
////	for(i=0;i<10;i++)
//	{
//		Black_Select=(Button_Page->index==i)?1:0;
//		if(Black_Select)
//		{
//			Colour.black=LCD_COLOR_SELECT;
//		
//		}
//		else
//		{
//			Colour.black=LCD_COLOR_TEST_BACK;
//		}
//		//if()	
//		LCD_DrawRect( 32+Button_Page->page*60, FIRSTLINE+SPACE1+3+16*(Button_Page->index-1),88+Button_Page->page*60 ,
//			FIRSTLINE+SPACE1+3+16*Button_Page->index+16 , Colour.black ) ;
//		//LCD_DrawRect( LIST1+88, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
////		WriteString_16(LIST1+88, FIRSTLINE, User_FUNC[SaveData.Main_Func.Param.test],  1);//增加算法  把顺序改过来
//	}
//}
//void Disp_List_Select(Button_Page_Typedef* Button_Page)
//{
//	static vu32 pos[2];
//	if(Button_Page->index==0&&Button_Page->page==0)
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//	
//	}
//	else
//		if(Button_Page->index==1&&Button_Page->page==0)//
//		{
//			Colour.black=LCD_COLOR_SELECT;
//			LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;
//			Colour.black=LCD_COLOR_TEST_BACK;
////			pos[0]=Button_Page->index;
////			pos[1]=Button_Page->page;
//		
//		}
//		else
//		{
//			Colour.black=LCD_COLOR_SELECT;
//			LCD_DrawRect( 32+Button_Page->page*60, FIRSTLINE+SPACE1+3+16*(Button_Page->index-1),88+Button_Page->page*60 ,
//			FIRSTLINE+SPACE1+3+16*Button_Page->index+16 , Colour.black ) ;
//			
//			Colour.black=LCD_COLOR_TEST_BACK;
////			if(pos[0]==1&&pos[1]==0)
////			{
////				LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;
////			}
////			else
////			{
////			
////				LCD_DrawRect( 32+pos[1]*60, FIRSTLINE+SPACE1+3+16*pos[0],88+pos[1]*60 ,
////			FIRSTLINE+SPACE1+3+16*pos[0]+16 , Colour.black ) ;
////			}
////			pos[0]=Button_Page->index;
////			pos[1]=Button_Page->page;
//		
//		}



//}
//测试时，显示列表扫描的测试值，也就是从串口接收过来的值
void Disp_Scan_SetCompvalue(Button_Page_Typedef* Button_Page)
{
	vu8 i;
	if(Button_Page->index==2)
		Disp_Button_ItemScan_no();
	else
		if(Button_Page->index>2)
		Disp_Button_ItemScanSet(Button_Page->third);
	if(Button_Page->index==1&&Button_Page->third==0)//第一个值
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawRect( 54, FIRSTLINE,54+40 , FIRSTLINE+16 , Colour.black ) ;	
	WriteString_16(54,FIRSTLINE, User_FUNC[SaveData.Sys_Setup.Main_Func],  0);
	
	if(Button_Page->index==2&&Button_Page->third==0)//第二个值
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawRect( 0, FIRSTLINE+24,30 , FIRSTLINE+24+16 , Colour.black ) ;	
	WriteString_16(0,FIRSTLINE+24, "No.",  0);
	
	for(i=3;i<13;i++)
	{
		
		if(Button_Page->index==i&&Button_Page->third==2)//第三列
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect( 250, FIRSTLINE+SPACE1+3+ (i-2)*16,250+56 ,
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;	
		if(SaveData.Limit_ScanValue.Range_Set_main[Button_Page->force*10+(i-3)].Num==0xffffffff)
			WriteString_12(250,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
		else
		{
			Hex_Format(SaveData.Limit_ScanValue.Range_Set_main[Button_Page->force*10+(i-3)].Num ,
			 SaveData.Limit_ScanValue.Range_Set_main[Button_Page->force*10+(i-3)].Dot , 5 , 1);
				WriteString_12(250,FIRSTLINE+SPACE1+3+ (i-2)*16, DispBuf,  0);
			
		}
		if(Button_Page->index==i&&Button_Page->third==3)//第四列
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect( 250+80, FIRSTLINE+SPACE1+3+ (i-2)*16,250+80+56 ,
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;	
		if(SaveData.Limit_ScanValue.Range_Sed_Second[Button_Page->force*10+(i-3)].Num==0xffffffff)
			WriteString_12(250+80,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
		else
		{
			
			Hex_Format(SaveData.Limit_ScanValue.Range_Sed_Second[Button_Page->force*10+(i-3)].Num ,
				SaveData.Limit_ScanValue.Range_Sed_Second[Button_Page->force*10+(i-3)].Dot , 5 , 1);
			WriteString_12(250+80,FIRSTLINE+SPACE1+3+ (i-2)*16, DispBuf,  0);
		
		}
		if(Button_Page->index==i&&Button_Page->third==4)//第五列
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect( 420, FIRSTLINE+SPACE1+3+ (i-2)*16,479 ,
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;	
		if(SaveData.Limit_ScanValue.comp[Button_Page->force*10+(i-3)]==0xff)
			WriteString_12(420,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
		else
		{
			
			//if(SaveData.Limit_ScanValue.comp[SaveData.Limit_ScanValue.num*10+(i-2)])
				WriteString_12(420,FIRSTLINE+SPACE1+3+ (i-2)*16, "P",  0);
			//else
				//WriteString_12(8+30+108+100+100+100,FIRSTLINE+SPACE1+3+ (i-1)*16, "F",  0);
		
		}
		{
			
			if(Button_Page->index==i&&Button_Page->third==0)//第一列
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect(40, FIRSTLINE+SPACE1+3+ (i-2)*16,126 , 
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;
			if(SaveData.Limit_ScanValue.freq[Button_Page->force*10+(i-3)]>0x30)
				WriteString_12(40,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
			else
				{
					WriteString_12(40,FIRSTLINE+SPACE1+3+ (i-2)*16, 
					User_Freq[SaveData.Limit_ScanValue.freq[Button_Page->force*10+(i-3)]],  0);
				
				}
			if(Button_Page->index==i&&Button_Page->third==1)//第二列
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
			LCD_DrawRect( 38+88, FIRSTLINE+SPACE1+3+ (i-2)*16,38+88+70 , 
			FIRSTLINE+SPACE1+3+ (i-2)*16+16 , Colour.black ) ;	
			if(SaveData.Limit_ScanValue.ac_leave[Button_Page->force*10+(i-3)]>2)
				WriteString_12(8+30+88,FIRSTLINE+SPACE1+3+ (i-2)*16, "----",  0);
			else
				{
					WriteString_12(8+30+88,FIRSTLINE+SPACE1+3+ (i-2)*16, 
					User_Level[SaveData.Limit_ScanValue.ac_leave[Button_Page->force*10+(i-3)]],  0);
				
				}
			Colour.black=LCD_COLOR_TEST_BACK;	
			Colour.black=LCD_COLOR_TEST_BACK;	
			Hex_Format(Button_Page->force*10+i-2 , 0 , 3 , 1);
			
			WriteString_12(8,FIRSTLINE+SPACE1+3+ (i-2)*16, DispBuf,  0);
		}
				
		
	}
	



}
//测试时，列表扫描显示值
void Disp_LIMIT_ComDispValue(Button_Page_Typedef *Button_Page)
{
//	vu32 flag;
	vu32 i;
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;

	Colour.black=LCD_COLOR_TEST_BACK;	
	
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawRect( 48, FIRSTLINE,88 , FIRSTLINE+16 , Colour.black ) ;//SPACE1//SPACE1
	WriteString_16(48, FIRSTLINE, Limit_Scan_Fun[SaveData.Limit_ScanValue.fun],  1);
	
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawRect( 8, FIRSTLINE+SPACE1,30 , FIRSTLINE+SPACE1+16 , Colour.black ) ;//SPACE1//SPACE1
	WriteString_16(8, FIRSTLINE+SPACE1, "No.",  1);
	//Disp_Fastbutton();
	if(Button_Page->index==1)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		for(i=0;i<2;i++)
			WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+12, Limit_Scan_Fun[i],  0);
	}
	else if(Button_Page->index==2)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		for(i=0;i<2;i++)
			WriteString_16(BUTTOM_X_VALUE+(i+3)*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+12, Limit_Scan_Page[i],  0);
	}else
		Disp_Button_value1(Button_Page->page);
	

}
//测试的时候显示的设置值
void Disp_Test_value(Button_Page_Typedef* Button_Page)
{
//	vu32 flag;
//	vu32 i;
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;
		
	Black_Select=(Button_Page->index==1)?1:0;//组别
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawRect( LIST1+88, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
    PutChar( LIST1+88+24, FIRSTLINE, Tft_5520.Group+'1', Colour.Fword, Colour.black );
	//WriteString_16(LIST1+88, FIRSTLINE, Test_Tirpvalue[Save_Res.Set_Data.trip],  0);//增加算法  把顺序改过来
	
//电阻上限
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		

	Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].Hi_Limit , 0 , 4 , 0);
    LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1 ,SELECT_1END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1, DispBuf,  0);

		WriteString_16(LIST1+90+8*6, FIRSTLINE+SPACE1, "MΩ",  0);

//电阻下限
	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}

	
	Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].Lo_Limit , 0 , 4 , 0);
     LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*2 ,SELECT_1END , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*2, DispBuf,  0);
	
		WriteString_16(LIST1+90+8*6, FIRSTLINE+SPACE1*2, "MΩ",  0);
//量程
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
    if(GetSystemStatus()==SYS_STATUS_IDEM)
    {
		
	LCD_DrawRect( LIST2+88, FIRSTLINE-2,SELECT_2END , FIRSTLINE+SPACE1*1-4 , Colour.black ) ;//SPACE1

	WriteString_16(LIST2+88, FIRSTLINE, User_Range[Tft_5520.Set_Value[Tft_5520.Group].Range],  0);
	}
	
//输出电压
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}

	Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].voltage , 0 , 4 , 0);
    LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*1-2,SELECT_2END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;
	WriteString_16(LIST2+88+10, FIRSTLINE+SPACE1*1, DispBuf,  0);
	WriteString_16(LIST2+88+8*6, FIRSTLINE+SPACE1, " V",  0);
	
//判定时间

	Black_Select=(Button_Page->index==6)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}

	Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].Delay_Time , 1 , 4 , 0);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, DispBuf,  0);
	WriteString_16(LIST2+88+8*6, FIRSTLINE+SPACE1*2, " s",  0);


}
//显示设置参数的值Setup_Valueall
void DispSet_value(Button_Page_Typedef* Button_Page)
{
	vu32 i;
	vu32 Black_Select;
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)//组别
	{
		Colour.black=LCD_COLOR_SELECT;
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
		
	LCD_DrawRect( LIST1+88, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	 PutChar( LIST1+88, FIRSTLINE, Tft_5520.Group+'1', Colour.Fword, Colour.black );
	
//速度	
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1-2,SELECT_1END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1, Test_Speedvalue[Tft_5520.Set_Value[Tft_5520.Group].Speed][Tft_5520.Sys_Set.Lang],  0);
	
	
//电压设置
	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_MID;
	}
	
	LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*3-2,SELECT_1END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
    Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].voltage , 0 , 4 , 0);
	WriteString_16(LIST1+88+8, FIRSTLINE+SPACE1*3, DispBuf,  0);
    WriteString_16(LIST1+90+10*6, FIRSTLINE+SPACE1*3, "V",  0);
	
	
//测量时间
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_MID;
	}
	
	
	Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].Delay_Time , 1 , 4 , 0);
    
	
	LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*4-2,SELECT_1END , FIRSTLINE+SPACE1*5 -4, Colour.black ) ;
	WriteString_16(LIST1+88+10, FIRSTLINE+SPACE1*4, DispBuf,  0);

    WriteString_16(LIST1+90+10*6, FIRSTLINE+SPACE1*4, "s",  0);
		
	
//高端开路检测	
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*6-2,SELECT_1END , FIRSTLINE+SPACE1*7-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*6, Test_Compvalue[Tft_5520.Set_Value[Tft_5520.Group].Hi_Open][Tft_5520.Sys_Set.Lang],  0);
	//低端开路检测
	Black_Select=(Button_Page->index==6)?1:0;
	
	
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	LCD_DrawRect( LIST1+88, FIRSTLINE+SPACE1*7-2,SELECT_1END , FIRSTLINE+SPACE1*8 -4, Colour.black ) ;
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*7, Test_Compvalue[Tft_5520.Set_Value[Tft_5520.Group].Lo_Open][Tft_5520.Sys_Set.Lang],  0);
	
	

//量程
	Black_Select=(Button_Page->index==7)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
//	Save_Res.Set_Data.Range=1;	
	LCD_DrawRect( LIST2+88, FIRSTLINE-2,SELECT_2END , FIRSTLINE+SPACE1*1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2+88, FIRSTLINE, User_Range[Tft_5520.Set_Value[Tft_5520.Group].Range],  0);
	
//讯响
	Black_Select=(Button_Page->index==8)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1-2,SELECT_2END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1, Setup_Beep[Tft_5520.Set_Value[Tft_5520.Group].beep_Mode][Tft_5520.Sys_Set.Lang],  0);
	
	//电阻上限
	Black_Select=(Button_Page->index==9)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_MID;
	}
	
	
	Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].Hi_Limit , 0 , 4 , 0);
	
	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*3-2,SELECT_2END , FIRSTLINE+SPACE1*4 -4, Colour.black ) ;
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*3, DispBuf,  0);
	WriteString_16(LIST2+90+8*6, FIRSTLINE+SPACE1*3, "MΩ",  0);

		
	

//电阻下限
	
	Black_Select=(Button_Page->index==10)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_MID;
	}
	
	Hex_Format(Tft_5520.Set_Value[Tft_5520.Group].Lo_Limit , 0 , 4 , 0);
	
	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*4-2,SELECT_2END , FIRSTLINE+SPACE1*5 -4, Colour.black ) ;
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*4, DispBuf,  0);
	WriteString_16(LIST2+90+8*6, FIRSTLINE+SPACE1*4, "MΩ",  0);

	
//短路测试
	
	Black_Select=(Button_Page->index==11)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*6-2,SELECT_2END , FIRSTLINE+SPACE1*7 -4, Colour.black ) ;
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*6, Test_Compvalue[Tft_5520.Set_Value[Tft_5520.Group].Short][Tft_5520.Sys_Set.Lang],  0);
	
	
	
//电压上限
	

	
	
	//Disp_Fastbutton();
    LCD_DrawRect( 0, 271-18, 479, 271 , LCD_COLOR_TEST_BAR);
    Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	switch(Button_Page->index)
	{
		case 0:
				//Disp_Button_TestSet(0);
			break;
		case 1:
			
//				Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
//				for(i=0;i<2;i++)
				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE+13, "组别1-组别5",  0);
				
//				
			
			
		break;
		case 2:
////			Colour.Fword=White;
////			Colour.black=LCD_COLOR_TEST_BAR;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+13, Test_Speedvalue[i][Tft_5520.Sys_Set.Lang],  0);
			}
		break;
		case 3:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE+13, "25V-1000V",  0);
//			}
			break;
		case 4:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<5;i++)
//			{
//				
				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE+13,"0S-999.9S",  0);
//			}
			break;
		case 5:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_SELECT;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+13, Test_Compvalue[i][Tft_5520.Sys_Set.Lang],  0);
			}
			break;
		case 6:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_SELECT;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+13, Test_Compvalue[i][Tft_5520.Sys_Set.Lang],  0);
			}
			break;
		case 7:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_SELECT;
			for(i=0;i<5;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+13, User_Range[i],  0);
			}
			break;
		case 8:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_SELECT;
			for(i=0;i<3;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+13, Setup_Beep[i][Tft_5520.Sys_Set.Lang],  0);
			}
			break;
		case 9:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<5;i++)
//			{
//				
				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE+13, "0-9999M",  0);
//			}
			break;
		case 10:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<5;i++)
//			{
//				
				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE+13, "0-9999M",  0);
//			}
			break;
		case 11:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_SELECT;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+13, Test_Compvalue[i][Tft_5520.Sys_Set.Lang],  0);
			}
			break;
		case 12:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<5;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, LevleButton_Tip[i],  0);
//			}
			break;
		
			
		default:
			break;
		
			
	
	
	}

}

void Disp_LimitSEt_value(Button_Page_Typedef* Button_Page)
{
//	vu32 flag;
	vu32 i;
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;
//	if(SaveData.Main_Func.V_i==1)
//	{
//		Colour.black=LCD_COLOR_TEST_BACK;
//		WriteString_16(30, 210, User_Comp[1],  1);
//		WriteString_16(30+120, 210, User_Comp[1],  1);
//		
//	}
//	
	Colour.black=LCD_COLOR_TEST_BACK;

	if(SaveData.Limit_Tab.Mode==0)//上下限
	{
		Disp_Set_Unit_12(SaveData.Main_Func.Param.test,320+40,65);
		Disp_Set_Unit_12(SaveData.Main_Func.Param.test,160+40,65);
		
	}
	else
	{
		WriteString_12(320+40, 65, "%",  0);	
		WriteString_12(160+40, 65, "%",  0);
		
	}
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+70, FIRSTLINE-2,SELECT_1END-80 , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+70, FIRSTLINE, User_FUNC[SaveData.Main_Func.Param.test],  1);//增加算法  把顺序改过来
	
//标称
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	//WriteString_16(LIST1+60, FIRSTLINE, User_Freq[SaveData.Main_Func.Freq],  1);//显示参数
	Hex_Format(SaveData.Limit_Tab.Nom.Num , SaveData.Limit_Tab.Nom.Dot , 5 , 0);//加单位
	LCD_DrawRect( LIST2-24, FIRSTLINE,SELECT_2END-150 , FIRSTLINE+16, Colour.black ) ;
	WriteString_16(LIST2-24, FIRSTLINE, DispBuf,  1);//显示标称值  后面要接着显示单位
	WriteString_16(LIST2+34, FIRSTLINE, Disp_Unit[SaveData.Limit_Tab.Nom.Unit],  1);
	
	
//方式
	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2+134, FIRSTLINE-2,SELECT_2END+10 , FIRSTLINE+SPACE1*1 -4, Colour.black ) ;//SPACE1
	WriteString_16(LIST2+134, FIRSTLINE, User_LimitList_Value[SaveData.Limit_Tab.Mode],  1);
//附属
	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+70, FIRSTLINE+SPACE1-2,SELECT_1END-80 , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
//	if(SaveData.Main_Func.Range.Auto)
		WriteString_16(LIST1+70, FIRSTLINE+SPACE1, Switch_Value[SaveData.Limit_Tab.Param],  0);
//	else
//	{
//		WriteString_16(LIST1+88, FIRSTLINE, User_Range[SaveData.Main_Func.Range.Range+1],  0);
//	
//	}
	
//比较
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST2, FIRSTLINE+SPACE1-2,SELECT_2END-100 , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST2, FIRSTLINE+SPACE1, Switch_Value[SaveData.Limit_Tab.Comp],  1);
	for(i=6;i<16;i++)
	{
		//SaveData.Limit_Tab.Comp_Value[Button_Page.index-6].low
		if(i==Button_Page->index)
			Colour.black=LCD_COLOR_SELECT;
		else
			if(i-6>2&&i-6<7)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
		LCD_DrawRect( LIST2-90-8, 76+(i-6)*15,LIST2-30 , 76+(i-6)*15+15 , Colour.black ) ;
		if(SaveData.Limit_Tab.Comp_Value[i-6].low.Num!=0)
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[i-6].low.Num , 
			SaveData.Limit_Tab.Comp_Value[i-6].low.Dot , 5 , 0);//加单位
			
			WriteString_12(LIST2-90-8, 76+(i-6)*15, "-",  1);
			WriteString_12(LIST2-90, 76+(i-6)*15, DispBuf,  1);//显示标称值  后面要接着显示单位
			if(SaveData.Limit_Tab.Mode==0)
			WriteString_12(LIST2-42, 76+(i-6)*15, 
			Disp_Unit[SaveData.Limit_Tab.Comp_Value[i-6].low.Unit],  1);
			//WriteString_16(LIST1+70, FIRSTLINE+SPACE1, Switch_Value[SaveData.Limit_Tab.Param],  0);
			//WriteString_16(LIST2+34, FIRSTLINE, Disp_Unit[SaveData.Limit_Tab.Nom.Unit],  1);
		}
		else
		WriteString_12(LIST2-90, 76+(i-6)*15, "------",  1);	
	}
	for(i=16;i<26;i++)
	{
		
		if(i==Button_Page->index)
			Colour.black=LCD_COLOR_SELECT;
		else
			if(i-16>2&&i-16<7)
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
			
		LCD_DrawRect( LIST2+70, 76+(i-16)*15,LIST2+70+60 , 80+(i-16)*15+15, Colour.black ) ;
		if(SaveData.Limit_Tab.Comp_Value[i-16].high.Num !=0)
		{
			Hex_Format(SaveData.Limit_Tab.Comp_Value[i-16].high.Num ,
				SaveData.Limit_Tab.Comp_Value[i-16].high.Dot , 5 , 0);//加单位
			
			WriteString_12(LIST2+70, 76+(i-16)*15, DispBuf,  0);//显示标称值  后面要接着显示单位
			if(SaveData.Limit_Tab.Mode==0)
			WriteString_12(LIST2+70+48, 76+(i-16)*15, 
			Disp_Unit[SaveData.Limit_Tab.Comp_Value[i-16].high.Unit],  1);
		}
		else
			WriteString_12(LIST2+70, 76+(i-16)*15, "------",  1);
	}

		
//	LCD_DrawRect( LIST2+88, FIRSTLINE+SPACE1*2-2,SELECT_2END , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1
//	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, User_Comp[SaveData.Main_Func.Bias],  1);
	Disp_Fastbutton();
	
	switch(Button_Page->index)
	{
		case 0:
				Disp_Button_TestSet(Button_Page->page);
			break;
		case 1:

		break;
		case 2:

		break;
		case 3:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, User_LimitList_Value[i],  0);
			}
			break;

		case 4:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Switch_Value[i],  0);
			}
			break;
		case 5:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Switch_Value[i],  0);
			}
			break;
//		case 6:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, User_Comp[i],  0);
//			}
//			break;
		default:
			WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE, "清除行",  0);
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE-10, "清除",  0);
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+10, "表格",  0);
		break;
		
			

		
			
	
	
	}

}
const vu8 Debug_Dot[DEBUG_RANGE]={0,3,2,1,0};
void Disp_Debug_Reference(Button_Page_Typedef* Button_Page)
{
	vu8 i;
    vu16 value=0;;
    for(i=0;i<6;i++)
    DispBuf[i]=0; 
    for(i=0;i<4;i++)
    {
        DispBuf[i]=Send_Testvalue[0].dianya[i];
        if(DispBuf[i]>'0')
            DispBuf[i]-='0';
        else
            DispBuf[i]=0;
    
    }
    for(i=0;i<4;i++)
    {
        value*=10;
        value+=DispBuf[i];
        
        
        //DispBuf[i]=Send_Testvalue[0].dianya[i];//接收数据
        
    }
//	if(Button_Page->index)//电阻
//	{
		
		//V_BCD_Int(eee);
        
        Hex_Format(value, Debug_Dot[i] , 4 , 0);
		WriteString_16(LIST1+160+160, FIRSTLINE+SPACE1*(Button_Page->index), DispBuf,  1);
		
//	}
//		else//电压
//		{
//			
//			//BCD_Int(ddd);
//			WriteString_16(LIST1+160+160, FIRSTLINE+SPACE1*(Button_Page->index), DispBuf,  1);
//			
//		}


}

//显示设置参数的值Setup_Valueall
void Disp_Debug_value(Button_Page_Typedef* Button_Page)
{
//	vu32 flag;
	vu32 i;
//	vu32 xpose;
//	vu32 Black_Select;
//	vu32 Select_color;
	for(i=0;i<DEBUG_RANGE;i++)
	{
		if(Button_Page->index==(i+1))
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
	
//        if(Save_Res.Debug_Value[i].standard>Debug_Comp[i][1]||Save_Res.Debug_Value[i].standard<Debug_Comp[i][0])
//            Save_Res.Debug_Value[i].standard=Debug_Comp1[i];
		LCD_DrawRect( LIST1+160, FIRSTLINE+SPACE1*(i+1)-2,SELECT_1END+40 , FIRSTLINE+SPACE1*(i+2)-4 , Colour.black ) ;//SPACE1
		Hex_Format(Tft_5520.Debugvalue[i], Debug_Dot[i] , 4 , 0);//sprintf((char *)DispBuf,"%4",Save_Res;
		
//        for(i=0;i<6;i++)
//            DispBuf[i]=0;
//        for(i=0;i<4;i++)
//        {
//            DispBuf[i]=Send_Testvalue[0].dianya[i]+'0';
//            
//        }
        WriteString_16(LIST1+160, FIRSTLINE+SPACE1*(i+1), DispBuf,  1);
		
//		LCD_DrawRect( LIST1+160+160, FIRSTLINE+SPACE1*(i+1)-2,SELECT_1END+40+160 , FIRSTLINE+SPACE1*(i+2)-4 , Colour.black ) ;//SPACE1
		//Hex_Format(Save_Res.Debug_Value[i].ad_value, Debug_Dot[i] , 4 , 0);//sprintf((char *)DispBuf,"%4",Save_Res;
		
		
			
	}
	Colour.black=LCD_COLOR_TEST_BACK;
	


}
//void Disp_Debug_valueset(Button_Page_Typedef* Button_Page)
//{
////	vu32 flag;
//	vu32 i;
////	vu32 xpose;
//	vu32 Black_Select;
////	vu32 Select_color;
//	for(i=0;i<DEBUG_RANGE;i++)
//	{
//		if(Button_Page->index==(i+1))
//		{
//			Colour.black=LCD_COLOR_SELECT;
//		
//		}
//		else
//		{
//			Colour.black=LCD_COLOR_TEST_BACK;
//		}
////	
////        if(Save_Res.Debug_Value[i].standard>Debug_Comp[1][i]||Save_Res.Debug_Value[i].standard<Debug_Comp[0][i])
////            Save_Res.Debug_Value[i].standard=Debug_Comp1[i];
////		LCD_DrawRect( LIST1+160, FIRSTLINE+SPACE1*(i+1)-2,SELECT_1END+40 , FIRSTLINE+SPACE1*(i+2)-4 , Colour.black ) ;//SPACE1
////		Hex_Format(Save_Res.Debug_Value[i].standard, Debug_Dot[i] , 4 , 0);//sprintf((char *)DispBuf,"%4",Save_Res;
//		WriteString_16(LIST1, FIRSTLINE+SPACE1*(i+1), User_Check_Item[i],  1);
//		
////		LCD_DrawRect( LIST1+160+160, FIRSTLINE+SPACE1*(i+1)-2,SELECT_1END+40+160 , FIRSTLINE+SPACE1*(i+2)-4 , Colour.black ) ;//SPACE1
//		//Hex_Format(Save_Res.Debug_Value[i].ad_value, Debug_Dot[i] , 4 , 0);//sprintf((char *)DispBuf,"%4",Save_Res;
//		
//		
//			
//	}
//	Colour.black=LCD_COLOR_TEST_BACK;
//	


//}
//void Disp_Sys(void)
//{
//	vu8 i;
//	Disp_Button_SysSet();
//	for(i=0;i<sizeof(Sys_Disp_Main);i++)
//	{
//		WriteString_16(LIST1+90, FIRSTLINE+SPACE1*i+2, Sys_Disp_Main[i],  0);
//	}


//}
//显示设置参数的值Setup_Valueall
void Disp_Sys_value(Button_Page_Typedef* Button_Page)
{
//	vu32 flag;
	vu32 i;
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;
//串口
	Black_Select=(Button_Page->index==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+100, FIRSTLINE,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+100, FIRSTLINE+2, Test_Compvalue[Tft_5520.Sys_Set.Uart][Tft_5520.Sys_Set.Lang],  0);//
	
//波特率
	Black_Select=(Button_Page->index==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+100, FIRSTLINE+SPACE1,SELECT_1END , FIRSTLINE+SPACE1*2-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+100, FIRSTLINE+SPACE1+2, Sys_Buard_Value[Tft_5520.Sys_Set.Buad],  0);
	
	//U盘
	Black_Select=(Button_Page->index==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
//IO口	
	LCD_DrawRect( LIST1+100, FIRSTLINE+SPACE1*2,SELECT_1END , FIRSTLINE+SPACE1*3-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+100, FIRSTLINE+SPACE1*2+2, Test_Compvalue[Tft_5520.Sys_Set.U_Switch][Tft_5520.Sys_Set.Lang],  0);

	Black_Select=(Button_Page->index==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+100, FIRSTLINE+SPACE1*3,SELECT_1END , FIRSTLINE+SPACE1*4-4 , Colour.black ) ;//SPACE1
	WriteString_16(LIST1+100, FIRSTLINE+SPACE1*3+2, Test_Compvalue[Tft_5520.Sys_Set.I_O][Tft_5520.Sys_Set.Lang],  0);

	
//显示语言
	Black_Select=(Button_Page->index==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+100, FIRSTLINE+SPACE1*4,SELECT_1END , FIRSTLINE+SPACE1*5 -4, Colour.black ) ;//SPACE1
	WriteString_16(LIST1+100, FIRSTLINE+SPACE1*4+2, Sys_Language_Value[Tft_5520.Sys_Set.Lang],  0);

//通讯地址
	Black_Select=(Button_Page->index==6)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawRect( LIST1+100, FIRSTLINE+SPACE1*5,SELECT_1END , FIRSTLINE+SPACE1*6 -4, Colour.black ) ;//SPACE1
	Hex_Format(Tft_5520.Sys_Set.Addr,0,3,0);
	WriteString_16(LIST1+100, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	
    LCD_DrawRect( 0, 271-18, 479, 271 , LCD_COLOR_TEST_BAR);
    Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	switch(Button_Page->index)
	{
		case 0:
//				Disp_Button_SysSet();
			break;
		case 1:
//			if(Button_Page->page==0)
//			{
//				Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
//				for(i=0;i<4;i++)
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Button_TipPage1[i],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE-10, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE+10, " 1/3",  0);
////				
//			}
//			else if(Button_Page->page==1)
//			{
//				Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
				for(i=0;i<2;i++)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+12, Test_Compvalue[i][Tft_5520.Sys_Set.Lang],  0);

//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE-10, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE+10, " 2/3",  0);
				
			
			
//			}
//			else
//			{
//				Colour.Fword=White;
//				Colour.black=LCD_COLOR_TEST_BUTON;
//				WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE,Button_TipPage3[0],  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE-10, "MORE",  0);
//				WriteString_16(BUTTOM_X_VALUE+BUTTOM_MID_VALUE*4, BUTTOM_Y_VALUE+10, " 3/3",  0);
//			
//			
//			}
		break;
		case 2:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<5;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+12, Sys_Buard_Value[i],  0);
			}
		break;
		case 3:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+12, Test_Compvalue[i][Tft_5520.Sys_Set.Lang],  0);
			}
			break;
		case 4:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+12, Test_Compvalue[i][Tft_5520.Sys_Set.Lang],  0);
			}
			break;
		case 5:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+12, Sys_Language_Value[i],  0);
			}
			break;
		case 6:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
			WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE+13, "1-255",  0);
			break;
		case 7:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE+12, BiasButton_Tip[i],  0);
			}
			break;
		case 8:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 9:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 10:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
		case 11:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Sys_Bais_Value[i],  0);
			}
			break;
//		case 12:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<5;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Sys_Addr_value[i],  0);
//			}
//			break;
//		case 13:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 14://
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
//			}
//			break;
//		case 15:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 16:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 17:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 18:
//			break;
//		case 19:
//			break;
//		case 20:
//			break;
		default:
			break;
		
			
	
	
	}

}
//用户校正界面子函数
void Disp_Usercheck_Screen(void)
{
	Disp_TopBar_Color(0);
	//Disp_Fastbutton();//显示快速按键
//	Disp_TestSet_MidRect();//显示中间色块
}
void Disp_UserCheck_Item(void)
{
	uint32_t i;
	Disp_Usercheck_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName[5],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;//User_Check_main
	
	for(i=0;i<(sizeof(User_Check_main)/(sizeof(User_Check_main[0])));i++)
	{
		WriteString_16(LIST1+160*i, FIRSTLINE, User_Check_main[i],  0);

	}
	
	for(i=0;i<(sizeof(User_Check_Item)/(sizeof(User_Check_Item[0])));i++)
	{
		WriteString_16(LIST1, FIRSTLINE+SPACE1*(i+1), User_Check_Item[i],  0);

	}
//	else
//	{
//		WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-sizeof(User_Check_Item)/(sizeof(User_Check_Item[0]))/2), User_Check_Item[i],  0);
//	}	
//	Disp_Button_TestSet(0);
}
//极限列表设置子函数
void Disp_LimitList_MidRect(void)
{

	LCD_DrawRect( 0, 90+32,479 , 150+32 , LCD_COLOR_TEST_MID ) ;
//	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
}
void Disp_LimitList_Screen(void)
{
	Disp_TopBar_Color(0);
	Disp_Fastbutton();//显示快速按键
	Disp_LimitList_MidRect();//显示中间色块	 User_LimitList_Item
}
void Disp_LimitList_Item(void)
{
	uint32_t i;
	Disp_LimitList_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName[6],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<(sizeof(User_LimitList_Item)/(sizeof(User_LimitList_Item[0])));i++)
	if(i<sizeof(User_LimitList_Item)/(sizeof(User_LimitList_Item[0]))/2)
	{
		WriteString_16(LIST1+i*160, FIRSTLINE, User_LimitList_Item[i],  0);

	}
	else
	{
		WriteString_16(LIST1+(i-sizeof(User_LimitList_Item)/(sizeof(User_LimitList_Item[0]))/2)*160,
		 FIRSTLINE+SPACE1, User_LimitList_Item[i],  0);
	}
	for(i=0;i<11;i++)			//
	{
		if(i>3&&i<8)
		 	Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
		WriteString_12(LIST1, 65+i*15, User_LimitList_Item2[i],  0);
	}
	WriteString_12(160, 65,"LOW",  0);
	
		
	WriteString_12(320, 65,"HIGH",  0);	
	
	Disp_Set_Unit_16(SaveData.Main_Func.Param.test,SELECT_2END-148,FIRSTLINE );//显示单位
	
	
	//Disp_Set_Unit_12(SaveData.Main_Func.Param.test,LIST2-154,65);
	Disp_Button_TestSet(0);

}
//列表模式选择的子函数
void Disp_ListScan_MidRect(void)
{

	LCD_DrawRect( 0, 90+32,479 , 150+32 , LCD_COLOR_TEST_MID ) ;
//	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
}
void Disp_ListScan_Screen(void)
{
	Disp_TopBar_Color(0);
	Disp_Fastbutton();//显示快速按键	  
//	Disp_ListScan_MidRect();//显示中间色块	 User_LimitList_Item
}
void Disp_ListScan_Item(void)
{
	uint32_t i;
	Disp_ListScan_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName[7],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST1, FIRSTLINE, User_ListScan_Item[0],  0);
	for(i=1;i<(sizeof(User_ListScan_Item)/(sizeof(User_ListScan_Item[0])));i++)
		WriteString_16(LIST1+30+(i-1)*77, FIRSTLINE+SPACE1, User_ListScan_Item[i],  0);
//	for(i=0;i<11;i++)
//	WriteString_16(LIST1, FIRSTLINE+SPACE1+i*16, User_LimitScan_Item2[i],0);
	Disp_Button_TestSet(0);
}
//系统设置子函数
void Disp_SysLine(void)
{
 	uint32_t i;
	for(i=0;i<5;i++)
	{
		//if(i<=13/2)
			LCD_DrawLine( 90, FIRSTLINE+(i+1)*SPACE1-2, 90+100,FIRSTLINE+(i+1)*SPACE1-2, White );
//		else
//			LCD_DrawLine( 90+250, FIRSTLINE+(i-13/2+1)*SPACE1-2, 90+100+250,FIRSTLINE+(i-13/2+1)*SPACE1-2, White );
	}

}
//void Disp_Sys_MidRect(void)
//{
//
//	LCD_DrawRect( 0, 90+32,479 , 150+32 , LCD_COLOR_TEST_MID ) ;
////	LCD_DrawRect( 0, 195,479 , 227 , LCD_COLOR_TEST_MID ) ;
//}
void Disp_Sys_Screen(void)
{
	Disp_TopBar_Color(2);
//	Disp_Fastbutton();//显示快速按键	  
//	Disp_ListScan_MidRect();//显示中间色块	 User_LimitList_Item  
}
void Disp_Sys(void)
{
	uint32_t i;
	//Disp_Sys_Screen();
//	Colour.Fword=White;
//	Colour.black=LCD_COLOR_TEST_BAR;
//	WriteString_16(0, 4, All_TopName[10],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
//	WriteString_16(LIST1, FIRSTLINE, User_ListScan_Item[0],  0);

	for(i=0;i<(sizeof(Sys_Sys)/(sizeof(Sys_Sys[0])));i++)
	//if(i<=sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2)
	{
		WriteString_16(LIST1, FIRSTLINE+SPACE1*i, Sys_Sys[i][Tft_5520.Sys_Set.Lang],  0);

	}
//	if(Save_Res.Production_Data.flag==0x55)
//	{
//		WriteString_16(LIST1+90, FIRSTLINE+SPACE1*3, Save_Res.Production_Data.buff,  0);
//		WriteString_16(LIST1+90, FIRSTLINE+SPACE1*4, Save_Res.Number.buff,  0);
//		
//	
//	
//	}else
//	{
//		WriteString_16(LIST1+90, FIRSTLINE+SPACE1*3, "2017.12",  0);
//		WriteString_16(LIST1+90, FIRSTLINE+SPACE1*4, "0000000",  0);
//	
//	
//	}
	//Main_Read ();
//	else
//	{
//		WriteString_16(LIST2,FIRSTLINE+SPACE1*(i-sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2), Sys_Setitem[i],  0);
//	}
//	Disp_SysLine();
//	Disp_Button_SysSet();
}
void Disp_Sys_Item(uint8_t page)
{
	uint32_t i;
//	Disp_Sys_Screen();
    Disp_TopBar_Color(page);
	Colour.Fword=White;
//	Disp_TestSetScreen(2);
//	Colour.Fword=White;
//	Disp_TestScreen();
	//Colour.Fword=White;
	
    for(i=0;i<4;i++)
    {
        if(i!=page)
            Colour.black=LCD_COLOR_TEST_BACK;
        else
            Colour.black=LCD_COLOR_TEST_BAR;
        WriteString_16(0+i*(120), 4, TFT5520_NAME[i][Tft_5520.Sys_Set.Lang],0);
    }
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
//	WriteString_16(LIST1, FIRSTLINE, User_ListScan_Item[0],  0);
    if(page==2)
	for(i=0;i<(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0])));i++)
	//if(i<=sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2)
	{
		WriteString_16(LIST1, FIRSTLINE+SPACE1*i, Sys_Setitem[i][Tft_5520.Sys_Set.Lang],  0);

	}
//	else
//	{
//		WriteString_16(LIST2,FIRSTLINE+SPACE1*(i-sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2), Sys_Setitem[i],  0);
//	}
//	Disp_SysLine();
//	Disp_Button_SysSet();
}
Sort_TypeDef Time_Set_Cov(Sort_TypeDef *Time)
{
	u32 value;
	if(Time->Num>9999)
	{
		Time->Num/=10;
		Time->Dot=4;
		
	}
	Time->Dot=4-Time->Dot;
	value=Time->Num;
	if(Time->Unit==0)
		value*=1000;
	else if(Time->Unit==1)
		value*=1e6;
	value/=pow(10,Time->Dot);
	if(value>30e6)
	{
		value=30e6;
		Time->Num=3000;
		Time->Dot=2;
		Time->Unit=1;
		
	}else if(value>=10e6)
	{
		Time->Num=value/10000;
		Time->Dot=2;
		Time->Unit=1;
		
	
	
	
	}else if(value>=1e6)
	{
		Time->Num=value/1000;
		Time->Dot=3;
		Time->Unit=1;
	
	
	}else if(value>=1e5)
	{
		Time->Num=value/100;
		Time->Dot=1;
		Time->Unit=0;
	
	
	}
	else if(value>=1e4)
	{
		Time->Num=value/10;
		Time->Dot=2;
		Time->Unit=0;
	
	}else if(value>=1e3)
	{
		Time->Num=value;
		Time->Dot=3;
		Time->Unit=0;
	
	
	}else 
	{
		Time->Num=value;
		Time->Dot=3;
		Time->Unit=0;
	
	}
	Time->mid_data=	value;
	
	

	return *Time;
}
int16_t Debug_Value(Sort_TypeDef *Debug_value)
{
	return Debug_value->Num;


}
Sort_TypeDef Freq_Set_Cov(Sort_TypeDef *Freq)
{
//	vu8 i;
	if(Freq->Unit==0)//单位是Hz
	{
		switch(Freq->Dot)
		{
			case 0:
				if(Freq->Num>1e5)
				{
					Freq->Num=1e5;
					Freq->Updata_flag=0;
				
				}
				else
					Freq->Updata_flag=1;
					
				break;
			case 1://1个小数点 
				break;
			case 2://两个小数点
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=10&&Freq->Num<100)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				Freq->Num/=1000;
				
				break;
			case 3://三个小数点
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=100&&Freq->Num<1000)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				Freq->Num/=100;
				break;
			case 4://4个小数点
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=1e3&&Freq->Num<1e4)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				Freq->Num/=10;
				break;
			default:
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=1e4&&Freq->Num<1e5)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				break;
		
		
		
		}
		
		
		
		
	
	}
	else//单位是KHZ
	{
		switch(Freq->Dot)
		{
			case 0:
				if(Freq->Num>100000)
				Freq->Num=100000;
				else
					Freq->Num*=1000;
					
				break;
			case 1:
				if(Freq->Num>0)
					Freq->Num/=10;
//				if(Freq->Num>=10000)
				
//				for(i=0;i<5;i++)
//				{
//					if((Freq->Num>=10&&Freq->Num<100)||Freq->Num<10)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
				//Freq->Num*=100;
				break;
			case 2:
				
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=10&&Freq->Num<100)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
//				Freq->Num*=1000;
				break;
			case 3:
				if(Freq->Num>10000)
					Freq->Num=10000;
				Freq->Num*=10;
//				for(i=0;i<5;i++)
//				{
//					if(Freq->Num>=10&&Freq->Num<100)
//						break;
//					else
//						Freq->Num/=10;
//				
//				
//				}
//				Freq->Num*=1e4;
				break;
			case 4:
				if(Freq->Num>1000)
					Freq->Num=1000;
				Freq->Num*=100;
				
				break;
			case 5:
			if(Freq->Num>100)
				Freq->Num=100;
			Freq->Num*=1000;
			
			break;
			default:
				Freq->Num=1e6;
				break;
		
		}
	
	
	}
	
	
	return *Freq;
}
Sort_TypeDef Input_Set_Cov(Sort_TypeDef *Input_Ref)//
{
//	vu8 i;
//	vu32 buff[]={10000,1000,100,10,1};
	vu32 value;
	if(Input_Ref->Num>9999)
	{
		Input_Ref->Num/=10;
		Input_Ref->Dot=4;
		
	}
	value=Input_Ref->Num*1000;
	value/=(pow(10,4-Input_Ref->Dot));
	if(value>60000)
	{
		value=60000;
		
		
	}
	if(value>10000)
	{
		Input_Ref->Dot=2;
		Input_Ref->Num=value/10;
	
	
	}else //if(value>1e3)
	{
		Input_Ref->Dot=3;
		Input_Ref->Num=value;
	
	
	}
//	else
//	{
//		Input_Ref->Dot=3;
//		Input_Ref->Num=value;
//	
//	
//	}
	Input_Ref->mid_data=value;
//	value=pow(10.0,4-(float)Input_Ref->Dot);
//	value=Input_Ref->Num/value;
//	if(value>=1000)
//	{
//		Input_Ref->Unit++;
//		Input_Ref->Dot+=3;
//	
//	
//	}else if(value==0)
//	{
//		if(Input_Ref->Unit>0)
//		{
//			Input_Ref->Unit--;
//			if(Input_Ref->Dot>=3)
//				Input_Ref->Dot-=3;
//		
//		
//		}
//	
//	
//	}
	
	return *Input_Ref;
}
Sort_TypeDef Input_Set_CovPre(Sort_TypeDef *Input_Ref)//
{
//	vu8 i;
//	vu32 buff[]={10000,1000,100,10,1};//10.000
	vu32 value;
	Input_Ref->Unit=0;
	//if(Input_Ref->Dot)
	value=pow(10.0,(float)Input_Ref->Dot);
	value=Input_Ref->Num/value;
	if(value>100)
	{
		
		Input_Ref->Dot=2;
	
	
	}
	
	return *Input_Ref;
}
void Disp_Cp_D(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+8*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);

}
void Disp_Cp_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Cp_G(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+6*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_Cp_Rp(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)_p);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_Cs_Rs(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_Cs_D(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+8*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+8);

}
void Disp_Cs_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)_C);//_p
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_Rp(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)_p);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_Rd(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_D(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+8*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Lp_G(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+6*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Ls_D(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+8*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Ls_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+9);

}
void Disp_Ls_Rs(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+2*40*55/8);

}
void Disp_Ls_Rd(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+1*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10*40*55/8);

}
void Disp_Z_d(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+3*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+10*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}
void Disp_Z_r(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+3*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+11*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}
void Disp_Y_d(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+4*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+10*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}
void Disp_Y_r(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+4*40*55/8);//
	LCD_ShowFontCN_40_55(60+40,92,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+11*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55,(uint8_t*)Out_Assic+10);

}
void Disp__(void)
{
    uint8_t i;
    Colour.black=LCD_COLOR_TEST_MID;
    for(i=0;i<3;i++)
    {
//        for(j=0;j<5;j++)
//        LCD_ShowFontCN_40_55(60+40*(j+1),92+55*i,32,55,(uint8_t*)_num[11*32*55/8]);
        WriteString_Big(60+40, 92+53*i, "-----");
    
    }


}
void Disp_R_X(void)
{
	Colour.black=LCD_COLOR_TEST_MID;
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+5*40*55/8);//R
//	
	LCD_ShowFontCN_40_55(60+40*6,92,40,55, (uint8_t*)Out_Assic+13*40*55/8);//M
	LCD_ShowFontCN_40_55(60+40*7,92,40,55,(uint8_t*)Out_Assic+20*40*55/8);//Ω
    
	LCD_ShowFontCN_40_55(60+15,90+55,40,55,(uint8_t*)Out_Assic+24*40*55/8);//I
//	
	LCD_ShowFontCN_40_55(60+40*6,90+55,40,55,(uint8_t*)Out_Assic+16*40*55/8);//u
	LCD_ShowFontCN_40_55(60+40*7,90+55,40,55,(uint8_t*)Out_Assic+31*40*55/8);//A
    
    LCD_ShowFontCN_40_55(60,90+55+53,40,55,(uint8_t*)Out_Assic+28*40*55/8);//T
    LCD_ShowFontCN_40_55(60+40*7,90+55+53,40,55,(uint8_t*)Out_Assic+29*40*55/8);
	

	

}

void Disp_Rs_Q(void)
{
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);//_p
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+5*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)Out_Assic+2*40*55/8);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_Rp_Q(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+5*40*55/8);//R
	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+9*40*55/8);
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}
void Disp_G_B(void)
{
	LCD_ShowFontCN_40_55(60,92,40,55,(uint8_t*)Out_Assic+6*40*55/8);//G
	//LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
	LCD_ShowFontCN_40_55(60,92+55,40,55,(uint8_t*)Out_Assic+7*40*55/8);//B
//	LCD_ShowFontCN_40_55(60+40,92,40,55,(uint8_t*)_p);
//	LCD_ShowFontCN_40_55(60+40,92+55,40,55, (uint8_t*)Out_Assic+22*40*55/8);
	//WriteString_Big(60,92+55 ,(uint8_t*)Out_Assic+6);

}

//const vu8 func_Item[][]=
//{};
void Disp_Button_Fun_Set(vu16 xpos,vu16 ypos,vu8 * Disp_Item,Button_Page_Typedef* Button_Page)
{
	vu8 while_flag=1,i;
	vu8 key,Disp_Flag=1;
	vu32 keynum;
	Button_Page->third=0;
	do{
		key=HW_KeyScsn();
	
	}
		while(key!=0xff);
	Button_Page->third=SaveData.Main_Func.Param.test;
	while(while_flag)
	{
		if(Disp_Flag)
		{
			Disp_Flag=0;
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
			Disp_Fastbutton();
			Colour.black=LCD_COLOR_SELECT;
			LCD_DrawRect( LIST1+88, FIRSTLINE-2,SELECT_1END , FIRSTLINE+SPACE1-4 , Colour.black ) ;//SPACE1
			WriteString_16(LIST1+88, FIRSTLINE, User_FUNC[Button_Page->third],  1);//增加算法  把顺序改过来
			Colour.black=LCD_COLOR_TEST_BUTON;
			for(i=0;i<5;i++)
			WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, (Disp_Item+i*8),  0);
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
			Disp_Flag=1;
			switch(key)
			{
				case Key_F1:
					if(Fun_SelectValue[0][Button_Page->force]!=0xff)
					{
					Button_Page->third=Fun_SelectValue[0][Button_Page->force];
						while_flag=0;
						
					}
					//SaveData.Main_Func.Param.test
					SaveData.Main_Func.Param.test=Button_Page->third;
					Button_Page->third=0xff;
					break;
				case Key_F2:
					if(Fun_SelectValue[1][Button_Page->force]!=0xff)
					{
						//if(Fun_SelectValue[3][Button_Page->force]!=0xff)
						Button_Page->third=Fun_SelectValue[1][Button_Page->force];
						while_flag=0;
					}
					SaveData.Main_Func.Param.test=Button_Page->third;
					if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=0xff;
					break;
				case Key_F3:
					if(Fun_SelectValue[2][Button_Page->force]!=0xff)
					{
						//if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=Fun_SelectValue[2][Button_Page->force];
						while_flag=0;
					}
					SaveData.Main_Func.Param.test=Button_Page->third;
					if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=0xff;
					break;
				case Key_F4:
					if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					{
						//if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=Fun_SelectValue[3][Button_Page->force];
						while_flag=0;
					}
					SaveData.Main_Func.Param.test=Button_Page->third;
					if(Fun_SelectValue[3][Button_Page->force]!=0xff)
					Button_Page->third=0xff;
					break;
				case Key_F5:
					while_flag=0;
					break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				while_flag=0;
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUPTEST);
				while_flag=0;
				break;
				case Key_LEFT:
					while_flag=0;
//					if(Button_Page->index==0)
//						Button_Page->index=6;
//					else
//					if(Button_Page->index>3)
//						Button_Page->index-=3;
//					else
//						Button_Page->index+=2;
//					Button_Page->page=0;
				Button_Page->index=0;
				break;
				case Key_RIGHT:
					while_flag=0;
//					if(Button_Page->index==0)
//						Button_Page->index=1;
//					else
//					if(Button_Page->index<=3)
//						Button_Page->index+=3;
//					else
//						Button_Page->index-=2;
//					Button_Page->page=0;
				Button_Page->index=10;
						
				break;
				case Key_DOWN:
					while_flag=0;
//					if(Button_Page->index>5)
//						Button_Page->index=0;
//					else
//						Button_Page->index++;
//					Button_Page->page=0;
				Button_Page->index=2;
					
				break;
				case Key_UP:
					while_flag=0;
//					if(Button_Page->index<1)
//						Button_Page->index=6;
//					else
//						Button_Page->index--;
//					Button_Page->page=0;
				Button_Page->index=0;
				break;
				default:
					break;
				
			
			
			
			}
			
		
		
		}
	
	
	}


}
void Disp_Testvalue(int8_t chosen,int32_t eee )
{
	
	//WriteString_Big(100,92 ,Test_Dispvalue.Main_valuebuff);
	WriteString_Big(90+40,92 ,DispBuf);
	if(chosen==VH_FAIL||chosen==VL_FAIL||chosen==ALL_FAIL)
		Colour.Fword=LCD_COLOR_BLUE;
	else
		Colour.Fword=LCD_COLOR_WHITE;
	V_BCD_Int(eee);
//	if(Test_Unit.V_Neg)
//		WriteString_Big(100,92+55 ," ");
//	else
//		WriteString_Big(100,92+55 ,"-");
		
	WriteString_Big(90+40,92+53 ,DispBuf);
	
	Colour.Fword=LCD_COLOR_WHITE;
	Colour.black=LCD_COLOR_TEST_BACK;

//	WriteString_16(30, 92+55+55+8, "Test...",  0);
	
	
	//显示档号


}
void Disp_Big_MainUnit(vu8 unit,vu8 unit1)
{
	const vu8 nuitnum[]={12,15,16,17,22,14,13};
	const vu8 nuit_nuit[]={18,19,20,2,11,23,22};//F,H,Ω，S r °空格
	LCD_ShowFontCN_40_55(DISP_UNIT_XPOS,DISP_UNIT_YPOS,40,55,(uint8_t*)Out_Assic+nuitnum[unit]*(55*40/8));
	LCD_ShowFontCN_40_55(DISP_UNIT_XPOS+40,DISP_UNIT_YPOS,40,55,(uint8_t*)Out_Assic+nuit_nuit[unit1]*(55*40/8));
}

void Disp_Big_SecondUnit(vu8 unit,vu8 unit1)
{
	const vu8 nuitnum[]={12,15,16,17,22,14,13};
	const vu8 nuit_nuit[]={18,19,20,2,11,23,22};//F,H,Ω，S r °空格
	LCD_ShowFontCN_40_55(DISP_UNIT_XPOS,DISP_UNIT_YPOS+55,40,55,(uint8_t*)Out_Assic+nuitnum[unit]*(55*40/8));
	LCD_ShowFontCN_40_55(DISP_UNIT_XPOS+40,DISP_UNIT_YPOS+55,40,55,(uint8_t*)Out_Assic+nuit_nuit[unit1]*(55*40/8));
}

void Send_Freq(Send_Ord_Typedef *ord)
{
	vu8 Send_buff[10]={0xAB,0X30};
	
	Send_buff[2]=Save_Res.Set_Data.Range;
	Send_buff[3]=0xbf;
//	Send_buff[6]=0X80;//校验和 
//	Send_buff[7]=0XBF;
//	Send_buff[8]=0;
	UARTPuts( LPC_UART0, Send_buff);
	//_printf("s%",(const)Send_buff);

}
//void Send_Start(void)
//{
//    vu8 Send_buff[24]={0xAB,0X01,23,0x06};



//}
void Send_Set_Value(void)
{
    
//    vu8 *pt;
    vu8 i;
    vu8 sendbuff[100];
    Delay(1);
    UARTPutChar(LPC_UART0, 0xaa);
    Delay(1);
    UARTPutChar(LPC_UART0, 0xF1);
    Delay(1);
    sendbuff[0]=Tft_5520.Set_Value[Tft_5520.Group].Speed;
    sendbuff[1]=Tft_5520.Set_Value[Tft_5520.Group].voltage%10;
    sendbuff[2]=(Tft_5520.Set_Value[Tft_5520.Group].voltage/10)%10;
    sendbuff[3]=(Tft_5520.Set_Value[Tft_5520.Group].voltage/100)%10;
    sendbuff[4]=(Tft_5520.Set_Value[Tft_5520.Group].voltage/1000)%10;//电压
    sendbuff[5]=Tft_5520.Set_Value[Tft_5520.Group].Delay_Time%10;
    sendbuff[6]=(Tft_5520.Set_Value[Tft_5520.Group].Delay_Time/10)%10;
    sendbuff[7]=(Tft_5520.Set_Value[Tft_5520.Group].Delay_Time/100)%10;
    sendbuff[8]=(Tft_5520.Set_Value[Tft_5520.Group].Delay_Time/1000)%10;//测试时间
    sendbuff[9]=Tft_5520.Set_Value[Tft_5520.Group].Hi_Open;
    sendbuff[10]=Tft_5520.Set_Value[Tft_5520.Group].Lo_Open;
    sendbuff[11]=Tft_5520.Set_Value[Tft_5520.Group].Range;
    sendbuff[12]=Tft_5520.Set_Value[Tft_5520.Group].beep_Mode;
    sendbuff[13]=(Tft_5520.Set_Value[Tft_5520.Group].Hi_Limit%10);
    sendbuff[14]=(Tft_5520.Set_Value[Tft_5520.Group].Hi_Limit/10)%10;
    sendbuff[15]=(Tft_5520.Set_Value[Tft_5520.Group].Hi_Limit/100)%10;
    sendbuff[16]=(Tft_5520.Set_Value[Tft_5520.Group].Hi_Limit/1000)%10;
    sendbuff[17]=(Tft_5520.Set_Value[Tft_5520.Group].Lo_Limit%10);
    sendbuff[18]=(Tft_5520.Set_Value[Tft_5520.Group].Lo_Limit/10)%10;
    sendbuff[19]=(Tft_5520.Set_Value[Tft_5520.Group].Lo_Limit/100)%10;
    sendbuff[20]=(Tft_5520.Set_Value[Tft_5520.Group].Lo_Limit/1000)%10;
    sendbuff[21]=Tft_5520.Set_Value[Tft_5520.Group].Short;
//    pt=(vu8*)&Tft_5520.Set_Value[Tft_5520.Group];
//    num=sizeof(Tft_5520.Set_Value[Group]);
    for(i=0;i<22;i++)
    {
        UARTPutChar(LPC_UART0, sendbuff[i]);
        Delay(1);
        
    }
    Delay(1);
    UARTPutChar(LPC_UART0, 0xbf);
    Delay(100);
    


}
void Send_UartStop(void)
{
    vu8 Send_buff[24]={0xAA,0XFb,0XBF,0};
    vu8 i;
    for(i=0;i<3;i++)
    {
        UARTPutChar(LPC_UART0, Send_buff[i]);
        //Delay(1);
    
    }
   // UARTPuts( LPC_UART0, Send_buff);

}
void Send_UartStart(void)//启动命令
{
	vu8 Send_buff[24]={0xAA,0XFA,0XBF,0};
    vu8 i;
	for(i=0;i<3;i++)
    {
        UARTPutChar(LPC_UART0, Send_buff[i]);
        Delay(1);
    
    }
	

}
void Send_UartClear(void)//清零命令
{
	vu8 Send_buff[24]={0xAA,0XFE,0XBF,0};
    vu8 i;
	for(i=0;i<3;i++)
    {
        UARTPutChar(LPC_UART0, Send_buff[i]);
        Delay(1);
    
    }
	

}
void Send_Debug_Value(vu16 value)
{
    vu8 Send_buff[12]={0xAB,0Xf4,'a',0,'f',0,0,0,0,0xbf,0},i;
    //Tft_5520.Debugvalue
    Hex_Format((value),0,4,FALSE);
    for(i=0;i<4;i++)
    Send_buff[5+i]=DispBuf[i];

    
    
//	Send_buff[4]=ord->name;
//	Send_buff[5]=0X80;//校验和 
//	Send_buff[6]=0XBF;
//	Send_buff[7]=0;
	//UARTPuts( LPC_UART0, Send_buff);
    for(i=0;i<10;i++)
    {
        UARTPutChar(LPC_UART0, Send_buff[i]);
        Delay(1);
    
    }
    Delay(200);

}
//num 0  电压校正 1  1.000M校正点  2  10.00M校正点 
//3  100.0M校正点   4  1000M校正点
//校正串口发送数据，
void Send_Request(uint8_t order)
{
	vu8 Send_buff[10]={0xAB,0Xf2,0xbf},i;
    Send_buff[1]=order;
    for(i=0;i<3;i++)
    {
        UARTPutChar(LPC_UART0, Send_buff[i]);
        Delay(1);
    
    }
    Delay(100);
}


//
void Send_Sendstaetdebug(uint8_t num)
{
    vu8 Send_buff[10]={0xAB,0Xf2,'a',0xbf,0};
	vu8 i;
	Send_buff[2]=num;

	for(i=0;i<4;i++)
    {
        UARTPutChar(LPC_UART0, Send_buff[i]);
        Delay(1);
    
    }
    Delay(100);
 
}
//void Send_Uart3(uint8_t *buff)
//{
//	vu8 Send_buff[10]={0xAB,0X00,0xbf,0};
//	
////	Send_buff[3]=ord->Ordel;
////	Send_buff[4]=ord->name;
////	Send_buff[5]=0X80;//校验和 
////	Send_buff[6]=0XBF;
////	Send_buff[7]=0;
////	UARTPuts( LPC_UART3, Send_buff);
//	UARTPutChar (LPC_UART3, 0xaa);
//	UARTPuts( LPC_UART3, buff);
//	//_printf("s%",(const)Send_buff);

//}
void Send_Main_Ord(void)
{
	vu8 Send_buff[20]={0xAB};
	//Send_Mainord_Typedef *pt;
//	vu8 *pt;
//	vu8 i;
//	pt=(vu8 *)&Send_Mainord;
//	Send_Mainord.first.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][0];
//	Send_Mainord.first.Ordel=0X60;
//	Send_Mainord.second.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][1];
//	Send_Mainord.second.Ordel=0X70;
//	Send_Mainord.third.Ordel=0X71;
//	Send_Mainord.third.name=Disp_Main_Ord[SaveData.Main_Func.Param.test][2];
//	for(i=0;i<sizeof(Send_Mainord);i++)
//	{
//		Send_buff[4+i]=*(pt+i);
//	
//	}
//	Send_buff[3]=Send_Mainord.first.name;
//	Send_buff[4]=ord->name;
	Send_buff[1]=0X10;//校验和 
	Send_buff[2]=13;
	Send_buff[3]=keynum;
	Send_buff[4]=2;
	Send_buff[4]=0;
	Send_buff[10]=0XBF;
	Send_buff[11]=0;
	UARTPuts( LPC_UART0, Send_buff);


}
void Disp_RangeDispvalue(vu8 data)
{
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(20, 200, RangeDisp_Main[data],  1);//
	WriteString_16(280, 200, RangeDisp_Second[data],  1);
	Colour.black=LCD_COLOR_TEST_MID;
	LCD_ShowFontCN_40_55(80,92+30,40,55, (uint8_t*)Out_Assic+7*40*55/8);
	LCD_ShowFontCN_40_55(80+40,92+30,40,55, (uint8_t*)Out_Assic+24*40*55/8);
	LCD_ShowFontCN_40_55(80+40+20,92+30,40,55, (uint8_t*)Out_Assic+25*40*55/8);
//


}
void Disp_Range_DispMainUnit(vu8 num,vu8 unit)
{
	WriteString_16(128, 200, (uint8_t*)Disp_Unit[num],  1);//单位的量
	WriteString_16(136, 200, (uint8_t*)Disp_Range_Main_Disp[unit],  1);//单位

}
void Disp_Range_DispSecondUnit(vu8 num,vu8 unit)
{
	WriteString_16(388, 200, (uint8_t*)Disp_Unit[num],  0);//单位的量
	WriteString_16(396, 200, (uint8_t*)Disp_Range_Main_Disp[unit],  0);//单位

}
void Disp_RangeTestvalue(void)
{
	//u8 i;
	const vu8 DISP_UnitMain[]=
	{ 0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,3,3,2,2,2};
	const vu8 DISP_UnitSecond[]=
	{ 6,6,3,2,6,6,2,6,2,2,6,6,6,2,2,5,4,5,4,2,6,6};
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(52,200 ,Test_Dispvalue.Main_valuebuff,1);//
	WriteString_16(312,200 ,Test_Dispvalue.Secondvaluebuff,1);//
	Disp_Range_DispMainUnit(Test_Dispvalue.Unit[0],DISP_UnitMain[SaveData.Main_Func.Param.test]);
	Disp_Range_DispSecondUnit(Test_Dispvalue.Unit[1],DISP_UnitSecond[SaveData.Main_Func.Param.test]);
	//Count_buff
	
	

}
void Disp_RangeCount(void)
{
	
	u8 i;
	
	for(i=0;i<11;i++)
	
	{
		if((i>1&&i<7)||((i>8)))
			Colour.black=LCD_COLOR_TEST_MID;
		else
			Colour.black=LCD_COLOR_TEST_BACK;
		Hex_Format(Count_buff[i] , 0, 6 , 0);//加单位
				
		WriteString_12(LIST2+140, 58+i*15, DispBuf,  1);//显示标称值  后面要接着显示单位
	}

}
void Disp_Set_Unit_16(uint32_t unit,uint32_t x,uint32_t y)//Disp_Range_Main_NUm
{
	WriteString_16(x, y, Disp_Range_Main_Disp[Disp_Range_Main_NUm[unit]],  0);


}
void Disp_Set_Unit_12(uint32_t unit,uint32_t x,uint32_t y)
{
	WriteString_12(x, y, Disp_Range_Main_Disp[unit],  1);


}
void Disp_Test_Unit_v(uint32_t x,uint32_t y)
{
	WriteString_16(x, y, "V",  1);

}
void Disp_Test_Unit_mA(uint32_t x,uint32_t y)
{
	WriteString_16(x, y, "mA",  1);

}
void Savetoeeprom(void)
{
	//Saveeeprom
	u8 *pt;

	
	pt=(u8*)&Tft_5520;
	
	EEPROM_Write(0, 1, pt, MODE_8_BIT, sizeof(Tft_5520));


}
void ReadSavedata(void)
{
//	u8 i;
	Tft_5520_Typedef *pt;
	//Saveeeprom_Typedef 
	pt=&Tft_5520;
	EEPROM_Read(0, 1, (u8 *)pt, MODE_8_BIT, sizeof(Tft_5520));
}

const vu16 Set_Data_Comp[][2]=
{
//    {0,1},
//    {0,GROUP_MAX-1},
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
const vu16 Set_Data_Int[]={0,100,100,0,0,0,0,1000,1,0};
const  vu8 Sys_Data_Comp[][2]=
{
	{0,1},
	{0,4},
	{0,1},
	{0,1},
	{0,2},
	{1,255},
	{1,31},
	{1,24},
	{0,59},
	{0,59}

	
};
//设置数据的上下限判定
void SetData_High_Low_Comp(void)
{
	vu16 j,i;
	vu8 *pt;
    uint16_t *ppt;
    ppt=(uint16_t *)&Tft_5520.Set_Value;
    for(i=0;i<GROUP_MAX;i++)
    {
        for(j=0;j<SET_MAX-1;j++)
        {
            if(*(ppt+i*sizeof(Tft_5520.Set_Value[0])/2+j)>Set_Data_Comp[j][1]||*(ppt+i*sizeof(Tft_5520.Set_Value[0])/2+j)<Set_Data_Comp[j][0])
                *(ppt+i*sizeof(Tft_5520.Set_Value[0])/2+j)=Set_Data_Int[j];
        
        }
    
    
    }

	pt=(vu8 *)&Tft_5520.Sys_Set;
	for(i=0;i<SYS_MAX;i++)
	{
	if(*(pt+i)>Sys_Data_Comp[i][1])
		*(pt+i)=Sys_Data_Comp[i][0];
	
	}
	
	

}
void Disp_Range(uint8_t flag)
{
	Colour.black=LCD_COLOR_TEST_BACK;
	if(flag)//Hand_RangeDisp
		WriteString_16(LIST2+88, FIRSTLINE, Hand_RangeDisp[Tft_5520_Dispvalue.Range],  0);
	else
		WriteString_16(LIST2+88, FIRSTLINE, Auto_RangeDisp[Tft_5520_Dispvalue.Range],  0);
		//riteString_12(LIST2+140, 58+i*15, Auto_RangeDisp[Test_Dispvalue.Rangedisp],  1);

}
void Disp_Usbflag(uint8_t flag)
{
    u32 Fword;
    Fword=Colour.Fword;
	Colour.black=LCD_COLOR_TEST_MID;
	if(flag==1)
		Colour.Fword=LCD_COLOR_GREEN;
		
	else if(flag==2)
		Colour.Fword=LCD_COLOR_BLUE;
	else
		Colour.Fword=LCD_COLOR_TEST_BUTON;
//	LCD_ShowFontCN_40_55(uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize,uint8_t * image);
	LCD_ShowFontCN_40_55(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE,BUTTOM_Y_VALUE-20,48,24,(uint8_t*)nBitmapDot);
	//WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, " 显示",  0);
	Colour.Fword=Fword;

}
const u8 Bit_Dot[][4]=
{
    {0,1,2,3},
    {0,1,2,4},
    {0,1,3,4},
    {0,2,3,4},

};

//==========================================================
//函数名称：Number_Setup
//函数功能：数值框设置处理
//入口参数：NumBox_TypeDef
//出口参数：键值
//创建日期：2015.10.28
//修改日期：2015.10.28 09:44
//备注说明：无
//==========================================================
int8_t Number_Setup(NumBox_TypeDef * pt)
{
	u8 key;
	u8 len=0;
    u8 i;
//	u8 count;//步进计数器
//	u8 current;//当前设置位
    vu16 Disp_bit;
    vu16 Dispset_buff[4];
//    const u16 div[]={1000,100,10,1};
    vu16 dispdata;
    vu16 count_f=0;
    vu16 current_f;
	vu8 blink;//显示闪烁标志
	LcdAddr_TypeDef addr;//显示地址备份

	blink=TRUE;//显示闪烁标志

	addr.x=LcdAddr.x;//显示地址备份
	addr.y=LcdAddr.y;
    Disp_bit=LcdAddr.x;
    current_f=Disp_bit;

    dispdata=pt->Num;
    for(i=4;i>0;i--)
    {
        Dispset_buff[i-1]=dispdata%10;
        dispdata/=10;
    
    }

	while(1)
	{
        count_f++;
		if(blink)//显示
		{
			LcdAddr.x=addr.x;//显示地址
			LcdAddr.y=addr.y;
             dispdata=Dispset_buff[3]+Dispset_buff[2]*10+Dispset_buff[1]*100+Dispset_buff[0]*1000;
			Hex_Format(dispdata,pt->Dot,4,TRUE);//数值格式化，4位数值
            WriteString_16(LcdAddr.x, LcdAddr.y, DispBuf,  1);
			//显示菜单值
		}
        else
        {
        
            LcdAddr.x=Disp_bit;
            PutChar( LcdAddr.x, LcdAddr.y, ' ', LCD_COLOR_SELECT, LCD_COLOR_SELECT );

        }
        if(count_f>5)
        {
            if(blink)
                blink=FALSE;
            else
                blink=TRUE;
            
            count_f=0;
        }
        Delay(20);
        key=HW_KeyScsn();
		if(key==0xff)
		{
			keynum=0;
		}
		else
			keynum++;
        if(keynum==5)
		{	
			//Disp_Flag=1;
        Key_Beep();
		switch(key)
		{
            case Key_EXIT://退出
                
                //break;
            case Key_STOP://停止按键
                if(GetSystemStatus()==SYS_STATUS_USERDEBUG)
                {
                    Send_Request(0xf5);
                    Delay(100);
                    
                    
                }
                return(0);
                
			case Key_DOWN:		//下键
                blink=TRUE;//显示闪烁标志

                    if(Dispset_buff[len]>0)
                        Dispset_buff[len]--;
                    else
                        Dispset_buff[len]=9;
//                   

				break;

			case Key_UP:	//上键
                  blink=TRUE;//显示闪烁标志

                    if(Dispset_buff[len]<9)
                        Dispset_buff[len]++;
                    else
                        Dispset_buff[len]=0;
 
				break;
            case Key_FAST :
                if(dispdata>pt->Max)
                    dispdata=pt->Max;
                if(dispdata<pt->Min)
                    dispdata=pt->Min;
                pt->Num=dispdata;
//                if(GetSystemStatus()==SYS_STATUS_USERDEBUG)
//                {
//                    Send_Request(0xf4);
//                    Delay(100);
//                    
//                }
                
				return(1);
               // break;
            case Key_LEFT:
   				blink=TRUE;//显示闪烁标志
                if(current_f+len*10<current_f+3*10)
                {
                    //Disp_bit_c++;
                    len++;
                    
                }
                else
                {
                    len=0;
                    
                }
                Disp_bit=current_f+Bit_Dot[NumBox.Dot][current_f+len-current_f]*10;
                break;
            case Key_RIGHT:
                blink=TRUE;//显示闪烁标志
                if(current_f+len*10>current_f)
                {
                    len--;
                    
                }
                else
                {
                    len=3;
                    
                }
                Disp_bit=current_f+Bit_Dot[NumBox.Dot][current_f+len-current_f]*10;
                break;


            }
        }
	}
    
}
void Disp_Teststatue(uint8_t num)
{
    uint32_t colour=Colour.black;
    Colour.black=LCD_COLOR_TEST_BAR;
    WriteString_16(10, 272-17, TFT5520_state[num][Tft_5520.Sys_Set.Lang],  0);
    Colour.black=colour;

}
//==========================================================
//函数名称：Password_Check
//函数功能：密码校验
//入口参数：无
//出口参数：TRUE:有效
//创建日期：2015.01.29 
//修改日期：2015.08.24 11:40
//备注说明：无
//==========================================================
//Bool Password_Check(void)
//{
//	u8 key;

//	if(MenuIndex!=2)//出厂信息界面
//	{
//		if(F_Password==TRUE)//密码有效标志	
//			return (TRUE);
//	
//		if(SaveData.Factory.Password==0)//密码为零时无需校验
//			return (TRUE);
//	}
//	else
//	{
//		Disp_Clr( );//清屏
//	}
//	
//	LcdAddr.x=10;LcdAddr.y=2;
//	Disp_Str("请输入密码",0);//显示
//	Lcd_Draw_Line(0,20,LCD_ADDR_X_MAX);//X=0:画横线，否则画竖线，len:长度或高度
//	Lcd_Draw_Line(0,107,LCD_ADDR_X_MAX);//X=0:画横线，否则画竖线，len:长度或高度
//	LcdAddr.x=1;LcdAddr.y=110;
//	Disp_Str("按<上,下>键修改,<确认>键输入",0);//显示

//	while(1)
//	{
//		LcdAddr.x=13;//设置显示地址
//		LcdAddr.y=50;
//		Disp_Str("****",0);//显示

//		LcdAddr.x=13;//设置显示地址
//		NumBox.Num=0;//Value
//		NumBox.Max=9999;//Max
//		NumBox.Min=0;//Min
//		NumBox.Dot=0;//Dot

//		key=Number_Setup(&NumBox);//数值框设置
//		switch(key)//按键处理
//		{
//			case KEY_UP:	//上键
////			case L_KEY_UP:	//长按上键
//				break;

//			case KEY_DOWN:		//下键
////			case L_KEY_DOWN:	//长按下键
//				break;

//			case KEY_LEFT:	//左键
////			case L_KEY_LEFT:	//长按左键
//				break;
//	
//			case KEY_SET:	//设置键
////			case L_KEY_SET:	//长按设置键
////				break;

//			case KEY_RIGHT:		//右键
////			case L_KEY_RIGHT:	//长按右键
//				break;
//	
//			case KEY_ENTER:		//确认键
////			case L_KEY_ENTER:	//长按确认键
//				Disp_Clr();//清屏
//				if(MenuIndex==2)//出厂信息界面
//				{
//					if(NumBox.Num==918)//出厂配置密码
//						return (TRUE);
//					else
//						return (FALSE);
//				}
//				else
//				{
//					if(NumBox.Num==SaveData.Factory.Password)//当前密码判别
//					{
//						F_Password=TRUE;//密码验证有效
//						return (TRUE);
//					}
//					else
//						return (FALSE);
//				}
//				break;
//	
//			case KEY_START:		//启动键
////			case L_KEY_START:	//长按启动键
//				break;
//	
//			case KEY_RESET:		//复位键
////			case L_KEY_RESET:	//长按复位键
//				return (FALSE);
//				break;
//			
//			default:
//				break;
//		}
//	}
//}
