#ifndef __UARTTIMER_H
#define __UARTTIMER_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_timer.h"
#include "board.h"


void UartTimerInit(void);
void StartUartTimer(void);
void StopUartTimer(void);













#endif
