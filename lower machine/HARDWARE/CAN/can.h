#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32F103������V1
//CAN���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/1/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//CAN����RX0�ж�ʹ��
#define CAN_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
u8 CAN_Tx_Msg(u32 id,u8 ide,u8 rtr,u8 len,u8 *dat);		//��������
u8 CAN_Msg_Pend(u8 fifox);								//��ѯ���䱨��
void CAN_Rx_Msg(u8 fifox,u32 *id,u8 *ide,u8 *rtr,u8 *len,u8 *dat);//��������
u8 CAN_Tx_Staus(u8 mbox);  								//���ط���״̬
u8 CAN_Send_Msg(u8* msg,u8 len);						//��������
u8 CAN_Receive_Msg(u8 *buf);							//��������
#endif

















