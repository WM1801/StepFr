#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H


#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"

typedef enum 
{
    RING_ERROR = 0,             
    RING_SUCCESS = !RING_ERROR  
} Ring_ErrorStatus_t;


typedef struct 
{
	uint8_t *buffer; 
	uint16_t idxIn; 
	uint16_t idxOut; 
	uint16_t size; 
} Ring_Buffer_t; 


Ring_ErrorStatus_t Ring_Init(Ring_Buffer_t *ring, uint8_t *buf, uint16_t size); 
void Ring_Clear(Ring_Buffer_t* buf); 
uint16_t Ring_GetCount(const Ring_Buffer_t *buf); 

uint16_t Ring_CRC16ccitt(const Ring_Buffer_t *buf, uint16_t lenght, uint16_t position);
uint16_t Ring_CRC16ccitt_Intermediate(const Ring_Buffer_t *buf, uint16_t lenght, uint16_t tmpCrc, uint16_t position);


// view data bez udalenia
int32_t Ring_ShowSymbol(const Ring_Buffer_t *buf, uint16_t symbolNumber);


void Ring_Put(Ring_Buffer_t* buf, uint8_t symvol); 
void Ring_PutBuffr(Ring_Buffer_t *ringbuf, uint8_t *src, uint16_t len);

uint8_t Ring_Pop(Ring_Buffer_t *buf); 
uint16_t Ring_Pop16(Ring_Buffer_t *buf);
uint32_t Ring_Pop32(Ring_Buffer_t *buf);
void Ring_PopBuffr(Ring_Buffer_t *ringbuf, uint8_t *destination, uint16_t len);
void Ring_PopString(Ring_Buffer_t *ringbuf, char *string);



























#endif 

