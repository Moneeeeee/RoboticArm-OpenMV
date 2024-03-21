#include "sys.h"
#include "usart.h"	
#include "control.h"
#include "delay.h"

extern u16 flag,m1,motor1,m2,motor2,m3,motor3,m4,motor4;
 extern u16 pwm1,pwm2,pwm3,pwm4,p;
#define FL1 PDout(0)
#define FL2 PDout(1)
#define FR1 PDout(2)
#define FR2 PDout(3)
#define BL1 PDout(4)
#define BL2 PDout(5)
#define BR1 PDout(6)
#define BR2 PDout(7)
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		
		if(Res ==0x01)
		{	m1=limit(m1);
			motor1=m1;
		printf("motor1=%d\n",motor1);
//		delay_ms(500);
		m1+=5;
		}
		else if(Res ==0x02)
		{
			motor2=limit(m2);
		printf("motor2=%d   \n",motor2);
//		delay_ms(500);
		m2+=5;
		}
		
		else if(Res ==0x03)
		{
			motor3=limit(m3);
		printf("motor3=%d   \n",motor3);
//		delay_ms(500);
		m3+=5;
		}
		else if(Res ==0x04)
		{
			motor4=limit_m4(m4);
		printf("motor4=%d   \n",motor4);
//		delay_ms(500);
		m4+=5;
		}
		else if(Res ==0x05)
		{
			m1=limit(m1);
			motor1=m1;
		printf("motor1=%d   \n",motor1);
//		delay_ms(500);
		m1-=5;
		}
		else if(Res ==0x06)
		{
			m2=limit(m2);
			motor2=m2;
		printf("motor2=%d   \n",motor2);
//		delay_ms(500);
		m2-=5;
		}
		else if(Res ==0x07)
		{
			m3=limit(m3);
			motor3=m3;
		printf("motor3=%d   \n",motor3);
//		delay_ms(500);
		m3-=5;
//		flag=19;
		}
		else if(Res ==0x08)
		{
			m4=limit_m4(m4);
			motor4=m4;
		printf("motor4=%d   \n",motor4);
//		delay_ms(500);
		m4-=5;
//		flag=19;
		}
		
		
		else if(Res ==0x09)
		{
		flag=9;
		}
		else if(Res ==0x10)
		{
		flag=10;
		}
		else if(Res ==0x11)
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
		}
		else if(Res ==0x12)
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
		pwm4=p;}
		else if(Res ==0x13)
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
		}
		else if(Res ==0x14)
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
		}
		else if(Res ==0x15)
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
		}
		else if(Res ==0x16)
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
		}
		else if(Res ==0x17)
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
		}
		else if(Res ==0x18)
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
		}
		else if(Res ==0x19)
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
		else if(Res ==0x20)
		{
			p+=100;
		p=limit_pwm(p);
		}
		else if(Res ==0x21)
		{
		p-=100;
		p=limit_pwm(p);
		}

		
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

 



