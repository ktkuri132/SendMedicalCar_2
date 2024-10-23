#ifndef __USART_H
#define __USART_H

#include "sys.h" 
#include "stdio.h"

#define USART_REC_LEN  			200  	
#define EN_USART1_RX 			1		
	  	
extern u8  USART_RX_BUF[USART_REC_LEN];  
extern u16 USART_RX_STA;         		



void uart_init(u32 pclk2,u32 bound);

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);

void USART1_SEND_DATA_U8(u8 data);
void USART1_SEND_BUF_U8(u8*buf,u8 len);
#endif


