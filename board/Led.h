#ifndef  LED
#define LED



#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "board.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


 
uint16_t LedInit(void); 

void LedOn(LEDs led); 
void LedOff(LEDs led);
void LedToggle(LEDs led); 
// use soft timer 
void ShotLed(LEDs led); 

uint16_t getStatusLed(void); 





#endif



