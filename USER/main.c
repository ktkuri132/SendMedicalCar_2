#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include <sys.h>


int main()
{
    delay_init(180);
	GPIO_InitTypeDef GPIO_InitStruct;
  
	// Enable the clock for GPIOB
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
	// Configure PB1 as output
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
  
	// Turn on the LED connected to PB1
	GPIO_SetBits(GPIOB, GPIO_Pin_0);

	while(1) 
    {
		    PBout(0) = 0;
        delay_ms(500);
        PBout(0) = 1;
        delay_ms(500);
	}
}
