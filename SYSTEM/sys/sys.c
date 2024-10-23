#include "sys.h"  

void TimingDelay_Decrement(void);
#ifdef  __CC_ARM
__asm void WFI_SET(void)
{
	WFI;		  
}
//??????(?????fault?NMI??)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//??????
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//??????
//addr:????
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
#endif



#ifdef __GNUC__
void WFI_SET(void)
{
    __ASM volatile("WFI");
}

//??????(?????fault?NMI??)
void INTX_DISABLE(void)
{
	__ASM volatile("CPSID   I");
	__ASM volatile("BX      LR");  
}
//??????
void INTX_ENABLE(void)
{
	__ASM volatile("CPSIE   I");
	__ASM volatile("BX      LR");
}
//??????
//addr:????
void MSR_MSP(u32 addr) 
{
	__ASM volatile("MSR MSP, r0"); 			//set Main Stack value
	__ASM volatile("BX r14");
}
#endif
