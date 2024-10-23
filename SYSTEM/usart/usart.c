#include "sys.h"
#include "usart.h"	 


int16_t RX_sign;

#include <stdarg.h>

//定义keil宏，在MDK编译环境下支持printf等函数

	#if 1
		#ifdef  __CC_ARM
			#pragma import(__use_no_semihosting)
			struct __FILE 
			{ 
				int handle; 
			}; 
			#elif defined ( __GNUC__ ) || defined (__clang__)
			__asm (".global __use_no_semihosting\n\t");   
		#endif
	#endif


/*arm-gcc编译环境下实现printf函数的重定向  失败*/
/*printf函数的替代品*/

#ifdef __GNUC__


int _write (int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成
        USART1->DR = (uint8_t) pBuffer[i];       //写DR,串口1将发送数据
    }
    return size;
}
#elif __CC_ARM

#pragma import(__use_no_semihosting)




int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}


void _sys_exit(int x)
{
    x = x;
}

//char *_sys_command_string(char *cmd, int len)
//{
//    return NULL;
//}


FILE __stdout;
FILE __stdin;



int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);     

    USART1->DR = (uint8_t)ch;             
    return ch;
}

int fgetc(FILE *f)
{
  while(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
  return (int)USART_ReceiveData(USART1);
}


#endif

 
#if EN_USART1_RX   
  	
u8 USART_RX_BUF[USART_REC_LEN];     

u16 USART_RX_STA=0;         

int i=0;

void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1时钟
	
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9，PA10
	
	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound; //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启串口接收中断
	USART_Cmd(USART1, ENABLE); //使能串口1
}

/* 串口2初始化  */
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟
	
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA3复用为USART2
	
	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2，PA3
	
	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound; //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启串口接收中断
	USART_Cmd(USART2, ENABLE); //使能串口2
}



/* 串口3初始化  */
void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3时钟
	
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //GPIOB11复用为USART3
	
	//USART3端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10，PB11
	
	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound; //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启串口接收中断
	USART_Cmd(USART3, ENABLE); //使能串口3
}

//串口中断
void USART1_IRQHandler(void)
{
	u8 res;	
	if(USART1->SR&(1<<5))
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x8000)==0)
		{
			if(USART_RX_STA&0x4000)
			{
				if(res!=0x0a)USART_RX_STA=0;
				else USART_RX_STA|=0x8000;	
			}
			else 
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;	  
				}		 
			}
		}  		 									     
	} 

}

uint16_t data[20];
uint8_t data_len;

void USART2_IRQHandler(void)
{
	static int receive_strat;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{   
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
		uint16_t Res=USART_ReceiveData(USART2);
		if(Res==0x01)
		{
			receive_strat=1;
			
		}
		if(receive_strat)
		{
			if(Res==0x02)
			{
				receive_strat=0;
				return ;
			}
			data[data_len++]=Res;
			printf("%d\r\n",Res);
		}
	}

}

//发送函数
void USART1_SEND_DATA_U8(u8 data)
{
	USART1->DR = (data & (uint16_t)0x01FF);
	while((USART1->SR&0X40)==0);
}

void USART1_SEND_BUF_U8(u8*buf,u8 len)
{
	while(len)
	{
		USART1_SEND_DATA_U8(*buf);
		buf++;
		len--;
	}
}































#endif

