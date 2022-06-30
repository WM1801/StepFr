#include "uartTimer.h"


static TIMER_CntInitTypeDef UsTIM_CntInitBlink;


void UartTimerInit(void)
{
	RST_CLK_PCLKcmd(UART_TIMER_CLK, ENABLE);
	
	TIMER_DeInit(UART_TIMER); 
	
	TIMER_BRGInit(UART_TIMER, TIMER_HCLKdiv128); // 80/128
	
	TIMER_CntStructInit(&UsTIM_CntInitBlink);
	UsTIM_CntInitBlink.TIMER_Prescaler = 19;  
	UsTIM_CntInitBlink.TIMER_Period = 10; // 320us
  UsTIM_CntInitBlink.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir; 
	UsTIM_CntInitBlink.TIMER_CounterDirection = TIMER_CntDir_Up; 
	UsTIM_CntInitBlink.TIMER_EventSource =    TIMER_EvSrc_TIM_CLK;  // TIMER_EvSrc_None; 
	UsTIM_CntInitBlink.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1; 
	UsTIM_CntInitBlink.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately; 
	UsTIM_CntInitBlink.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK; 
	UsTIM_CntInitBlink.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None; 
	UsTIM_CntInitBlink.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted; 
	UsTIM_CntInitBlink.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted; 
	TIMER_CntInit(UART_TIMER, &UsTIM_CntInitBlink); 
	
	NVIC_EnableIRQ(UART_TIMER_IRQ); 
	TIMER_ITConfig(UART_TIMER, TIMER_STATUS_CNT_ARR, ENABLE); 
	
	TIMER_Cmd(UART_TIMER, DISABLE); 
	
	
}


void StartUartTimer(void)
{
		UART_TIMER->CNT=0;
		TIMER_Cmd(UART_TIMER, ENABLE); 
}

void StopUartTimer(void)
{
		TIMER_Cmd(UART_TIMER, DISABLE); 
}
