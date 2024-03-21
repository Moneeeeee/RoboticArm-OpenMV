#include "pwm.h"
#include "usart.h"
#include "led.h"

u16 timer;
extern u16 motor1,motor2,motor3,motor4, pwm1,pwm2,pwm3,pwm4;

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		timer++;		
		if(timer==2000)//20ms
		timer=0;
		
		if(timer<motor1)
		{PCout(6)=1;}
		else if(timer>motor1)
		{PCout(6)=0;}
		
		if(timer<motor2)
		{PCout(7)=1;}
		else if(timer>motor2)
		{PCout(7)=0;}
		
		if(timer<motor3)
		{PCout(8)=1;}
		else if(timer>motor3)
		{PCout(8)=0;}
		
		if(timer<motor4)
		{PCout(9)=1;}
		else if(timer>motor4)
		{PCout(9)=0;}
		
		if(timer<pwm1)
		{PCout(2)=1;}
		else if(timer>pwm1)
		{PCout(2)=0;}
		
		if(timer<pwm2)
		{PCout(3)=1;}
		else if(timer>pwm2)
		{PCout(3)=0;}
		
		if(timer<pwm3)
		{PCout(4)=1;}
		else if(timer>pwm3)
		{PCout(4)=0;}
		
		if(timer<pwm4)
		{PCout(5)=1;}
		else if(timer>pwm4)
		{PCout(5)=0;}
		
	}
	TIM_ClearITPendingBit(TIM3 ,TIM_IT_Update);  //����жϱ�־λ
}


