#ifndef __CALC_DRIVE_H
#define __CALC_DRIVE_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_it.h"
#include "MDR32F9Qx_timer.h"
#include "math.h"



#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "board.h"
#include "Led.h"
#include "spiRtos.h"
#include "l6470Timer.h"



void initCalcDrive(uint8_t sel); 


void getStatusCalc(uint8_t * data32);



void getPosition(uint8_t * data32); 
void StartMove( SHD_dir dir, int32_t position); 
void StopMove(void); 
void SetSpeed(uint32_t speed); 
uint32_t GetSpeed( uint8_t sel); 

SHD_Info* GetShdInfo(void); 
//acc
void setAcc(uint32_t data); 
uint32_t getAcc(void); 
uint32_t getC0(void);
//Vmax
void setVmax(uint32_t speed); 
uint32_t getVmax(void); 
//Vmin 
void setVmin(uint32_t speed); 
uint32_t getVmin(void); 
uint32_t getCountSteToVmax(void); 

void calcCn(void); 












#endif



