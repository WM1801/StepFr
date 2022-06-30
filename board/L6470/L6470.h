#ifndef  __L6470_H
#define __L6470_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_it.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "Led.h"
#include "spiRtos.h"
#include "l6470Timer.h"


// dSPIN internal register addresses
typedef enum {
    dSPIN_ABS_POS           =((uint8_t)0x01),
    dSPIN_EL_POS            =((uint8_t)0x02),
    dSPIN_MARK              =((uint8_t)0x03),
    dSPIN_SPEED             =((uint8_t)0x04),
    dSPIN_ACC               =((uint8_t)0x05),
    dSPIN_DEC               =((uint8_t)0x06),
    dSPIN_MAX_SPEED         =((uint8_t)0x07),
    dSPIN_MIN_SPEED         =((uint8_t)0x08),
    dSPIN_FS_SPD            =((uint8_t)0x15),
    dSPIN_KVAL_HOLD         =((uint8_t)0x09),
    dSPIN_KVAL_RUN          =((uint8_t)0x0A),
    dSPIN_KVAL_ACC          =((uint8_t)0x0B),
    dSPIN_KVAL_DEC          =((uint8_t)0x0C),
    dSPIN_INT_SPD           =((uint8_t)0x0D),
    dSPIN_ST_SLP            =((uint8_t)0x0E),
    dSPIN_FN_SLP_ACC        =((uint8_t)0x0F),
    dSPIN_FN_SLP_DEC        =((uint8_t)0x10),
    dSPIN_K_THERM           =((uint8_t)0x11),
    dSPIN_ADC_OUT           =((uint8_t)0x12),
    dSPIN_OCD_TH            =((uint8_t)0x13),
    dSPIN_STALL_TH          =((uint8_t)0x14),
    dSPIN_STEP_MODE         =((uint8_t)0x16),
    dSPIN_ALARM_EN          =((uint8_t)0x17),
    dSPIN_CONFIG            =((uint8_t)0x18),
    dSPIN_STATUS            =((uint8_t)0x19),
    dSPIN_RESERVED_REG2     =((uint8_t)0x1A),
    dSPIN_RESERVED_REG1     =((uint8_t)0x1B)
} dSPIN_Registers_TypeDef;

typedef enum {
    dSPIN_DIR_FWD   =((uint8_t)0x01),
    dSPIN_DIR_REV   =((uint8_t)0x00)
} dSPIN_Direction_TypeDef;


void initL6470Hw(uint8_t mStep, uint8_t current);
void getErrorL6470(uint8_t *data); 
void getCountErrorL6470(uint8_t *data); 
void getStatusL6470(uint8_t *data); 
void initL6470Register(void);

#endif
