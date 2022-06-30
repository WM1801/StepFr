#ifndef MYUART_H
#define MYUART_H

#ifdef UARTDMA
// mode dma
#else


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_it.h"
#include "string.h"






void UartInit(void);
void UartDirPin2(void); 


void SetModeRx2(void); 
void SetModeTx2(void);

void ReadInBuf(uint16_t data); 
int UartFlagRxIrqReset(void);
uint16_t GetCurrentModeTx(void);

void SendUart(uint8_t * data, uint16_t index); 
void SendRow(uint8_t * data, uint16_t index); 
uint16_t GetSizeRxData(void);
uint8_t *GetReadBuffer(void);
void ResetCounterRxData(void); 


void send8(uint8_t d); 
void send16(uint16_t d); 
void answerCom(uint8_t adr, uint8_t com); 
void answer1X8(uint8_t adr, uint8_t com, uint8_t d1);
void answer2X8(uint8_t adr, uint8_t com ,uint8_t d1, uint8_t d2);
void answer4X8(uint8_t adr, uint8_t com ,uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4); 
void answerBuf(uint8_t adr, uint8_t com, uint8_t * buf, uint32_t size);

void answer1X32(uint8_t adr, uint8_t com ,uint32_t d); 






#endif




#endif


