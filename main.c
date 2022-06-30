
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"



#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"


#include "board.h"
#include "clk.h"
#include "Led.h"
#include "reset.h"
#include "serial.h"
#include "spiRtos.h"
#include "L6470.h"
#include "Input.h"
//*************************************
//Piority task 


uint8_t adres = 9; 
uint8_t command; 

#define TOK_DEFAULT  28  
#define STEP_DEFAULT 5



uint16_t Ispravnost;
static xComPortHandle xPort = NULL;

xSemaphoreHandle* pCounterSemaphor; 
QueueHandle_t* pReciveList; 
QueueHandle_t* pSendList;
uint32_t *ptrData; 




#if (TRACE_ON==DEF_ON)
SemaphoreHandle_t OutEnab; 

void vSendTrace(void *pvParameters)
{
	ptrData = (uint32_t*)RecorderDataPtr; 
	for(;;)
	{
			xSemaphoreTake(OutEnab, portMAX_DELAY);
			uint8_t data; 
			for(int i=0; i<TRC_CFG_EVENT_BUFFER_SIZE*4; i++)
			{
					data =(uint8_t) *(ptrData); 
				  xQueueSend(*pSendList, &data, portMAX_DELAY);
					ptrData++; 
			}
	}
	
}

#endif // TRACE_ON==DEF_ON

void sendBuf8(uint8_t adr, uint8_t com, uint8_t *data)
{
		xQueueSend(*pSendList, &adr, 0);
		xQueueSend(*pSendList, &com, 0); 
		for(int i = 0; i<congigCOUNT_BYTE_MESSAGE-2; i++)
		{
				xQueueSend(*pSendList, &data[i], 0);
				
		}
}

void to32InBuf(uint8_t *data, uint32_t val)
{
		data[0] = (uint8_t)((val>>24)&0xFF);
		data[1] = (uint8_t)((val>>16)&0xFF);
		data[2] = (uint8_t)((val>>8)&0xFF);
		data[3] = (uint8_t)(val&0xFF);
		
}




void fill(uint8_t adr, uint8_t com, uint8_t data)
{
		xQueueSend(*pSendList, &adr, 0);
		xQueueSend(*pSendList, &com, 0); 
		for(int i = 0; i<congigCOUNT_BYTE_MESSAGE-2; i++)
		{
				xQueueSend(*pSendList, &data, 0);
		}
}



void vObmenTask(void *pvParameters)
{
		uint8_t data[congigCOUNT_BYTE_MESSAGE] = {0x00}; 
		uint8_t b; 
		for(;;)
		{
				xSemaphoreTake(*pCounterSemaphor, portMAX_DELAY);
				#if ( configDEBUG_SEND_ECHO == DEF_ON) 
					if (xQueueReceive(*pReciveList, &b, portMAX_DELAY)==pdPASS)
					{	
						xQueueSend(*pSendList, &b, 0); 
						ShotLed(LED2);
					}		
				#else 
					while(uxQueueMessagesWaiting(*pReciveList)>=congigCOUNT_BYTE_MESSAGE)
					{			
							//ShotLed(LED2);
							for(int i=0; i<congigCOUNT_BYTE_MESSAGE; i++)
							{									
									xQueueReceive(*pReciveList, &b, portMAX_DELAY);
									data[i] = b; 				
							}
							#if (TRACE_ON == DEF_ON)
								if(data[0]==(uint8_t)'1')
								{
										vTraceStop(); 
										xSemaphoreGive(OutEnab); 							
								}
							#endif
							
							
							if(data[0]==(uint8_t)0x39)
							{		
									switch(data[1])
									{
										case 0x39: { break; }
										case 0x40: { ShotLed(LED2);
																	 uint8_t mas[4]={0,0,0,0};
																	if(readRegister(data[2],  mas)>0)
																	{
																	 
																			uint8_t adr = 9;
																			uint8_t com = 2; 
																	xQueueSendToBack(*pSendList, &adr, 0); 
																	xQueueSendToBack(*pSendList, &com, 0);
																	xQueueSendToBack(*pSendList, &mas[0], 0);
																	xQueueSendToBack(*pSendList, &mas[1], 0);
																	xQueueSendToBack(*pSendList, &mas[2], 0);
																	xQueueSendToBack(*pSendList, &mas[3], 0);
																	}
																		break; 
																}	
										case 0x41: {
																ShotLed(LED2); 
																	writeRegister(data[2], &data[3], 1); 
																	sendBuf8(	adres, 0x41, &data[2]); 
																	break; 
																}
										case 0x42: {
																ShotLed(LED2); 
																	writeRegister(data[2], &data[3], 2); 
																	sendBuf8(	adres, 0x42, &data[2]); 
																	break; 
																}
										case 0x43: {
																ShotLed(LED2); 
																	writeRegister(data[2], &data[3], 3); 
																	sendBuf8(	adres, 0x43, &data[2]); 
																	break; 
																}
										case 0x44: {
																ShotLed(LED2); 
																	getStatus(&data[2]); 
																	sendBuf8(	adres, 0x44, &data[2]); 
																	break; 
																}
										case 0x45: {
																	stepClock(data[5]); 
																	sendBuf8(	adres, 0x45, &data[2]);
																	break; 
															 }
										case 0x46: {
																	resetL6470Soft();
																	sendBuf8(	adres, 0x46, &data[2]);
																	break; 
															 }
										case 0x47: {
																	//uint32_t timeUs = (uint32_t)((((uint32_t)data[2])<<8)|data[3]); 
																	uint32_t timeUs = ((((uint32_t)data[2])<<24)|
																(((uint32_t)data[3])<<16)|
																(((uint32_t)data[4])<<8)|
																(((uint32_t)data[5])));
																	startTimerL6470(timeUs); 	
																	sendBuf8(	adres, 0x47, &data[2]);
																	break; 
															 }
										case 0x48: {
																	stopTimerL6470();  	
																	sendBuf8(	adres, 0x48, &data[2]);
																	break; 
															 }
										case 0x50: {
											stepClock(dSPIN_DIR_FWD);
											sendBuf8(	adres, 0x50, &data[2]);
											break; 
										}
										case 0X51:{
											// mode microshag
											writeRegister(dSPIN_STEP_MODE, &data[2], 1); 
											sendBuf8(	adres, 0x51, &data[2]);
											break; 
										}
										case 0X52:{
											initL6470Register();
											sendBuf8(	adres, 0x52, &data[2]);
											break; 
										}
										case 0x53:{
												getErrorL6470(&data[2]); 
												sendBuf8(	adres, 0x53, &data[2]);
												break; 
										}
										case 0x54:{
												getCountErrorL6470(&data[2]); 
												sendBuf8(	adres, 0x54, &data[2]);
												break; 
										}
										case 0x55:{
												getStatusL6470(&data[2]); 
												sendBuf8(	adres, 0x55, &data[2]);
												break; 
										}
										
										case 0x57:{
											startTimerL6470(0xFFFF); 
											sendBuf8(	adres, 0x57, &data[2]);
											break; 
										}
										case 0x58:{
											getPosition(&data[2]);  
											sendBuf8(	adres, 0x58, &data[2]);
											break; 
										}
										case  0x5F:{
											getStatusCalc(&data[2]); 
											sendBuf8(	adres, 0x5F, &data[2]);
											break; 
										}
										case 0x60: {
											uint32_t dataId = SCB->CPUID; 
											data[2] = (uint8_t)((dataId>>24)&0xFF);
											data[3] = (uint8_t)((dataId>>16)&0xFF);
											data[4] = (uint8_t)((dataId>>8)&0xFF);
											data[5] = (uint8_t)(dataId&0xFF);
											sendBuf8(	adres, 0x60, &data[2]);
											break; 
										}
										case 0x61: {
										
										int32_t pos = ((((int32_t)data[2])<<24)|
																(((int32_t)data[3])<<16)|
																(((int32_t)data[4])<<8)|
																(((int32_t)data[5])));
										StartMove(SHD_DIR_FWD, pos);
										sendBuf8(	adres, 0x61, &data[2]);	
										break; 
										}
										case 0x62: {
										int32_t pos = ((((int32_t)data[2])<<24)|
																(((int32_t)data[3])<<16)|
																(((int32_t)data[4])<<8)|
																(((int32_t)data[5]))); 
										StartMove(SHD_DIR_REV, pos);
										sendBuf8(	adres, 0x62, &data[2]);	
										break; 

										}
										case 0x63: {
										StopMove();  
										sendBuf8(	adres, 0x63, &data[2]);	
										break; 

										}
										
										case 0x64: {
											uint32_t dataVal = getDipValue(); 
											to32InBuf(&data[2], dataVal);  
											sendBuf8(	adres, 0x64, &data[2]); 
											break; 
										}
										
										case 0x65: {
											uint32_t dataVal = getSensorRead(); 
											to32InBuf(&data[2], dataVal);  
											sendBuf8(	adres, 0x65, &data[2]); 
											break; 
										}
										
										case 0x66: {
											
											to32InBuf(&data[2], 0x12345678);  
											sendBuf8(	adres, 0x66, &data[2]); 
											break; 
										}
										
										case 0x70: {
											
											uint32_t timeUs = ((((uint32_t)data[2])<<24)|
																(((uint32_t)data[3])<<16)|
																(((uint32_t)data[4])<<8)|
																(((uint32_t)data[5]))); 
											SetSpeed(timeUs); 
											sendBuf8(	adres, 0x70, &data[2]); 
											break; 
										}
										case 0x71: {

												initL6470Hw(data[5]&0x07, TOK_DEFAULT);
												sendBuf8(	adres, 0x71, &data[2]); 
												break; 
										}
										// set ACC
										case 0x72: {
												uint32_t a_data = ((((uint32_t)data[2])<<24)|
																(((uint32_t)data[3])<<16)|
																(((uint32_t)data[4])<<8)|
																(((uint32_t)data[5]))); 
												setAcc(a_data); 
												sendBuf8(	adres, 0x72, &data[2]); 
												break; 
										}
										
										// set Vmax
										case 0x74: {
												uint32_t a_data = ((((uint32_t)data[2])<<24)|
																(((uint32_t)data[3])<<16)|
																(((uint32_t)data[4])<<8)|
																(((uint32_t)data[5]))); 
												setVmax(a_data); 
												sendBuf8(	adres, 0x74, &data[2]); 
												break; 
										}
										
										case 0x75: {
												uint32_t a_data = ((((uint32_t)data[2])<<24)|
																(((uint32_t)data[3])<<16)|
																(((uint32_t)data[4])<<8)|
																(((uint32_t)data[5]))); 
												setVmin(a_data); 
												sendBuf8(	adres, 0x75, &data[2]); 
												break; 
										}
										
										
										case 0x80: {
											uint8_t sel = data[5];   
											to32InBuf(&data[2], GetSpeed(sel)); 
											sendBuf8(	adres, 0x80, &data[2]); 
											break; 
										}
										case 0x81: {
											uint8_t rdBuf[4];  
											readRegister(dSPIN_STEP_MODE, rdBuf);  
											sendBuf8(	adres, 0x81, rdBuf ); 
											break; 
										}
										case 0x82: {
											to32InBuf(&data[2], getAcc()); 
											sendBuf8(	adres, 0x82, &data[2]); 
											break; 
										}
										case 0x83: {
											to32InBuf(&data[2], getC0()); 
											sendBuf8(	adres, 0x83, &data[2]);
											break; 
										}
										case 0x84: {
											to32InBuf(&data[2], getVmax()); 
											sendBuf8(	adres, 0x84, &data[2]);
											break; 
										}
										case 0x85: {
											to32InBuf(&data[2], getVmin()); 
											sendBuf8(	adres, 0x85, &data[2]);
											break; 
										}
										case 0x86: {
											to32InBuf(&data[2], getCountSteToVmax()); 
											calcCn(); 
											sendBuf8(	adres, 0x86, &data[2]);
											break; 
										}
										
																
									}
											
									/*switch(data[1])
									{
												case '0': {  break; }
												case '1': {  break; }
												case '2': {  uint8_t mas[4]={0,0,0,0}; 
																			uint8_t adr = 9;
																			uint8_t com = 2; 
																			
																			getReadSpiData(mas); 
																			xQueueSendToBack(*pSendList, &adr, 0); 
																			xQueueSendToBack(*pSendList, &com, 0);
																			xQueueSendToBack(*pSendList, &mas[0], 0);
																			xQueueSendToBack(*pSendList, &mas[1], 0);
																			xQueueSendToBack(*pSendList, &mas[2], 0);
																			xQueueSendToBack(*pSendList, &mas[3], 0);break; }
												case '3': {
																	sendSpiByte(0x20|data[2]); 
																	for(int i=0; i<(int)data[3]; i++)
																	{
																			sendSpiByte(0xFF); 
																	}
																	uint8_t mas[4]={0,0,0,0}; 
																			uint8_t adr = 9;
																			uint8_t com = 2; 
																			
																			getReadSpiData(mas); 
																			xQueueSendToBack(*pSendList, &adr, 0); 
																			xQueueSendToBack(*pSendList, &com, 0);
																			xQueueSendToBack(*pSendList, &mas[0], 0);
																			xQueueSendToBack(*pSendList, &mas[1], 0);
																			xQueueSendToBack(*pSendList, &mas[2], 0);
																			xQueueSendToBack(*pSendList, &mas[3], 0);

																	break; }
												case '4': { fill('9','4','5'); break; }
												case '5': { fill('9','5','6'); break; }
												case '6': { fill('9','6','7'); break; }
												case '7': { fill('9','7','8'); break; }
												case '8': { fill('9','8','9'); break; }
												default: { fill('9','9','9'); break; }
												
									}*/
									//ShotLed(LED3); 
							}
							else 
							{
									for(int i=0; i<congigCOUNT_BYTE_MESSAGE; i++)
									xQueueSend(*pSendList, &data[i], 0);
							}
					}
					#if (configMODE_ALLIGN_6_BAIT == DEF_ON)
							if(uxQueueMessagesWaiting(*pReciveList)!=0) { xQueueReset(*pReciveList); } 
					#else 
					#endif // (configMODE_ALLIGN_6_BAIT == DEF_ON)
				
			 #endif // ( configDEBUG_SEND_ECHO == DEF_ON) 
					taskYIELD(); 
		}
		vTaskDelete(NULL); 
}







void vApplicationIdleHook (void)
{
	
}

uint32_t tempDel(uint32_t a, uint32_t b)
{
	return  a/b; 
} 



int main(void)
{
	SystemInit ();	
	
	if(initClk()==0)
	{
		 //IndicateError();
	}

	
	DisableIrqDma(); 
 
	
	Ispravnost = 1; 
	#if (TRACE_ON==DEF_ON)
	vTraceEnable(TRC_INIT); 
	vTraceEnable(TRC_START); 
	#endif
	//***********  init   ***************

	//led 
	if(LedInit()!=1) { Ispravnost = 0;} 
	// uart
	xPort = xSerialPortInitMinimal(115200); 
	if(xPort==(xComPortHandle)0) { Ispravnost = 0;} 
	pCounterSemaphor = GetCounterSemaphor(); 
	pReciveList = GetReciveList(); 
	pSendList = GetSendList(); 
	
	//SPI
	initSpiRtos(); 
	initL6470Hw(STEP_DEFAULT, TOK_DEFAULT);
	initCalcDrive(STEP_DEFAULT); 
  initPtrShdInfo(GetShdInfo()); 
	initInput(); 
	



	
	if(xTaskCreate(vObmenTask, "ObmenUart",  configMINIMAL_STACK_SIZE*2, NULL, PRIORITY_TASK_OBMEN_SERIAL, (TaskHandle_t *) NULL)!=pdTRUE) { Ispravnost = 0; }
	#if (TRACE_ON==DEF_ON)
	OutEnab = xSemaphoreCreateBinary(); 
	if(xTaskCreate(vSendTrace, "SendTrace",  configMINIMAL_STACK_SIZE*2, NULL, 2, (TaskHandle_t *) NULL)!=pdTRUE) { Ispravnost = 0; }
	#endif //(TRACE_ON==DEF_ON)
	
	
	// run dispetcher
	 if(Ispravnost==1){	
			vTaskStartScheduler();
	 }

	
	 LedOn(LED1); 
	 LedOn(LED2); 
	 LedOn(LED3); 
		return 0; 
}



