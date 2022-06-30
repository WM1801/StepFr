#ifndef __TIMER2_H
#define __TIMER2_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_it.h"
#include "MDR32F9Qx_timer.h"



#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"



void initTimer2(void); 
void startTimer2(uint32_t timeUs); 
void stopTimer2(void); 






#endif
