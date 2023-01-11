#ifndef __Key_H_
#define __Key_H_
typedef unsigned char u8;
typedef unsigned           int u32;
extern volatile unsigned long SysTickCnt;
#define 	Key_F1  	(9)
#define		Key_F2  	(17)
#define 	Key_F3  	(25)
#define 	Key_F4  	(1)
#define		Key_F5 		(26)
#define		Key_Disp 	 (27)//9
#define		Key_SETUP	 (31)
#define		Key_FAST	(10)//OK
#define		Key_LEFT	(19)//LEFT
#define		Key_RIGHT  	(20)//RIGHT
#define		Key_UP		(11)//UP
#define		Key_DOWN	(12)//DOWN
#define		Key_NUM7	(14)//翻页 从新定义案件名字
#define		Key_NUM8	(13)//
#define		Key_COMP	(13)//
#define		Key_NUM9	(2)
#define		Key_NUM4	(22)//GREEN_LED
#define		Key_NUM5	(50)//退出  从新定义按键
#define		Key_NUM6	(3)
#define		Key_NUM1	(30)
#define		Key_NUM2	(29)
#define		Key_NUM3	(28)
#define		Key_NUM0	(6)
#define		Key_DOT		(5)
#define		Key_BACK	(4)
#define		Key_LOCK	(15)
#define		Key_BIAS	(23)
#define		Key_STOP	(22)//RED_LED
#define		Key_REST	(60)//无
#define		Key_TRIG	(7)
#define		Key_POWER	(55)
#define		Key_PAGE	(14)
#define		Key_START	(23)
#define		Key_VSET	(21)
#define		Key_EXIT	(18)//退出

#define _Hc164CpL GPIO_ClearValue(1, (1<<1))
#define _Hc164CpH GPIO_SetValue(1, (1<<1))
#define _Hc164DL GPIO_ClearValue(1, (1<<10))
#define _Hc164DH GPIO_SetValue(1, (1<<10))
void HW_Sendvalueto164(u8 value);
void HW_keyInt(void);
u8 HW_KeyScsn(void);
void Delay_Key(void);
u8 HW_KeyScsn1(void);
void SysTick_Handler (void);
void Delay (unsigned long tick);
void GPIO_Led_Configuration(void);

void Beep_on(void);
void Beep_Off(void);
void Pass_Led(void);
void Fail_led(void);

void Lock_LedOn(void);
void Lock_LedOff(void);
void Lock_Control_Off(void);
void Lock_Control_On(void);

void Bais_LedOff(void);
void Bais_LedOn(void);

void Beep_Off(void);
void Beep_on(void);

void Pro_PlcOn(void);
void Pro_Plcoff(void);

void Power_Off_led(void);
void Power_On_led(void);
void All_LedOff(void);
void All_LedOff1(void);
	
void Turnoff_backlight(void);
void Turnon_backlight(void);

void Key_Beep(void);

void GPIO_LPC_Configration(void);
#endif

