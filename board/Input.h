#ifndef __INPUT_H
#define __INPUT_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"





#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "board.h"

void initInput(void); 

uint32_t getDip(void);
uint32_t getSensorRead(void);
uint32_t getSensorMask(SENSOR sens);

uint16_t getDipValue(void); 











#endif



