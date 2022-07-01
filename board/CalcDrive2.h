#ifndef __CALC_DRIVE_H
#define __CALC_DRIVE_H



#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"


#include "math.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "board.h"
#include "Led.h"
#include "spiRtos.h"
#include "l6470Timer.h"




void initCalcDrive(uint8_t stepMode); 
SHD_Info* GetShdInfo(void); 

void StartMove(int32_t position); 
void StopMove(void);

void SetSpeed(uint32_t speed); 
uint32_t GetSpeed( uint8_t sel); 

//acc
void setAcc(uint32_t data); 
uint32_t getAcc(void); 
uint32_t getC0(uint8_t sel);

//Vmax
void setVmax(uint32_t speed); 
uint32_t getVmax(void); 

//Vmin 
void setVmin(uint32_t speed); 
uint32_t getVmin(void); 

uint32_t getPosition(uint8_t sel ); 

void reTakt(void);








#endif 



