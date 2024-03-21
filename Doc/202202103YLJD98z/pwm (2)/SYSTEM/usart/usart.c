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
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		
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

		
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif	

 



