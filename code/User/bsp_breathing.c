/**
  ******************************************************************************
  * @file    bsp_pwm_output.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   tIM1�����·pwm bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  

/* LED���ȵȼ� PWM�� */
uint8_t indexWave[] = {1,1,2,2,3,4,6,8,10,14,19,25,33,44,59,80,
	107,143,191,255,255,191,143,107,80,59,44,33,25,19,14,10,8,6,4,3,2,2,1,1};

 /**
  * @brief  ����TIM1�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIM1_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

  /* ���ú������õ���PB0���� */
  GPIO_InitStructure.GPIO_Pin =  (GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15) ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Config_PWM(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* ����TIM1_IRQ�ж�Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ����TIM1�����PWM�źŵ�ģʽ�������ڡ�����
  * @param  ��
  * @retval ��
  */

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> �ж� & TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 *
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * 
 */

/*    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 */
static void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
	
	/* ����TIM1CLK ʱ��Ϊ72MHZ */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 					//ʹ��TIM1ʱ��

  /* ������ʱ������ */		 
  TIM_TimeBaseStructure.TIM_Period = 255;       							  //����ʱ����0������255����Ϊ266�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    							//����Ԥ��Ƶ��
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //�߼���ʱ��1���ö�ʱ��������������ſ����������ļ���Ƶ��һ��ʼ��72mhz ֵ��ע��ĵط�
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* PWMģʽ���� */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ�����
  TIM_OCInitStructure.TIM_Pulse = 0;										  			//���ó�ʼPWM������Ϊ0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ

  TIM_OC3Init(TIM1, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
	

  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);						//ʹ��Ԥװ��	

  TIM_ARRPreloadConfig(TIM1, ENABLE);			 										//ʹ��TIM1���ؼĴ���ARR

  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);                   										//ʹ�ܶ�ʱ��3	
	
	TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE);										//ʹ��update�ж�
		
	NVIC_Config_PWM();																					//�����ж����ȼ�		
	
}

/**
  * @brief  TIM1 �����Ƴ�ʼ��
  *         ����PWMģʽ��GPIO
  * @param  ��
  * @retval ��
  */
void TIM1_Breathing_Init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config();	
}

/*********************************************END OF FILE**********************/
