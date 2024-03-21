#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "control.h"
#include "adc.h"
#include "key.h"

extern u16 flag;
extern u16 adcx;

int main(void)
{ 
//	u8 key;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);  
	TIM3_Int_Init(10-1,84-1);
	KEY_Init();
	Adc_Init();
	LED_Init();
	uart_init(115200);    
	while(1) 
	{
//		key=KEY_Scan(1);
//		adc_control(key);
		control_go(flag);
		car_go(flag);
	}
}
