#ifndef __SPI_H
#define __SPI_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_it.h"
#include "MDR32F9Qx_ssp.h"


#include "Led.h"




void initSPI(void); 
// return 1 if ok, else 0
uint8_t sendSpi16(uint16_t data); 
// return 1 if ok, else 0
uint8_t readSpi16(uint16_t *readData);
uint8_t readSpi8(uint8_t *readData);
void clearReadBufSpi(void); 









#endif 



