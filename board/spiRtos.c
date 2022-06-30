#include "spiRtos.h"

static uint32_t ErrorCountSpi;  
static uint32_t ErrorTypeSpi;
void addError(uint8_t type); 

#define DEBUG_TIMER_ON DEF_OFF

#if (DEBUG_TIMER_ON==DEF_ON)
#define TIME_SPI 100
#endif 



//********* freertos ***********
xSemaphoreHandle xSemL6470; // dostup po spi k L6470

QueueHandle_t xSendListSpi; 
#define SIZE_SEND_LIST_SPI 50


void xTaskSendSpiData(void *pvParameters); 

#if (DEBUG_TIMER_ON==DEF_ON)
static xTimerHandle SpiTmr; 
void SlotSpiTmr(xTimerHandle Tmr); 
#endif 

void sendSpiByte(uint8_t data);
uint16_t readDataIzBuf(uint8_t * Data); 


void addError(uint8_t type)
{
		ErrorCountSpi++; 
		ErrorTypeSpi<<=8; 
		ErrorTypeSpi|=type; 
}



#if (DEBUG_TIMER_ON==DEF_ON)
void SlotSpiTmr(xTimerHandle Tmr)
{
		if(Tmr==SpiTmr)
		{
				//sendSpiByte(0x01);	
				//sendSpiByte(0x05);	
				sendSpiByte(0x20|0x18);
				sendSpiByte(0xFF);
				sendSpiByte(0xFF);
				counterR = readSpi16(&data1); 
		
		}
	
}
#endif



void xTaskSendSpiData(void *pvParameters)
{
		while(1)
		{
				//xSemaphoreTake(xSemPauseSpi, portMAX_DELAY); 
				//if(uxQueueMessagesWaiting(xSendListSpi)>0)
				{
						uint8_t b; 
						while(xQueueReceive(xSendListSpi, &b, portMAX_DELAY)==pdPASS)
						{
							sendSpi16((uint16_t) b);  
						}
				}
				portYIELD(); 
		}
		vTaskDelete(NULL);
}


//***************************************************************************
void initSpiRtos(void)
{
	initSPI(); 
	vSemaphoreCreateBinary(xSemL6470); 
	if(xSemL6470!=NULL)
	{

			// create task send 
			if(xTaskCreate(xTaskSendSpiData, "SendSpi", configMINIMAL_STACK_SIZE, NULL, PRIORITY_TASK_SEND_SPI, (TaskHandle_t *)NULL)!=pdTRUE)
			{
					addError(ER_SPI_CREATE_SEND_TASK); 
			}
			// ochered send data 
			xSendListSpi = xQueueCreate(SIZE_SEND_LIST_SPI, ( unsigned portBASE_TYPE ) sizeof(uint8_t));
				if(xSendListSpi == NULL) 
				{
						addError(ER_SPI_CREATE_QUEUE_SEND); 
				}
	}
	else 
	{
			addError(ER_SPI_CREATE_SEMAPH_SEND); 
	}
	
	#if (DEBUG_TIMER_ON==DEF_ON)
	SpiTmr = xTimerCreate("SpiTmr", TIME_SPI, pdTRUE, (void*)0, SlotSpiTmr); 
	xTimerStart(SpiTmr, 0);
	#endif 
}


// send 
void sendSpiByte(uint8_t data)
{
		uint8_t b = data; 
		xQueueSendToBack(xSendListSpi, &b, portMAX_DELAY);  
		
}
// read 
uint16_t readDataIzBuf(uint8_t* readData)
{
	uint16_t counterR  = readSpi8( readData); 
	return counterR; 
}


//**************************************************
//    L6470 
//**************************************************

uint16_t readRegister(uint8_t adr, uint8_t* readData) 
{
		uint16_t count = 0; 
		
		if(xSemL6470!=NULL)
		{
			if(xSemaphoreTake(xSemL6470, portMAX_DELAY)==pdTRUE)
			{
					clearReadBufSpi(); 
					sendSpiByte(L6470_READ_REGISTER|adr);
					// max 22 bit datasheet L6470
					sendSpiByte(L6470_NOP);
					sendSpiByte(L6470_NOP);
					sendSpiByte(L6470_NOP);	
					LedToggle(LED2); 
					count = readSpi8(readData);
					xSemaphoreGive(xSemL6470);
			}
		}
		 return count; 		 
}


uint16_t writeRegister(uint8_t adr, uint8_t* writeData, uint16_t size) 
{
		uint16_t count = 0; 
		if(xSemL6470!=NULL)
		{
			if(xSemaphoreTake(xSemL6470, portMAX_DELAY)==pdTRUE)
			{
					sendSpiByte(adr);
					// max 22 bit datasheet L6470
					uint16_t i; 
					for(i = 0;  i<size; i++)
					{
						sendSpiByte(writeData[i]);
					}
					count = i;
					//LedToggle(LED1); 
					xSemaphoreGive(xSemL6470);
			}
		}
		 return count; 		 
}
// read 2 baita
uint16_t getStatus(uint8_t* readData) 
{
		uint16_t count = 0; 
		if(xSemL6470!=NULL)
		{
			if(xSemaphoreTake(xSemL6470, portMAX_DELAY)==pdTRUE)
			{
					sendSpiByte(L6470_GET_STATUS);
					// max 22 bit datasheet L6470
					sendSpiByte(L6470_NOP);
					sendSpiByte(L6470_NOP);	
	
					count = readSpi8(readData);
					xSemaphoreGive(xSemL6470);
			}
		}
		 return count; 		 
}

uint16_t stepClock(uint8_t dir)
{
		uint16_t count = 0; 
		if(xSemL6470!=NULL)
		{
			if(xSemaphoreTake(xSemL6470, portMAX_DELAY)==pdTRUE)
			{
					sendSpiByte(L6470_STEP_CLOCK|(dir&0x01));
					xSemaphoreGive(xSemL6470);
					count = 1; 
			}
		}
		 return count; 	
}

uint16_t resetL6470Soft(void)
{
		uint16_t count = 0; 
		if(xSemL6470!=NULL)
		{
			if(xSemaphoreTake(xSemL6470, portMAX_DELAY)==pdTRUE)
			{
					sendSpiByte(L6470_RESET_DEVICE);
					xSemaphoreGive(xSemL6470);
					count = 1;
			}
		}
		 return count; 	
}

uint16_t hardHiz(void)
{
		uint16_t count = 0; 
		if(xSemL6470!=NULL)
		{
			if(xSemaphoreTake(xSemL6470, portMAX_DELAY)==pdTRUE)
			{
					sendSpiByte(L6470_HARD_HIZ);
					xSemaphoreGive(xSemL6470);
					count = 1;
			}
		}
		 return count; 	
	
}
