#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
#include "ov7670.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32F103开发板V1
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/10
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved		
//********************************************************************************
//V1.1 20150118
//1,增加EXTI8_Init函数。
//2,增加EXTI9_5_IRQHandler函数											  
//////////////////////////////////////////////////////////////////////////////////  

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(WK_UP==1)	//WK_UP按键
	{				 
		BEEP=!BEEP;	
	}		 
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
} 
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(KEY1==0)	 	//按键KEY1
	{				 
		LED1=!LED1;
	}		 
	EXTI->PR=1<<3;  //清除LINE3上的中断标志位  
}
//外部中断4服务程序
void EXTI4_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(KEY0==0)	 	//按键KEY0
	{
		LED0=!LED0;
		LED1=!LED1;
	}		 
	EXTI->PR=1<<4;  //清除LINE4上的中断标志位  
}		   
//外部中断初始化程序
//初始化PA0/PE3/PE4为中断输入.
void EXTIX_Init(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_A,0,RTIR); 	//上升沿触发 
	Ex_NVIC_Config(GPIO_E,3,FTIR); 	//下降沿触发
 	Ex_NVIC_Config(GPIO_E,4,FTIR); 	//下降沿触发
	MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//抢占2，子优先级3，组2  
	MY_NVIC_Init(2,1,EXTI3_IRQn,2);	//抢占2，子优先级1，组2	   
	MY_NVIC_Init(2,0,EXTI4_IRQn,2);	//抢占2，子优先级0，组2	   
}
u8 ov_sta;	//帧中断标记
 //外部中断5~9服务程序
void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI->PR&(1<<8))		//是8线的中断
	{      
		if(ov_sta==0)
		{
			OV7670_WRST=0;	//复位写指针		
			OV7670_RCK_L;
			OV7670_RCK_H;  		 
			OV7670_WRST=1;	
			OV7670_WREN=1;	//允许写入FIFO 	 
			ov_sta++;		//帧中断加1 
		}else OV7670_WREN=0;//禁止写入FIFO 	 
	}
	EXTI->PR=1<<8;			//清除LINE8上的中断标志位						  
} 
//外部中断8初始化
void EXTI8_Init(void)
{												  
	Ex_NVIC_Config(GPIO_A,8,RTIR); 			//任意边沿触发			  
	MY_NVIC_Init(0,0,EXTI9_5_IRQn,2);		//抢占0,子优先级0，组2	   
}











