#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	   
#include "timer.h"  
//ALIENTEK��ӢSTM32F103������ ʵ��44
//USB������ ʵ�� 
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
#define spa 0x00
#define working 0x11
void space_duty(void);
u8 flag = spa;
u16 Sevx = 0;
u16 Sevy = 0;
u16 X=538;
u16 Y=559;
void state1(u16 duty);
void pwmchange(u16 x_,u16 y_);
int main(void)
{		
	u16 k=618,len=0,x_h=0,x_l=0,y_h=0,y_l=0;
	Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,115200);	//���ڳ�ʼ��Ϊ115200
	delay_init(72);	   	 	//��ʱ��ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
  TIM4_PWM_Init(999,217);	//����Ƶ��PWMƵ��=72000/(1000*218)=330hz
	//PWM�ź���Ч��Χ�� %~ %
	//��ӦCCR��Χ�� ~ 
	TIM4_PWM_Open();
//	while(1)
//	{
//		state1(111);
//	

//	}
	while(1)
	{
//����ͨ��
		if(USART_RX_STA&0x8000){
			//flag=working;					   
			len= USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
			x_h = USART_RX_BUF[1];
			y_h = USART_RX_BUF[2];
			if(x_h>=20)x_h=20;
			if(y_h>=20)y_h=20;
			pwmchange(x_h,y_h);
//			if (x_h==3)
//			{
//				state1(0);
//			}
			//�ɽǶȼ���ռ�ձ�
			USART_RX_STA=0;
			//state1(0);
		}
    
		//state1(0);
	}
}

		//state1(0);
//����ͨ��
//		if(USART_RX_STA&0x8000){
//			//flag=working;					   
//			len = USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
////			x_h |= USART_RX_BUF[2];
////			x_l |= USART_RX_BUF[3];
////			y_h |= USART_RX_BUF[4];
////			y_l |= USART_RX_BUF[5];
//			//�ɽǶȼ���ռ�ձ�
//			USART_RX_STA=0;
////			x_h = x_h * 4 + 605;
//			state1(0);
////			if(len==6)
////				state1(0);
////				state1(0);
////				state1(0);
////			state1(x_l * 4 + 605);
////			state1(y_h * 4 + 605);
//		}



////�ĽǶ�
//pwmchange(5,5);
//		delay_ms(500);
//		pwmchange(15,5);
//		delay_ms(500);
//		pwmchange(15,15);
//		delay_ms(500);
//		pwmchange(5,15);
//		delay_ms(500);



void state1(u16 duty)
{
	if(duty>=625)
	{
		duty = 625;
	}
	if(duty<=585)
	{
		duty=585;
	}
	TIM4->CCR1 = 554;
	delay_ms(1000);delay_ms(1000);delay_ms(1000);
	TIM4->CCR1 = 544;
	delay_ms(1000);delay_ms(1000);delay_ms(1000);
	TIM4->CCR1 = 514;
	delay_ms(1000);delay_ms(1000);delay_ms(1000);
	TIM4->CCR1 = 544;
	delay_ms(1000);delay_ms(1000);delay_ms(1000);
}
void pwmchange(u16 x_,u16 y_)
{
	int X_in=0,Y_in=0,temp_x=0,temp_y=0;
	if(x_ >= 20)
		x_ = 20;
	if(y_ >= 20)
		y_ = 20;
	temp_x = x_ - 10;
	if(temp_x > 0)
		temp_x *= 1.2;
	if(temp_x > 0)
		temp_x *= 0.9 ;
//		temp_x -= 5;
//	}
	temp_y = y_ - 10;
	//if(temp_y > 0)
		//temp_y *= 1.3 ;
//		temp_y -= 5;
//  }
	X_in = X - temp_x;
	Y_in = Y - temp_y;
	TIM4->CCR1 = X_in;
	TIM4->CCR2 = Y_in;
}


//ˮƽ	
//	while(1){
//			//delay_ms(300);
//			//TIM4->CCR2 = 790;
//			//TIM4->CCR2 = 615+20;//xx
//			delay_ms(1200);
//			TIM4->CCR2 = 610;//sx��������
//			TIM4->CCR1 = 605;
//		
//		}


////����ͨ��
//		if(USART_RX_STA&0x8000){
//			//flag=working;					   
//			len= USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
//			x_h= USART_RX_BUF[2];
//			x_l= USART_RX_BUF[3];
//			y_h= USART_RX_BUF[4];
//			y_l= USART_RX_BUF[5];
//			//�ɽǶȼ���ռ�ձ�
//			USART_RX_STA=0;
//			state1(x_h * 4 + 605);
//			state1(x_l * 4 + 605);
//			state1(y_h * 4 + 605);
//		}







//int main(void)
//{		
//	Stm32_Clock_Init(9);	//ϵͳʱ������
//	uart_init(72,115200);	//���ڳ�ʼ��Ϊ115200
//	delay_init(72);	   	 	//��ʱ��ʼ�� 
//	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
//  TIM3_PWM_Init(999,217);	//����Ƶ��PWMƵ��=72000/(1000*218)=330hz
//	//PWM�ź���Ч��Χ�� %~ %
//	//��ӦCCR��Χ�� ~ 

//  while(1){
//		switch(flag){
//			case spa:space_duty();break;
//			case working:	Sevx_PWM_VAL = Sevx;
//										Sevy_PWM_VAL = Sevy;
//										flag=spa;break;
//		}
//	}
//}


//void space_duty(void)
//{
//	u8 x_h=0,x_l=0,y_h=0,y_l=0;
//	u8 len=0;
//	if(USART_RX_STA&0x8000){
//		flag=working;					   
//		len=USART_RX_STA&0x3FFF;//�õ��˴ν��յ������ݳ���
//		
//		x_h= USART_RX_BUF[0];
//		x_l= USART_RX_BUF[1];x
//		y_h= USART_RX_BUF[2];
//		y_l= USART_RX_BUF[3];
//		//�ɽǶȼ���ռ�ձ�
//		Sevx = ;
//		Sevy = ;
//		USART_RX_STA=0;
//	}














