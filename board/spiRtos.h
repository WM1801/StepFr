#ifndef __SPIRTOS_H
#define __SPIRTOS_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_it.h"
#include "MDR32F9Qx_ssp.h"

#include "stdlib.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "Led.h"
#include "spi.h"



#define L6470_NOP									0x00
#define L6470_READ_REGISTER       0x20
#define L6470_WRITE_REGISTER      0x00
#define L6470_GET_STATUS					0xD0
#define L6470_STEP_CLOCK					0x58 //    |DIR- 0-i bit 
#define L6470_RESET_DEVICE				0xC0
#define L6470_HARD_HIZ    				0xA8


typedef void * xSpiPortHandle; 


void initSpiRtos(void); 

void sendSpiByte(uint8_t data); 
// return count read data 
uint16_t readDataIzBuf(uint8_t* readData); 

// L6470
uint16_t readRegister(uint8_t adr, uint8_t* readData); 
uint16_t writeRegister(uint8_t adr, uint8_t* writeData, uint16_t size); 
uint16_t getStatus(uint8_t* readData);
//
#define L6470_FORWARD 		1
#define L6470_REVERSE			0 
uint16_t stepClock(uint8_t dir); 

uint16_t resetL6470Soft(void);
uint16_t hardHiz(void); 









#endif 

