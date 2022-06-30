#include "uartTimer.h"


TIMER_CntInitTypeDef UsTIM_CntInitBlink;


void UartTimerInit(void)
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER1, ENABLE);
	
	TIMER_DeInit(MDR_TIMER1); 
	
	TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv128); // 80/128
	
	UsTIM_CntInitBlink.TIMER_Prescaler = 19;  
	UsTIM_CntInitBlink.TIMER_Period = 10; // 320us
  UsTIM_CntInitBlink.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir; 
	UsTIM_CntInitBlink.TIMER_CounterDirection = TIMER_CntDir_Up; 
	UsTIM_CntInitBlink.TIMER_EventSource = TIMER_EvSrc_None; 
	UsTIM_CntInitBlink.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1; 
	UsTIM_CntInitBlink.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately; 
	UsTIM_CntInitBlink.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK; 
	UsTIM_CntInitBlink.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None; 
	UsTIM_CntInitBlink.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted; 
	UsTIM_CntInitBlink.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted; 
	TIMER_CntInit(MDR_TIMER1, &UsTIM_CntInitBlink); 
	
	NVIC_EnableIRQ(Timer1_IRQn); 
	TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE); 
	
	TIMER_Cmd(MDR_TIMER1, DISABLE); 
	
	
}


void StartUartTimer(void)
{
		MDR_TIMER1->CNT=0;
		TIMER_Cmd(MDR_TIMER1, ENABLE); 
}

void StopUartTimer(void)
{
		TIMER_Cmd(MDR_TIMER1, DISABLE); 
}
