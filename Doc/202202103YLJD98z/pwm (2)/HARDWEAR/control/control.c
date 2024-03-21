#include "pwm.h"
#include "control.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "led.h"

u16 flag;
 u16 motor1=150,motor2=100,motor3=190,motor4=143,m1=150,m2=100,m3=190,m4=143;
 u16 pwm1=1800,pwm2=1800,pwm3=1800,pwm4=1800,p=1800;

#define FL1 PDout(0)
#define FL2 PDout(1)
#define FR1 PDout(2)
#define FR2 PDout(3)
#define BL1 PDout(4)
#define BL2 PDout(5)
#define BR1 PDout(6)
#define BR2 PDout(7)

extern u16 adcx;
/////////////pwm范围:246-53 180度///////
u16 limit(u16 MOTOR)
{
	if(MOTOR<=53)
		MOTOR=53;
	else if(MOTOR>=246)
		MOTOR=246;
	else MOTOR=MOTOR;
	return MOTOR;
}

u16 limit_m4(u16 MOTOR)
{
	if(MOTOR<=143)
		MOTOR=143;
	else if(MOTOR>=198)
		MOTOR=198;
	else MOTOR=MOTOR;
	return MOTOR;
}

u16 limit_pwm(u16 pwm)
{
	if(pwm<=500)
		pwm=500;
	else if(pwm>=1800)
		pwm=1800;
	else pwm=pwm;
	return pwm;
}

void control_go(u16 FLAG)
{	
	if(FLAG==1)
	{
		motor1=limit(m1);
		printf("motor1=%d\n",motor1);
//		delay_ms(500);
		m1+=5;
		flag=19;
	}
	else if(FLAG==2)
	{
		motor2=limit(m2);
		printf("motor2=%d   \n",motor2);
//		delay_ms(500);
		m2+=5;
//		flag=19;
	}
	else if(FLAG==3)
	{
		motor3=limit(m3);
		printf("motor3=%d   \n",motor3);
//		delay_ms(500);
		m3+=5;
//		flag=19;
	}
	else if(FLAG==4)
	{
		motor4=limit_m4(m4);
		printf("motor4=%d   \n",motor4);
//		delay_ms(500);
		m4+=5;
//		flag=19;
	}
	
	
	else if(FLAG==5)
	{
		motor1=limit(m1);
		printf("motor1=%d   \n",motor1);
//		delay_ms(500);
		m1-=5;
//		flag=19;
	}
	else if(FLAG==6)
	{
		motor2=limit(m2);
		printf("motor2=%d   \n",motor2);
//		delay_ms(500);
		m2-=5;
//		flag=19;
	}
	else if(FLAG==7)
	{
		motor3=limit(m3);
		printf("motor3=%d   \n",motor3);
//		delay_ms(500);
		m3-=5;
//		flag=19;
	}
	else if(FLAG==8)
	{
		motor4=limit_m4(m4);
		printf("motor4=%d   \n",motor4);
//		delay_ms(500);
		m4-=5;
//		flag=19;
	}
	
}
	
void car_go(u16 FLAG)
{
	printf("MOTO=%d\n",FLAG);
	if(FLAG==9)         //前进
	{
		FL1=1;
		FL2=0;
		FR1=1;
		FR2=0;
		BL1=1;
		BL2=0;
		BR1=1;
		BR2=0;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==10)         //后退
	{
		FL1=0;
		FL2=1;
		FR1=0;
		FR2=1;
		BL1=0;
		BL2=1;
		BR1=0;
		BR2=1;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==11)         //原地左
	{
		FL1=1;
		FL2=0;
		FR1=0;
		FR2=1;
		BL1=1;
		BL2=0;
		BR1=0;
		BR2=1;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==12)         //原地右
	{
		FL1=0;
		FL2=1;
		FR1=1;
		FR2=0;
		BL1=0;
		BL2=1;
		BR1=1;
		BR2=0;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==13)         //平移左
	{
		FL1=0;
		FL2=1;
		FR1=1;
		FR2=0;
		BL1=1;
		BL2=0;
		BR1=0;
		BR2=1;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==14)         //平移右
	{
		FL1=1;
		FL2=0;
		FR1=0;
		FR2=1;
		BL1=0;
		BL2=1;
		BR1=1;
		BR2=0;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==15)         //左上
	{
		FL1=1;
		FL2=0;
		FR1=0;
		FR2=0;
		BL1=0;
		BL2=0;
		BR1=1;
		BR2=0;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==16)         //右上
	{
		FL1=0;
		FL2=0;
		FR1=1;
		FR2=0;
		BL1=1;
		BL2=0;
		BR1=0;
		BR2=0;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==17)         //左下
	{
		FL1=0;
		FL2=0;
		FR1=0;
		FR2=1;
		BL1=0;
		BL2=1;
		BR1=0;
		BR2=0;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==18)         //右下
	{
		FL1=0;
		FL2=1;
		FR1=0;
		FR2=0;
		BL1=0;
		BL2=0;
		BR1=0;
		BR2=1;
		pwm1=p;
		pwm2=p;
		pwm3=p;
		pwm4=p;
//		flag=19;
	}
	if(FLAG==19)         //右下
	{
		FL1=0;
		FL2=0;
		FR1=0;
		FR2=0;
		BL1=0;
		BL2=0;
		BR1=0;
		BR2=0;
		pwm1=0;
		pwm2=0;
		pwm3=0;
		pwm4=0;
		flag=19;
	}
	if(FLAG==20) 
	{
		p+=100;
		p=limit_pwm(p);
	}
	if(FLAG==21) 
	{
		p-=100;
		p=limit_pwm(p);
	}
	
}



