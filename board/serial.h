#ifndef SERIAL_H
#define SERIAL_H

#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_it.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "Led.h"
#include "uartTimer.h"


typedef void * xComPortHandle; 

xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud); // unsigned portBASE_TYPE uxQueueLength );
QueueHandle_t* GetReciveList(void);
QueueHandle_t* GetSendList(void);
xSemaphoreHandle* GetCounterSemaphor(void); 





#endif 
