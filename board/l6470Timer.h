#ifndef __L6470TIMER_H
#define __L6470TIMER_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_it.h"
#include "MDR32F9Qx_timer.h"




#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "Led.h"
#include "CalcDrive.h"



void initTimerL6470(void); 
void startTimerL6470(uint32_t timeUs); 
void startMoveTimer(void); 

void stopTimerL6470(void); 
uint16_t restartTimerMoreFFFF(void);

void initPtrShdInfo(SHD_Info * ptrInfo); 






#endif
