//==========================================================
//文件名称：GlobalValue.h
//文件描述：全局变量头文件
//文件版本：Ver1.0
//创建日期：2015.10.26 
//修改日期：2015.10.28 15:38
//文件作者：黄宏灵
//备注说明：无
//注意事项：无
//==========================================================
#ifndef __GlobalValue_h__
#define __GlobalValue_h__
#include "stdint.h"
//#include "TypeDefine.h"
#define FIT_INUM    12
//==========================================================
//软件版本：10
#define SOFTWARE_VERSION		(0x10)

//==========================================================
//默认出厂编号(0)
#define DEFAULT_MACHINE_ID 		(0)

//默认出厂密码(0)
#define DEFAULT_PASSWORD 		(0)

//默认开机次数(0)
#define SET_BOOT_DEFAULT		(0)

//==========================================================
//调试配置
#define _DEBUG_CONFIG_FALSE		(0)
#define _DEBUG_CONFIG_TRUE		(1)

//==========================================================
//仿真调试
//#define DEBUG_SUPPORT			(_DEBUG_CONFIG_TRUE)
#define DEBUG_SUPPORT			(_DEBUG_CONFIG_FALSE)

//==========================================================
//ISP开关配置，仿真调试用
#if DEBUG_SUPPORT
	#define HW_ISP_SUPPORT		(_DEBUG_CONFIG_FALSE)
#else
	#define HW_ISP_SUPPORT		(_DEBUG_CONFIG_TRUE)
#endif

//==========================================================
//Uart开关配置
#define HW_UART_SUPPORT			(_DEBUG_CONFIG_TRUE)

//==========================================================
//蜂鸣器开关配置
#define HW_BEEP_SUPPORT			(_DEBUG_CONFIG_TRUE)

//==========================================================
//AD值滤波配置，增加数据稳定度
#define AD_FILTER_SUPPORT		(_DEBUG_CONFIG_TRUE)

//==========================================================
#define SETUP_MENU_MAX  	(2)//最大设置菜单

#define DEBUG_MENU_MAX  	(6)//最大校准菜单

#define PARAMETER_MENU_MAX	(3)//最大参数菜单

#define SYSTEM_MENU_MAX		(4)//最大系统菜单

#define CONFIG_MENU_MAX		(3)//最大配置项(0-2)

#define PW_LENGTH 			(4)//最大密码位数

#define RANGE_MAX 			(4)//最大量程

#define AD_BUF_LENGTH		(8)//A/D值缓冲区长度

#define MAX_R_RANGE			(5)//电阻最大量程

#define BUTTOM_X_VALUE		(84)//显示下面提示显示的第一个X位置

#define BUTTOM_MID_VALUE	(80)//显示下面提示显示的中间间隔位置

#define BUTTOM_Y_VALUE		(271-30)//显示下面提示显示的第一个Y位置

#define KEY_NUM				(3)	//按键灵敏度

#define NUM_LENTH 			(5)//显示长度
#define NUM_FREQ 			(33+4)//显示长度
#define DEBUG_RANGE			(4+1)
#define SET_MAX             (11)
#define SYS_MAX				(6)
#define GROUP_MAX           (5)

#define USBSENDLENTH        (27)
//==========================================================
#define OHM 				(0xF4)	//CGROM中有Ω字符，编码为0xF4

//==========================================================
//显示分区的X坐标
#define MSG_ADDR_X 			(1)
#define AUTO_ADDR_X 		(12)
#define RANGE_ADDR_X 		(17)

#define RES_ADDR_X 			(1)
#define VOL_ADDR_X 			(12)

#define DISP_UNIT_XPOS	380
#define DISP_UNIT_YPOS	92
//==========================================================
//标题长度
#define TITLE_LEN_MAX		(8)
typedef unsigned          char vu8;
typedef unsigned short     int vu16;
typedef unsigned           int vu32;

extern uint8_t Send_buff2[30];
typedef struct
{
    vu16 x;
    vu16 y;

}
LcdAddr_TypeDef;

extern LcdAddr_TypeDef LcdAddr;

typedef struct 
{
    vu8 start;//头
    vu8 king;
    vu8 range;
    vu8 dianya[4];//电压ascii
    vu8 dianzu[6];
    vu8 time[5];
    vu8 comp;
    vu8 end;

}Send_Testvalue_Typedef;
extern Send_Testvalue_Typedef Send_Testvalue[2];

typedef struct 
{
    char start;//帧头
//    char addr;//通讯地址
    char voltage[5];//5字节电压ASCII
    char risistence[6];//6字节电阻ASCII，最后一个字节为单位
		char current[6];//6字节电流ASCII，最后一个字节为单位
    char time[4];//4字节时间，单位0.1s
    u8 comp;//分选结果
    char end;//帧尾
}SendPC_Testvalue_Typedef;
extern SendPC_Testvalue_Typedef SendPC_Testvalue;

typedef struct 
{
    uint8_t    Range;
    uint16_t    Test_R;
    uint8_t    Test_R_Dot;  
    uint16_t    Test_V;
    uint16_t    Test_T;
    uint8_t    comp;


}Tft_5520_Disp_Typedef;
extern Tft_5520_Disp_Typedef Tft_5520_Dispvalue;
//数值框属性定义
typedef struct
{
	vu8 *Title;//标题
	vu16 Num;//数值
	vu8  Dot;//小数点(0-3: xxxx xxx.x xx.xx x.xxx)
	//vu8  Unit;//单位(0-2: MΩ  GΩ  TΩ)
	vu16 Max;//最大值
	vu16 Min;//最小值
}NumBox_TypeDef;
extern NumBox_TypeDef NumBox;
//绝缘电阻定义
typedef struct 
{
    uint16_t Speed;
    uint16_t voltage;
    uint16_t Delay_Time;
    uint16_t Hi_Open;
    uint16_t Lo_Open;
    uint16_t Range;
    uint16_t beep_Mode;
    uint16_t Hi_Limit;
    uint16_t Lo_Limit;
    uint16_t Short;
    
    

}Set_Value_Typedef;//主菜单设置
typedef struct 
{
    uint8_t Uart;
    uint8_t Buad;
    uint8_t U_Switch;
    uint8_t I_O;
    uint8_t Lang;
		uint8_t Addr;
}Sys_Set_Typedef;
typedef struct 
{
    uint8_t Group;
    Set_Value_Typedef Set_Value[GROUP_MAX];
    Sys_Set_Typedef Sys_Set;
    uint16_t Debugvalue[5];

}Tft_5520_Typedef;
extern Tft_5520_Typedef Tft_5520;
//交流电阻数据定义
//带小数点和单位的数
typedef struct
{
	vu16 standard;//标准显示值
	vu16 ad_value;

}Debug_Value_Typedef;
typedef struct
{
	vu8 Res_dot;
	vu8 V_dot;
	vu8 V_Neg;
}Test_UnitTypedef;
extern Test_UnitTypedef Test_Unit;

typedef struct 
{
	vu8 sign;
	vu32 Num;
	vu32 Dot;
	vu32 Unit;
	vu32 Updata_flag;
	vu32 Order;
	vu32 mid_data;
}Sort_TypeDef;
typedef struct
{
	vu8 trip;//触发
	vu8 speed;//测试速度
	vu8 Res_comp;
	Sort_TypeDef Res_low;//电阻下限
	vu8 V_comp;
	Sort_TypeDef V_low;
	vu8 Range;
	vu8 beep;
	Sort_TypeDef Nominal_Res;
	Sort_TypeDef High_Res;
	Sort_TypeDef Nominal_V;
	Sort_TypeDef	V_high;
	


}Set_Data_Typedef;
typedef struct
{
	vu8 Year;
	vu8 Mon;
	vu8 data;

}Data_Value_Typedef;
typedef struct
{
	vu8 Hour;
	vu8 Min;
	vu8 Sec;

}Timer_Value_Typedef;
typedef struct
{
	vu8 uart;
	vu8 buard;
	vu8 U_store;
	vu8 plc;
	vu8 lanage;
	
	Timer_Value_Typedef Time;
	Data_Value_Typedef Data;
	//vu8 
	

}Sys_Setvalue_Typedef;
typedef struct 
{
	vu8 buff[8];
	vu8 flag;

} Out_Typedef;
typedef struct
{
	Set_Data_Typedef Set_Data;
	Debug_Value_Typedef Debug_Value[DEBUG_RANGE];
	Sys_Setvalue_Typedef Sys_Setvalue;
	float clear;
	float V_Clear;
	vu8 softswitch;
	Out_Typedef Production_Data;
	Out_Typedef Number;

}Save_Res_Typedef;

extern Save_Res_Typedef Save_Res;



//


//数值框属性定义
//typedef struct
//{
//	vu8 *Title;//标题
//	vu16 Num;//数值
//	vu8  Dot;//小数点(0-3: xxxx xxx.x xx.xx x.xxx)
//	vu8  Unit;//单位(0-1: mOhm、Ohm)
//	vu16 Max;//最大值
//	vu16 Min;//最小值
//}NumBox_TypeDef;
typedef struct
{
	float first_value;
	float second_value;
	float third_value;
	float fourth_value;
	float comp_highvalue[10];
	float comp_lowvalue[10];
	


}Comp_Testvalue_Typedef;
extern Comp_Testvalue_Typedef  Comp_Testvalue;
//typedef struct
//{
//	vu8  buff[5];
//	vu8 dot;
//	vu8 unit;


//}
typedef struct
{
	vu8 Main_Funbuff[8];
	vu8 Main_Secondbuff[8];
	vu8 Main_Vmbuff[8];
	vu8 Main_Imbuff[8];


}Test_DispBCD_Typedef;
typedef struct
{
	vu8 Main_flag;
	vu8 Second_falg;

}Main_Second_TypeDed;

typedef struct
{
	vu8 buff[5];
	vu32 Num;
	vu32 Dot;
	vu32 Unit;


}Range_Set_Typedef;
typedef struct
{
	Range_Set_Typedef all[4];

}All_Compvalue_Typedef;
extern All_Compvalue_Typedef Comp_Change;
typedef struct
{
	vu32 test;
	vu32 page;

}Param_Typedef;
//==========================================================
typedef struct
{
	vu32 Auto;
	vu32 Range;

}AotoRange_Typedef;

//参数项(20项)测量设置
typedef struct 
{
	Param_Typedef Param;//功能(有列表)
	vu32 Freq;//频率(有列表)
	vu32 Level;//电平(1V  0.3V)
	vu32 Trig;//外部触发开关(0-3，内部 手动 外部 总线)
	vu32 Alc;//恒电平(0-1，默认关0)
	vu32 Rsou;//内阻(0-1，默认关0)
    Sort_TypeDef Trig_time;
    Sort_TypeDef Temp_time;
    vu32 Dev_a;
    vu32 Dev_b;
    AotoRange_Typedef Range;
    vu32 Bias;
    vu32 Speed;
    vu32 Avg;
    vu32 V_i;
    vu32 Dcr;
    AotoRange_Typedef DC_Range;
    vu32 DC_Level;
    Sort_TypeDef Ref_A; 
    Sort_TypeDef Ref_B;
	vu32 beep;
	vu32 buad;
//	vu8  Count;
}Main_Func_TypeDef;
typedef struct
{
	vu8 param;
	vu8 freq;
	vu8 level;
	vu8 trig;
	vu8 rsou;
	
	vu8 range;
	vu8 bisa;
	vu8 speed;
	vu8 avg;
	vu8 v_i;
	vu8 beep;
	vu8 buad;
	Sort_TypeDef Trig_time;
    Sort_TypeDef Temp_time;
	

}Save_Main_Funce_Typedef;
typedef struct
{
	Sort_TypeDef Mainvalue;
	Sort_TypeDef Secondvalue;
	Sort_TypeDef Vmvalue;
	Sort_TypeDef Imvalue;
	vu8 Rangedisp;
	vu8 Main_valuebuff[10];
	vu8 Secondvaluebuff[10];
	vu8 Vmvaluebuff[10];
	vu8 Imvaluebuff[10];
	vu8 Dot[4];
	vu8 Unit[4];
	vu32 Test_V;
	vu32 Test_R;
	


}Test_Dispvalue_TypeDef;
extern Test_Dispvalue_TypeDef Test_Dispvalue; 
typedef struct
{
    vu32 Open;//开路
    vu32 Short;//短路
    vu32 Load;//负载
    vu32 Spot;//校正点
    vu32 Freq;//频率
    Sort_TypeDef Ref_A;
    Sort_TypeDef Open_A;
    Sort_TypeDef Short_A;
    Sort_TypeDef Load_A;
    vu32 Cable;//电缆长度
    vu32 Mode;//方式
    vu32 Param;//功能
    Sort_TypeDef Ref_B;
    Sort_TypeDef Open_B;
    Sort_TypeDef Short_B;
    Sort_TypeDef Load_B;

}User_Correction_Typedef;
typedef struct
{
    Sort_TypeDef low;
    Sort_TypeDef high;

}Comp_Value_Typedef;
//极限列表设置
typedef struct
{
    vu32 Param;//参数
    Sort_TypeDef Nom;//标称
    vu32 Mode;//方式
    vu32 Aux;//附属
    vu32 Comp;//比较开关
    Comp_Value_Typedef Comp_Value[9];
    Comp_Value_Typedef Comp_Value_2nd;
	vu32 count;
}Limit_Tab_Typedef;

typedef struct
{
    vu32 Value;
    vu32 Unit; 
} Sweep_Value_Typedef;
typedef struct
{
    vu32 Freq;
    vu32 Level;
    vu32 Lmt;
    Sweep_Value_Typedef Low;
    Sweep_Value_Typedef High;
    Sweep_Value_Typedef Delay_Time;

}List_Value_Typedef;
//列表扫描设置
typedef struct
{
    vu32 mode;
    List_Value_Typedef  List_Value[200];


} List_Sweep_Typedef;

//系统设置
typedef struct
{
    vu32 Main_Func;
    vu32 Pass_Beep;
    vu32 Fail_Beep;
    vu32 Language;
    vu32 Password;
    vu32 Menu_Disp;
	Data_Value_Typedef Data_Value;
	Timer_Value_Typedef Timer_Value;
    vu32 Baue_Rate;
	
    vu32 Bias;
    vu32 Bus_Mode;
    vu32 GP_Addr;
    vu32 Talk_Only;
}Sys_Setup_Typedef;

typedef struct
{
	vu8 fun;
	vu8 num;
	vu8 freq[210];
	vu8 ac_leave[210];
	Range_Set_Typedef Range_Set_main[210];
	Range_Set_Typedef Range_Sed_Second[210];
	vu8 comp[210];
	Sort_TypeDef time[210];
	vu8 xpos;
	vu8 ypos;

}Limit_ScanValue_Typedef;
typedef struct
{
	
	User_Correction_Typedef		User_Correction;
	Limit_Tab_Typedef	Limit_Tab;
	List_Value_Typedef	List_Value;
	Sys_Setup_Typedef	Sys_Setup;
	Main_Func_TypeDef	Main_Func;
	Limit_ScanValue_Typedef Limit_ScanValue;
}SaveData_Typedef;
extern SaveData_Typedef SaveData;

typedef struct
{
	Save_Main_Funce_Typedef	Save_Main_Funce;
	Limit_Tab_Typedef	Limit_Tab;

}Saveeeprom_Typedef;
extern Saveeeprom_Typedef	Saveeeprom;

typedef struct
{
	vu8 index;
	vu8 page;
	vu8 third;
	vu8  force;

}Button_Page_Typedef;
typedef struct
{
	vu16 xpos;
	vu16 ypos;
	vu16 lenth;


}Disp_Coordinates_Typedef;
typedef struct
{
	vu8 Ordel;
	vu8 name;

} Send_Ord_Typedef;
typedef struct
{
	Send_Ord_Typedef first;
	Send_Ord_Typedef second;
	Send_Ord_Typedef third;

}Send_Mainord_Typedef;
extern Send_Mainord_Typedef Send_Mainord;
//extern Disp_Coordinates_Typedef Disp_Coordinates;

extern uint32_t  numBlks, blkSize;
extern uint8_t  inquiryResult[36];
extern uint8_t usb_oenflag;
extern uint8_t Group;
extern  uint8_t debug_saveflag;
//==========================================================
//系统项(2项)
//typedef struct 
//{
//	Sort_TypeDef Res_Hi;//上限(xxxx mΩ)
//	Sort_TypeDef Res_Lo;//下限(xxxx mΩ)
//	Sort_TypeDef Vol_Hi;//上限(xxxx V)
//	Sort_TypeDef Vol_Lo;//下限(xxxx V)
//}System_TypeDef;

//==========================================================
//配置项(3项)
//typedef struct 
//{
//	vu16 Password;//设置密码(0-9999，默认0)
//	vu16 BootNum;//开机次数(0-9999，默认0)
//	vu16 ProductId;//出厂编号(0-9999，默认0)
//}Config_TypeDef;

//==========================================================
//校正值
//typedef struct 
//{
//	vu16 Num;  //显示值
//	vu16 NumAd;//A/D值
//}Cal_TypeDef;

////校正项
//typedef struct 
//{
//	Cal_TypeDef IrCur[4];//绝缘电阻电流值-4档
//	Cal_TypeDef IrVol[2];//绝缘电阻电压值-2档
//	
//}Calibrate_TypeDef;
//typedef struct 
//{
//	vu16 Real;  //实数
//	vu16 Imaginary;//虚数
//	vu16 DC_voltage;
//	vu8	 Dot;
//	vu8	 Negative;
//}Res_TypeDef;
//==========================================================
//保存信息
//typedef struct 
//{
//	Parameter_TypeDef Parameter;//参数项
//	System_TypeDef System;//系统项
//	Config_TypeDef Config;//配置项
//	Calibrate_TypeDef Calibrate;//校准项
//	vu16 Crc16;//校验字
//}Save_TypeDef;
//
////==========================================================
////默认校准值上下限
//extern const u16 CalibrateLimit[][2];
//extern const s16 BASE_NUM[6];
//extern const u8 DOT_POS[6];

//==========================================================
//全局变量
//extern  Save_TypeDef SaveData;//保存值
//extern  Cal_TypeDef Cal[6];//校准值
//extern  Res_TypeDef	Res;
//extern  vu8 Range;//当前量程
//extern  bool F_Password;//密码有效标志
//extern  bool F_100ms;//100ms定时标志
//extern  bool F_Fail;//测试失败标志
//extern  vu8 MenuIndex;//待机菜单项
//extern  vu8 MenuSelect;//菜单选择项
#define  FILENAME_R  "MSREAD.TXT"
#define  FILENAME_W  "JK5520.TXT"
extern  unsigned long SystemStatus;//系统状态
extern unsigned long SystemMessage;//系统信息
extern uint8_t DispBuf[12];
extern unsigned long Count_buff[12];
extern vu8 Uart_Send_Flag;
extern uint32_t Usb_Open_flag;
extern uint32_t softswitch;
extern uint8_t Uart3_returnflag;
extern uint8_t Pc_Sendflag;
//extern  vu16 Voltage;//测试电压
//extern  vu16 Current;//测试电流
//extern  vu16 Resistance,xxxx;//测试电阻
//extern  NumBox_TypeDef NumBox;//数值框
//extern vu16 i_buff[FIT_INUM];		   //滤波值
//extern vu8 ffit_data1;
//extern	vu8	count_ffit;
//extern 	u16 	zreo_num[6];
//extern u8 fuhao;
//extern bool clear_flag;
//extern bool vol_flag;
//extern u32 MenuIndex;//待机菜单项
//extern u32 MenuSelect;//菜单选择项
//
//extern u32 SystemStatus;//系统状态
//extern u32 SystemMessage;//系统信息

//==========================================================
//系统状态
enum SysStatusEnum
{	
	SYS_STATUS_ERROR ,			//异常
	SYS_STATUS_RESET ,			//复位
	SYS_STATUS_POWER ,			//开机
//	SYS_STATUS_POWER ,
	SYS_STATUS_DATASTORE ,		//数据保存
	SYS_STATUS_FILE ,			//文件管理
	SYS_STATUS_SETUPTEST ,			//测量设置
	SYS_STATUS_TEST ,			//测试
	SYS_STATUS_RANGE ,		//档号显示
	SYS_STATUS_RANGECOUNT,	//档计数显示
	SYS_STATUS_ITEM ,			//列表扫描显示
	SYS_STATUS_USERDEBUG,//用户校正 
	SYS_STATUS_FACRDEBUG, //工厂校正
	SYS_STATUS_SYSSET,     //系统设置
    SYS_STATUS_LIMITSET,     //极限设置
    SYS_STATUS_ITEMSET,//列表扫描设置
	SYS_STATUS_TOOL,//工具
	SYS_STATUS_SYS,
    SYS_STATUS_IDEM,//等待测试
    SYS_STATUS_ABORT,//暂停测试
    SYS_STATUS_FINISH,//测试完成
};
//==========================================================
//分选状态
enum SlectStatusEnum
{
	RH_FAIL=0,
	RL_FAIL,
	VH_FAIL,
	VL_FAIL,
	ALL_FAIL,
	ALL_PASS,
//	NO_COMP
	
};
//==========================================================
//系统消息
enum SysMessageEnum
{
	MSG_CLEAR , 
	MSG_IDLE , 
	MSG_TEST , 
	MSG_ABORT , 
	MSG_PASS ,
//	MSG_FAIL ,
	MSG_HIGH , 
	MSG_LOW , 
};

//==========================================================
//最大AD值
#define AD_MAX (3000) //12bit=4096-1
////最大电压AD值
//#define VOL_AD_MAX (AD_MAX-100)
////最大电流AD值
//#define CUR_AD_MAX (AD_MAX-100)

//量程上下限
#define RANGR_LIMIT_HIGH (3000)
#define RANGR_LIMIT_LOW  (190)
extern uint32_t clear_flag;
//==========================================================
//函数列表
void InitGlobalValue(void);//全局变量初始化
void LoadDefaultSet(void);//加载默认设置值
void LoadDefaultParameter(void);//加载默认参数值
void LoadDefaultSystem(void);//加载默认系统值
void LoadDefaultConfig(void);//加载默认配置值
void LoadDefaultCalibrate(void);//加载默认校准值
void ReadSaveData(void);//读取保存参数
//void WriteSaveData(void);//写入保存参数

#define SetSystemStatus(status) (SystemStatus=status)
#define GetSystemStatus() (SystemStatus)
#define GetSystemMessage() (SystemMessage)
#define SetSystemMessage(msg) (SystemMessage=msg)

void Check_Calibrate_Limit(void);//校准值检查
void Hex_Format(vu32 dat , vu8 Dot , vu8 len , vu8 dispzero);


#endif
