#include "l6470Timer.h"

#define MODE_PWM DEF_ON

//CNT    /0/1/2/3/4/5/6/           // max 65.536 ms 
//CCR3	 /-/*/-/-/-/-/-/					 // min 1us

//out    __|**|________




// CCR3 < CNT  < CCR31  
//#define SHIRINA_IMP_V_TAKTAX_TAIMERA   1  /*CCR31 = START_IMPULSA_PO_CNT + SHIRINA_IMP_V_TAKTAX_TAIMERA */
#define START_IMPULSA_PO_CNT           1  /* vsegda > 0 CCR3 = START_IMPULSA_PO_CNT */ 
#define FIRST_STEP_CNT           START_IMPULSA_PO_CNT + 1  /* vsegda > 0 CCR3 = START_IMPULSA_PO_CNT */ 
static PORT_InitTypeDef PortInit;
static TIMER_CntInitTypeDef UsTIM_CntInitBlink;
static TIMER_ChnOutInitTypeDef sTim_ChnOutInit;
static TIMER_ChnInitTypeDef sTim_ChnInit;

uint16_t flagTimeMoreFFFF; 
volatile uint16_t dtimeMs;
volatile uint16_t dtimeUs; 
uint16_t saveTimeMs;
uint16_t saveTimeUs;


#define TIME_STCK 1


SHD_Info * ptrInfo; 

void initPinTimerL6470(void)
{
		
		RST_CLK_PCLKcmd(STCK_CLK, ENABLE); 
	
		 
		PORT_StructInit(&PortInit);
	
		// Fill PortInit structure 
	
    PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
	 
    PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;
		
		// Configure pin as output 
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = STCK_PIN ;
    PORT_Init(STCK_PORT, &PortInit);
}

////////////////////

void initTimerL6470(void)
{
	flagTimeMoreFFFF = 0; 
	initPinTimerL6470(); 

	RST_CLK_PCLKcmd(L6470_TIMER_CLK, ENABLE);	
	
	TIMER_DeInit(L6470_TIMER); 	
	
	TIMER_BRGInit(L6470_TIMER, TIMER_HCLKdiv2); // 80/2=40Mhz

  TIMER_CntStructInit(&UsTIM_CntInitBlink);
	UsTIM_CntInitBlink.TIMER_Prescaler = 39;   // (80Mhz /2 )*(40-1) = 1000ns        (80Mhz /2 )*(20-1) = 500ns
	UsTIM_CntInitBlink.TIMER_Period = 0xFFFF;      
  UsTIM_CntInitBlink.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir; 
	UsTIM_CntInitBlink.TIMER_CounterDirection = TIMER_CntDir_Dn; 
	UsTIM_CntInitBlink.TIMER_EventSource =    TIMER_EvSrc_TIM_CLK;  // TIMER_EvSrc_None; 
	UsTIM_CntInitBlink.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1; 
	UsTIM_CntInitBlink.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately; 
	UsTIM_CntInitBlink.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK; 
	UsTIM_CntInitBlink.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None; 
	UsTIM_CntInitBlink.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted; 
	UsTIM_CntInitBlink.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted; 

	TIMER_CntInit(L6470_TIMER, &UsTIM_CntInitBlink); 
	
	
#if(MODE_PWM==DEF_ON)	
	
	
	
	/* Initializes the TIMER1 Channel 3 -------------------------------------*/
	TIMER_ChnStructInit(&sTim_ChnInit); 
	sTim_ChnInit.TIMER_CH_Mode											= TIMER_CH_MODE_PWM;
	sTim_ChnInit.TIMER_CH_REF_Format								= TIMER_CH_REF_Format1; // TIMER_CH_REF_Format3 - toggle ;TIMER_CH_REF_Format6; -pwm 
	sTim_ChnInit.TIMER_CH_Number										= TIMER_CHANNEL3; 
	sTim_ChnInit.TIMER_CH_CCR1_Ena									= DISABLE;// enable
	TIMER_ChnInit(L6470_TIMER, &sTim_ChnInit); 
	
	TIMER_SetChnCompare(L6470_TIMER, TIMER_CHANNEL3, TIME_STCK);
	
	/* Initializes the TIMER Channel  Output -------------------------------*/
	TIMER_ChnOutStructInit(&sTim_ChnOutInit);
		
	sTim_ChnOutInit.TIMER_CH_DirOut_Polarity       = TIMER_CHOPolarity_NonInverted; 
	sTim_ChnOutInit.TIMER_CH_DirOut_Source         = TIMER_CH_OutSrc_REF;
	sTim_ChnOutInit.TIMER_CH_DirOut_Mode           = TIMER_CH_OutMode_Output; 
	sTim_ChnOutInit.TIMER_CH_NegOut_Polarity			 = TIMER_CHOPolarity_NonInverted; 
	sTim_ChnOutInit.TIMER_CH_NegOut_Source         = TIMER_CH_OutSrc_REF; 
	sTim_ChnOutInit.TIMER_CH_NegOut_Mode					 = TIMER_CH_OutMode_Output; 
	sTim_ChnOutInit.TIMER_CH_Number								 = TIMER_CHANNEL3;
	TIMER_ChnOutInit(L6470_TIMER, &sTim_ChnOutInit); 
	
	//********************************
	//TIMER_ChnCompareConfig(L6470_TIMER,TIMER_CHANNEL3, 1, TIMER_CH_CCR_Update_Immediately);

	//******************************
	
	NVIC_EnableIRQ(L6470_TIMER_IRQ); 
	TIMER_ClearITPendingBit(L6470_TIMER, TIMER_STATUS_CNT_ZERO);
	TIMER_ITConfig(L6470_TIMER, TIMER_STATUS_CNT_ZERO , ENABLE); 
 
	L6470_TIMER->CCR3 = START_IMPULSA_PO_CNT; 
	//L6470_TIMER->CCR31 = START_IMPULSA_PO_CNT + SHIRINA_IMP_V_TAKTAX_TAIMERA; 

	TIMER_Cmd(L6470_TIMER, DISABLE);
	
	
#else	
	NVIC_EnableIRQ(L6470_TIMER_IRQ); 
	TIMER_ITConfig(L6470_TIMER, TIMER_STATUS_CNT_ARR, ENABLE); 	
	TIMER_Cmd(L6470_TIMER, ENABLE);
#endif

	
}

void initPtrShdInfo(SHD_Info * ptr)
{
	ptrInfo = ptr; 
}






void startTimerL6470(uint32_t timeUs) 
{
		if(timeUs!=0) 
		{			
				ptrInfo->currentVal.speed = timeUs; 
				ptrInfo->nextVal.speed = timeUs; 
				
				TIMER_Cmd(L6470_TIMER, DISABLE);				
				L6470_TIMER->CCR3 = START_IMPULSA_PO_CNT;
				L6470_TIMER->CNT=FIRST_STEP_CNT;
				TIMER_Cmd(L6470_TIMER, ENABLE); 
		}
}

void startMoveTimer(void)
{
		TIMER_Cmd(L6470_TIMER, DISABLE);				
		L6470_TIMER->CCR3 = START_IMPULSA_PO_CNT;
		L6470_TIMER->CNT=FIRST_STEP_CNT;
		TIMER_Cmd(L6470_TIMER, ENABLE);
}

void stopTimerL6470(void)
{
		TIMER_Cmd(L6470_TIMER, DISABLE); 
}


// return 1 proshlo zadannoe vremz
// return 0 proshlo FFFF us iz zadannogo vremeni
uint16_t restartTimerMoreFFFF()
{	
			if(ptrInfo->currentVal.speed&0xFFFF0000)
			{
					ptrInfo->currentVal.speed = ptrInfo->currentVal.speed - 0x10000; 
					TIMER_Cmd(L6470_TIMER, DISABLE);
					L6470_TIMER->CCR3 = 0xFFFF; 
					TIMER_Cmd(L6470_TIMER, ENABLE);
					L6470_TIMER->ARR=0xFFFE; // dtimeUs++?
					return 0; 
			}
			else 
			{ 
					TIMER_Cmd(L6470_TIMER, DISABLE);
					L6470_TIMER->CCR3 = START_IMPULSA_PO_CNT;
					TIMER_Cmd(L6470_TIMER, ENABLE);
					L6470_TIMER->ARR= (ptrInfo->currentVal.speed &0xFFFF);
					ptrInfo->currentVal=ptrInfo->nextVal; 							
					return 1; 
			}		
}


/// file CalcDrive.c
///void Timer3_IRQHandler(void); 

void Timer3_IRQHandler(void)
{				
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		if(TIMER_GetITStatus(L6470_TIMER, TIMER_STATUS_CNT_ZERO) == SET)
		{
			TIMER_ClearITPendingBit(L6470_TIMER, TIMER_STATUS_CNT_ZERO);
				
				  
				if(restartTimerMoreFFFF())
				{
					reTakt();
					LedToggle(LED1);
					//TIMER_Cmd(L6470_TIMER, DISABLE);
					//xSemaphoreGiveFromISR(xSemTimer, &xHigherPriorityTaskWoken);
						
									 
						
					 	 
				}
				
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);	 
				
		}
}





