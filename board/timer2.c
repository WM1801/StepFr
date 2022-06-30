#include "timer2.h"


static TIMER_CntInitTypeDef UsTIM_CntInitBlink;

void initTimer2(void)
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER2, ENABLE);
	
	TIMER_DeInit(MDR_TIMER2); 
	
	TIMER_BRGInit(MDR_TIMER2, TIMER_HCLKdiv2); // 80/2=40Mhz
	
	UsTIM_CntInitBlink.TIMER_Prescaler = 20;   // (80Mhz /2 )*20 = 500ns
	UsTIM_CntInitBlink.TIMER_Period = 1;//2*500==1000us 
  UsTIM_CntInitBlink.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir; 
	UsTIM_CntInitBlink.TIMER_CounterDirection = TIMER_CntDir_Dn; 
	UsTIM_CntInitBlink.TIMER_EventSource =    TIMER_EvSrc_TIM_CLK;  // TIMER_EvSrc_None; 
	UsTIM_CntInitBlink.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1; 
	UsTIM_CntInitBlink.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately; 
	UsTIM_CntInitBlink.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK; 
	UsTIM_CntInitBlink.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None; 
	UsTIM_CntInitBlink.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted; 
	UsTIM_CntInitBlink.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted; 
	TIMER_CntInit(MDR_TIMER2, &UsTIM_CntInitBlink); 
	
	NVIC_EnableIRQ(Timer2_IRQn); 
	TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE); 
	
	TIMER_Cmd(MDR_TIMER2, DISABLE);
}

void startTimer2(uint32_t timeUs) 
{
		if(timeUs!=0) 
		{
			TIMER_Cmd(MDR_TIMER2, DISABLE);
			MDR_TIMER2->CNT=0;
			MDR_TIMER2->ARR=(timeUs<<1)-1; 
			TIMER_Cmd(MDR_TIMER2, ENABLE); 
		}
}

void stopTimer2(void)
{
		TIMER_Cmd(MDR_TIMER2, DISABLE); 
}




