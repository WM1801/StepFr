#include "LED.h"

static uint16_t StatusLed; 
//static PORT_InitTypeDef PORT_InitStructureLed; 

static volatile uint16_t  ledStattus;  // 1 - on, 0 - off

static xTimerHandle Led1TimerHandle;
static xTimerHandle Led2TimerHandle;
static xTimerHandle Led3TimerHandle;

#define PERIOD_SOFT_TMR1  150
#define PERIOD_SOFT_TMR2  150
#define PERIOD_SOFT_TMR3  150

void SlotTmr(xTimerHandle t); 

uint16_t getStatusLed(void)
{
		return StatusLed; 
}


uint16_t LedInit(void)
{
	StatusLed = 0; 
	// init Led
	SetOutputMode(LED1_PORT, LED1_PIN, LED1_CLK);
  SetOutputMode(LED2_PORT, LED2_PIN, LED2_CLK);
	SetOutputMode(LED3_PORT, LED3_PIN, LED3_CLK);
	ledStattus = 0; 
	
	Led1TimerHandle = xTimerCreate("Tmr1", PERIOD_SOFT_TMR1, pdFALSE, (void*)0, SlotTmr);
  Led2TimerHandle = xTimerCreate("Tmr2", PERIOD_SOFT_TMR2, pdFALSE, (void*)0, SlotTmr);
	Led3TimerHandle = xTimerCreate("Tmr3", PERIOD_SOFT_TMR3, pdFALSE, (void*)0, SlotTmr);
	ShotLed(LED1); 
	ShotLed(LED2);
	ShotLed(LED3);
	
	if((Led1TimerHandle!=NULL) && (Led2TimerHandle!=NULL) && (Led3TimerHandle!=NULL))
	{
			StatusLed = 1; 
	}
	return StatusLed; 
}


void ShotLed(LEDs led)
{
	switch(led)
	{
		case (LED1):{LedOn(LED1); xTimerStart(Led1TimerHandle, 10); break;}
		case (LED2):{LedOff(LED2); xTimerStart(Led2TimerHandle, 10); break;}
		case (LED3):{LedOff(LED3); xTimerStart(Led3TimerHandle, 10); break;}
		default: {}
	}
	
}

void SlotTmr(xTimerHandle t)
{
	//unsigned portBASE_TYPE *pxTimerID;
	//pxTimerID = pvTimerGetTimerID(t); 
  if(t==Led1TimerHandle) { LedToggle(LED1); }
	else {
		if(t==Led2TimerHandle)  { LedToggle(LED2); } 
		else {
			if(t==Led3TimerHandle) { LedToggle(LED3); }
		}
	}
}

void LedOn(LEDs led)
{
	switch(led)
	{
		case (LED1):{ PORT_ResetBits(LED1_PORT, LED1_PIN); ledStattus |= LED1 ; break;}
		case (LED2):{ PORT_ResetBits(LED2_PORT, LED2_PIN); ledStattus |= LED2 ; break;}
		case (LED3):{ PORT_SetBits(LED3_PORT, LED3_PIN); 	 ledStattus |= LED3 ; break;}
		default: {}
	}
	
}

void LedOff(LEDs led)
{
	switch(led)
	{
		case (LED1):{ PORT_SetBits(LED1_PORT, LED1_PIN);    ledStattus &= ((~LED1)&0xFFFF);  break;}
		case (LED2):{ PORT_SetBits(LED2_PORT, LED2_PIN);    ledStattus &= ((~LED2)&0xFFFF);  break;}
		case (LED3):{ PORT_ResetBits(LED3_PORT, LED3_PIN);  ledStattus &= ((~LED3)&0xFFFF);  break;}
		default: {}
	}
}

void LedToggle(LEDs led)
{
	uint16_t ledN = (uint16_t) led;
	switch(ledN)
	{
		case ((uint16_t)LED1):{ if((ledStattus&LED1) !=0){ LedOff(LED1); }  else {LedOn(LED1);} break;}
		case ((uint16_t)LED2):{ if((ledStattus&LED2) !=0){ LedOff(LED2); }  else {LedOn(LED2);} break;}
		case ((uint16_t)LED3):{ if((ledStattus&LED3) !=0){ LedOff(LED3); }  else {LedOn(LED3);} break;}
		default: {}
	}
}



