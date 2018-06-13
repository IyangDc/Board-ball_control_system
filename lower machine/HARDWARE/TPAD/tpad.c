#include "tpad.h"
#include "delay.h"		    
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32F103������V1
//TPAD��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/11
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20150331
//�޸���TPAD_Scan,��߳��򿹸�����
//////////////////////////////////////////////////////////////////////////////////

#define TPAD_ARR_MAX_VAL 	0XFFFF	//����ARRֵ	  
vu16 tpad_default_val=0;//���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��
//��ʼ����������
//��ÿ��ص�ʱ����������ȡֵ.
//psc:����ϵ��(ֵԽС,Խ����,��СֵΪ:1)
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
u8 TPAD_Init(u8 psc)
{
	u16 buf[10];
	u16 temp;
	u8 j,i;
	TIM5_CH2_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);//��1Mhz��Ƶ�ʼ��� 
	for(i=0;i<10;i++)//������ȡ10��
	{				 
		buf[i]=TPAD_Get_Val();
		delay_ms(10);	    
	}				    
	for(i=0;i<9;i++)//����
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)temp+=buf[i];//ȡ�м��8�����ݽ���ƽ��
	tpad_default_val=temp/6;
	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	if(tpad_default_val>TPAD_ARR_MAX_VAL/2)return 1;//��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������!
	return 0;		     	    					   
}
//��λһ��
//�ͷŵ��ݵ������������ʱ���ļ���ֵ
void TPAD_Reset(void)
{				   
	GPIOA->CRL&=0XFFFFFF0F;	//PA1  ����  
	GPIOA->CRL|=0X00000030;	//�������       
	GPIOA->ODR&=~(1<<1);	//���0,�ŵ�
	delay_ms(5);
	TIM5->SR=0;   			//������
	TIM5->CNT=0;			//����     
	GPIOA->CRL&=0XFFFFFF0F;	//PA1  ����  
	GPIOA->CRL|=0X00000040;	//��������      
}
//�õ���ʱ������ֵ
//�����ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ.
//����ֵ������ֵ/����ֵ����ʱ������·��أ�
u16 TPAD_Get_Val(void)
{				   
	TPAD_Reset();
	while((TIM5->SR&0X04)==0)//�ȴ�����������
	{
		if(TIM5->CNT>TPAD_ARR_MAX_VAL-500)return TIM5->CNT;//��ʱ��,ֱ�ӷ���CNT��ֵ
	};	
	return TIM5->CCR2;	  
} 	  
//��ȡn��,ȡ���ֵ
//n��������ȡ�Ĵ���
//����ֵ��n�ζ������������������ֵ
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();//�õ�һ��ֵ
		if(temp>res)res=temp;
	};
	return res;
}  
//ɨ�败������
//mode:0,��֧����������(����һ�α����ɿ����ܰ���һ��);1,֧����������(����һֱ����)
//����ֵ:0,û�а���;1,�а���;										  
#define TPAD_GATE_VAL 	100	//����������ֵ,Ҳ���Ǳ������tpad_default_val+TPAD_GATE_VAL,����Ϊ����Ч����.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;	//0,���Կ�ʼ���;>0,�����ܿ�ʼ���	 
	u8 res=0;
	u8 sample=3;		//Ĭ�ϲ�������Ϊ3��	 
	u16 rval;
	if(mode)
	{
		sample=6;		//֧��������ʱ�����ò�������Ϊ6��
		keyen=0;		//֧������	  
	}
	rval=TPAD_Get_MaxVal(sample); 
	if(rval>(tpad_default_val+TPAD_GATE_VAL))//����tpad_default_val+TPAD_GATE_VAL,��Ч
	{							 
		if(keyen==0)res=1;		//keyen==0,��Ч 
		//printf("r:%d\r\n",rval);		     	    					   
		keyen=3;				//����Ҫ�ٹ�3��֮����ܰ�����Ч   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	 
//��ʱ��2ͨ��2���벶������	 
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{
	//�˲������ֶ��޸� IO������  
 	RCC->APB1ENR|=1<<3;   	//TIM5 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ�� 

	GPIOA->CRL&=0XFFFFFF0F;	//PA1  ����  
	GPIOA->CRL|=0X00000040;	//��������     	  
	  
 	TIM5->ARR=arr;  		//�趨�������Զ���װֵ//�պ�1ms    
	TIM5->PSC=psc;  		//Ԥ��Ƶ��,1M�ļ���Ƶ��	 

	TIM5->CCMR1|=1<<8;		//CC2S=01 	ѡ������� IC2ӳ�䵽TI2��
 	TIM5->CCMR1|=0<<12; 	//IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
 	TIM5->CCMR1|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 

	TIM5->CCER|=0<<5; 		//CC2P=0	�����ز���
	TIM5->CCER|=1<<4; 		//CC2E=1 	�������������ֵ������Ĵ�����
																 
	TIM5->CR1|=0x01;    	//ʹ�ܶ�ʱ��5
}













