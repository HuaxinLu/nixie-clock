/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "DS3231.h"
#include "bsp_i2c_gpio.h"
#include "bsp_breathing.h" 
#include "bsp_usart1.h"

#define HV57708_CLK_H  GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define HV57708_CLK_L  GPIO_ResetBits(GPIOA,GPIO_Pin_3)

#define HV57708_LE_H   GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define HV57708_LE_L   GPIO_ResetBits(GPIOA,GPIO_Pin_4)

#define HV57708_DI1_H  GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define HV57708_DI1_L  GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define HV57708_DI2_H  GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define HV57708_DI2_L  GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define HV57708_DI3_H  GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define HV57708_DI3_L  GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define HV57708_DI4_H  GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define HV57708_DI4_L  GPIO_ResetBits(GPIOA,GPIO_Pin_5)
int time0,time1,time2,time3;
unsigned char key1_flag,key2_flag,key3_flag;
long test_number;
unsigned char LEDStyle;
unsigned char mode;
unsigned char setnumber;
unsigned char numberblink;
unsigned char numbers[][10] = {
  {10,   1,  2,  3,  4,  5,  6,  7,  8,  9},
  {20, 11, 12, 13, 14, 15, 16, 17, 18, 19},
  {30, 21, 22, 23, 24, 25, 26, 27, 28, 29},
  {44, 35, 36, 37, 38, 39, 40, 41, 42, 43},
  {54, 45, 46, 47, 48, 49, 50, 51, 52, 53},
  {64, 55, 56, 57, 58, 59, 60, 61, 62, 63},
};
int Offset[6]={0};
unsigned char indexWave[] = {1,1,2,2,3,4,6,8,10,14,19,25,33,44,59,80,
	107,143,191,255,255,191,143,107,80,59,44,33,25,19,14,10,8,6,4,3,2,2,1,1};
unsigned long protect[10]={862552,973663,84774,195885,206996,317007,428118,539229,640330,751441};
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
void HV57708_Init()
{
  HV57708_CLK_L;
  HV57708_LE_H;
  HV57708_DI1_L;
  HV57708_DI2_L;
  HV57708_DI3_L;
  HV57708_DI4_L;
}
void HV57708_SendData(unsigned long datapart1, unsigned long datapart2)
{
  unsigned char i = 0;
  unsigned long tmp = 0;
  tmp = datapart1;
  for(i = 0; i < 8; i++)
  {
    HV57708_CLK_L;
    if(tmp&0x00000001) 
      HV57708_DI1_H;
    else 
      HV57708_DI1_L;
    tmp=tmp>>1;
    if(tmp&0x00000001) 
      HV57708_DI2_H;
    else 
      HV57708_DI2_L;    
    tmp=tmp>>1;
    if(tmp&0x00000001) 
      HV57708_DI3_H;
    else 
      HV57708_DI3_L;   
    tmp=tmp>>1;
    if(tmp&0x00000001) 
      HV57708_DI4_H;
    else 
      HV57708_DI4_L;   
    tmp=tmp>>1;
	HV57708_CLK_H;
    __nop();__nop();__nop();__nop();
    HV57708_CLK_L;
    __nop();__nop();__nop();__nop();
  }
  tmp = datapart2;
  for(i = 0; i < 8; i++)
  {
    HV57708_CLK_L;
    if(tmp&0x00000001) HV57708_DI1_H;
    else HV57708_DI1_L;
    tmp=tmp>>1;
    if(tmp&0x00000001) HV57708_DI2_H;
    else HV57708_DI2_L;    
    tmp=tmp>>1;
    if(tmp&0x00000001) HV57708_DI3_H;
    else HV57708_DI3_L;   
    tmp=tmp>>1;
    if(tmp&0x00000001) HV57708_DI4_H;
    else HV57708_DI4_L;   
    tmp=tmp>>1;
	HV57708_CLK_H;
    __nop();__nop();__nop();__nop();
    HV57708_CLK_L;
    __nop();__nop();__nop();__nop();
  }  
}

void HV57708_OutputData(void)
{
  HV57708_LE_L;
     __nop();__nop();__nop();__nop(); 
  HV57708_LE_H;
    __nop();__nop();__nop();__nop();
}


void displayNumber(unsigned long number)
{
  unsigned long data_part2  = 0;
  unsigned long data_part1  = 0;
  unsigned char tmp_number = 0;
  int temp;	
  //tmp_number = numbers[0][number / 100000ul];
  //data_part1 |= 1ul << tmp_number;
	
  //tmp_number = numbers[1][(number % 100000ul) / 10000ul];
  //data_part1 |= 1ul << tmp_number;
  
  if(numberblink==1&&(setnumber==2||setnumber==5))
  {

  }
  else
  {	   
	  temp = numbers[0][number / 100000ul];
	  tmp_number=((temp-1)/4+1)*8-4-temp;
	  if(tmp_number < 32)
	    data_part1 |= 1ul << tmp_number;
	  else
	    data_part2 |= 1ul << (tmp_number - 32);
			
	  temp = numbers[1][(number % 100000ul) / 10000ul];
	  tmp_number=((temp-1)/4+1)*8-4-temp;
	  if(tmp_number < 32)
	    data_part1 |= 1ul << tmp_number;
	  else
	    data_part2 |= 1ul << (tmp_number - 32);    	  	
  }


  if(numberblink==1&&(setnumber==1||setnumber==4))
  {

  }
  else
  {		 
	  temp = numbers[2][(number % 10000ul) / 1000ul];
	  tmp_number=((temp-1)/4+1)*8-4-temp;
	  if(tmp_number < 32)
	    data_part1 |= 1ul << tmp_number;
	  else
	    data_part2 |= 1ul << (tmp_number - 32);  
	
	  temp = numbers[3][(number % 1000ul) / 100ul];
	  tmp_number=((temp-1)/4+1)*8-4-temp;
	  if(tmp_number < 32)
	    data_part1 |= 1ul << tmp_number;
	  else
	    data_part2 |= 1ul << (tmp_number - 32);  
  }

  if(numberblink==1&&(setnumber==0||setnumber==3))
  {

  }
  else
  {									 
	  temp = numbers[4][(number % 100ul) / 10ul];
	  tmp_number=((temp-1)/4+1)*8-4-temp;
	  if(tmp_number < 32)
	    data_part1 |= 1ul << tmp_number;
	  else
	    data_part2 |= 1ul << (tmp_number - 32);
		
		temp = numbers[5][(number % 10ul)];
	  tmp_number=((temp-1)/4+1)*8-4-temp;
	  if(tmp_number < 32)
	    data_part1 |= 1ul << tmp_number;
	  else
	    data_part2 |= 1ul << (tmp_number - 32);    	

  }		
	
  HV57708_SendData(data_part1, data_part2);
  HV57708_OutputData();
}
/*void PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           

	TIM_TimeBaseStructure.TIM_Prescaler = 4000;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;     
	TIM_OCInitTypeDef TIM_OCInitStructure;         
	TIM_DeInit(TIM1); 
	TIM_TimeBaseStructure.TIM_Prescaler = 4000;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_Period = 144;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
}*/
void TIM1_Mode_Config() 
{ 
    TIM_OCInitTypeDef TIM_OCInitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure; 
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure; 
     
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE); 
     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
     
     
    TIM_TimeBaseStructure.TIM_Period    = 1000;//ARR 10K 
    TIM_TimeBaseStructure.TIM_Prescaler = 18;//每计一个数为1us 
    TIM_TimeBaseStructure.TIM_ClockDivision     = 0; 
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000; 
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
     
    //TIM_ARRPreloadConfig(TIM1, DISABLE); //预装载寄存器的内容被立即传送到影子寄存器 
     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = 1000;//CCR 
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High; 
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High; 
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set; 
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; 
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);      
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);      
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); 
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);      
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); 
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable; 
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable; 
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF; 
    TIM_BDTRInitStructure.TIM_DeadTime = 0;  //这里调整死区大小0-0xff 
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable; 
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High; 
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; 
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure); 
    //TIM_OC1NPolarityConfig(TIM1, TIM_OCNPolarity_High); 
    //TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_Low); 
     
    //TIM_CCPreloadControl(TIM1, ENABLE); 
    TIM_Cmd(TIM1, ENABLE); 
    TIM_CtrlPWMOutputs(TIM1, ENABLE); 
}
/// TIM2中断优先级配置
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* 设置TIM2CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    //TIM_DeInit(TIM2);
	
	/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period=10000;
	
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
	  /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*先关闭等待使用*/    
}


void PWM_Control(unsigned int led)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//ch1 配置项   
	TIM_OCInitStructure.TIM_Pulse = led;   
	TIM_OC1Init(TIM2, & TIM_OCInitStructure); //ch1占空比75%  
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能的预装载寄存器 

}
int temp1;
char flag;
char color;	
void breath_led()
{
	if(color==0)  
	{
	 	TIM1->CCR1=temp1;
		TIM1->CCR2=1001;
		TIM1->CCR3=1001;
	}
	else if(color==1)  
	{
	 	TIM1->CCR1=1001;
		TIM1->CCR2=temp1;
		TIM1->CCR3=1001;
	}
	else if(color==2)  
	{
	 	TIM1->CCR1=1001;
		TIM1->CCR2=1001;
		TIM1->CCR3=temp1;
	}
}
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口（PC）的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Key_Scan()
{
	int i;
	if(key1_flag==0)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 0)
			key1_flag=1;
		else 
			key1_flag=0;
	}
	if(key1_flag==1)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 0)
			key1_flag=2;
		else 
			key1_flag=0;
	}
	if(key1_flag==2)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1)
			key1_flag=3;
		else 
			key1_flag=2;
	}
	if(key1_flag==3)
	{
		if(mode==0)
		{
			LEDStyle++;
			if(LEDStyle>15)
				LEDStyle=0;
		}
		if(mode==1)
			Offset[setnumber]++;
		key1_flag=0;
	}
	
	if(key2_flag==0)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0)
			key2_flag=1;
		else 
			key2_flag=0;
	}
	if(key2_flag==1)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0)
			key2_flag=2;
		else 
			key2_flag=0;
	}
	if(key2_flag==2)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 1)
			key2_flag=3;
		else 
			key2_flag=2;
	}
	if(key2_flag==3)
	{
		if(mode==1)
		{
			setnumber++;
			if(setnumber>5)
				setnumber=0;
		}
		key2_flag=0;
	}
	
	if(key3_flag==0)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0)
			key3_flag=1;
		else 
			key3_flag=0;
	}
	if(key3_flag==1)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0)
			key3_flag=2;
		else 
			key3_flag=0;
	}
	if(key3_flag==2)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 1)
			key3_flag=3;
		else 
			key3_flag=2;
	}
	if(key3_flag==3)
	{
		if(mode==0)
			mode=1;
		else if(mode==1)
		{
			mode=0;
			DS3231_Set(calendar.w_year+Offset[5],calendar.w_month+Offset[4],calendar.w_date+Offset[3],calendar.hour+Offset[2],calendar.min+Offset[1],calendar.sec+Offset[0]); 
			for(i=0;i<6;i++)
				Offset[i]=0;			
			setnumber=0;
		}

			
		key3_flag=0;
	}		
}
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	
	char last_second;
	unsigned char i=0;
	
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

AFIO->MAPR = (0x00FFFFFF & AFIO->MAPR)|0x04000000;          //关闭JTAG 



	/*开启GPIOA和GPIOB的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE); 
	/*选择要控制的GPIOA引脚*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	/*设置引脚模式为开漏输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOB0*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	/*选择要控制的GPIOA引脚*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
	/*设置引脚模式为开漏输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOB0*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	/*选择要控制的GPIOA引脚*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	/*设置引脚模式为开漏输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOB0*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	
	/*选择要控制的GPIOB引脚*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	/*设置引脚模式为开漏输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOB0*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	/*选择要控制的GPIOB引脚*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	/*设置引脚模式为开漏输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOB0*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*选择要控制的GPIOA引脚*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	/*设置引脚模式为开漏输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOA*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*选择要控制的GPIOB引脚*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	/*设置引脚模式为开漏输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOB0*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	Key_GPIO_Config();
	HV57708_Init();
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);

	DS3231_Init();

	USART1_Config();
	NVIC_Configuration();

	//DS3231_Set(17,6,24,1,57,0);
	//TIM1_Mode_Config();
	//TIM_SetAutoreload(TIM1,1000); 
	/* TIM2 定时配置 */	
    TIM2_Configuration();
	/* 实战定时器的中断优先级 */
	TIM2_NVIC_Configuration();
	/* TIM2 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
		USART1_Config();
	NVIC_Configuration();
	while(1)
	{
		  /*HV57708_CLK_L;
		  HV57708_LE_H;
		  HV57708_DI1_H;
		  HV57708_DI2_H;
		  HV57708_DI3_H;
		  HV57708_DI4_L;

		  HV57708_CLK_L;
		  HV57708_LE_L;
		  HV57708_DI1_L;
		  HV57708_DI2_L;
		  HV57708_DI3_L;
		  HV57708_DI4_L; */
		if(time0>=4)//40ms任务
		{
			time0=0;
			//Key_Scan();
		}
		if(time1>=5)//50ms任务
		{
			//displayNumber(234567);
			time1=0;
			if(mode==0)
			{
				get_show_time();
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0)
				{
				   displayNumber(calendar.w_date+calendar.w_month*100+calendar.w_year*10000);
				}
					
				else
				{
				    if(last_second!=calendar.sec)
					{
						displayNumber(calendar.sec+calendar.min*100+calendar.hour*10000);
					}
						
					last_second = calendar.sec;
					if(calendar.sec==29||calendar.sec==59)
					{
						displayNumber(protect[i]);
						i++;
						if(i>10)
							i=0; 
					}						
				}
				
			}
			else if(mode==1)
			{
				get_show_time();
				if((calendar.sec+Offset[0])>59)   Offset[0]=Offset[0]-60;
				if((calendar.sec+Offset[0])<0)   Offset[0]=Offset[0]+60;
				if((calendar.min+Offset[1])>59)   Offset[1]=Offset[1]-60;
				if((calendar.min+Offset[1])<0)   Offset[1]=Offset[1]+60;
				if((calendar.hour+Offset[2])>23)   Offset[2]=Offset[2]-24;
				if((calendar.hour+Offset[2])<0)   Offset[2]=Offset[2]+24;
				if((calendar.w_date+Offset[3])>31)   Offset[3]=Offset[3]-32;
				if((calendar.w_date+Offset[3])<0)   Offset[3]=Offset[3]+32;
				if((calendar.w_month+Offset[4])>12)   Offset[4]=Offset[4]-12;
				if((calendar.w_month+Offset[4])<1)   Offset[4]=Offset[4]+12;
				if((calendar.w_year+Offset[5])>50)   Offset[5]=Offset[5]-34;
				if((calendar.w_year+Offset[5])<17)   Offset[5]=Offset[5]+34;
				if(setnumber<=2)
					displayNumber(calendar.sec+Offset[0]+(calendar.min+Offset[1])*100+(calendar.hour+Offset[2])*10000); 
				else
					displayNumber(calendar.w_date+Offset[3]+(calendar.w_month+Offset[4])*100+(calendar.w_year+Offset[5])*10000); 		
			}
	
		}
		if(time2>=8)//120ms任务
		{
			time2=0;
			if(flag)
				temp1=temp1+10;
			else 
				temp1=temp1-10;
			if(temp1<820) 
			{
				temp1=820;
				flag=1;
			}		
			if(temp1>=1001)	
			{
				temp1=1001;
				flag=0;
				color++;
				if(color>=3)
					color=0;
			}
			switch(LEDStyle)
			{ 
				case 0 : breath_led();break;
				case 1 : TIM1->CCR1=temp1;TIM1->CCR2=1001;TIM1->CCR3=1001;break;
				case 2 : TIM1->CCR1=1001;TIM1->CCR2=temp1;TIM1->CCR3=1001;break;
				case 3 : TIM1->CCR1=1001;TIM1->CCR2=1001;TIM1->CCR3=temp1;break;
				case 4 : TIM1->CCR1=temp1;TIM1->CCR2=temp1;TIM1->CCR3=1001;break;
				case 5 : TIM1->CCR1=temp1;TIM1->CCR2=1001;TIM1->CCR3=temp1;break;
				case 6 : TIM1->CCR1=1001;TIM1->CCR2=temp1;TIM1->CCR3=temp1;break;
				case 7 : TIM1->CCR1=temp1;TIM1->CCR2=temp1;TIM1->CCR3=temp1;break;
				case 8 : TIM1->CCR1=820;TIM1->CCR2=1001;TIM1->CCR3=1001;break;
				case 9 : TIM1->CCR1=1001;TIM1->CCR2=820;TIM1->CCR3=1001;break;
				case 10 : TIM1->CCR1=1001;TIM1->CCR2=1001;TIM1->CCR3=820;break;
				case 11 : TIM1->CCR1=820;TIM1->CCR2=820;TIM1->CCR3=1001;break;
				case 12 : TIM1->CCR1=820;TIM1->CCR2=1001;TIM1->CCR3=820;break;
				case 13 : TIM1->CCR1=1001;TIM1->CCR2=820;TIM1->CCR3=820;break;
				case 14 : TIM1->CCR1=820;TIM1->CCR2=820;TIM1->CCR3=820;break;	
				case 15 : TIM1->CCR1=1001;TIM1->CCR2=1001;TIM1->CCR3=1001;break;				
			} 
			
		}
		if(time3>=50)//500ms任务
		{
			time3=0;
			if(mode==1)
				numberblink=1-numberblink;
			else
				numberblink=0;				
		} 
   	 
	}
	  /* add your code here ^_^. */
}

/*********************************************END OF FILE**********************/

