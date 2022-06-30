#include "serial.h"


#define portHandle (xComPortHandle)2; 
//save status config and error
uint16_t StatusSerial; 

xSemaphoreHandle counterSemaphor; 
#define MAX_COUNT_S 24 // size counterSemaphor 
#define DEFAULT_VALUE 0

static UART_InitTypeDef UART_InitStructure;
static PORT_InitTypeDef PortInit;
uint16_t flagModeRx; // 1 - mode Rx, 0 - mode Tx 


QueueHandle_t reciveList; 
#define SIZE_RECIVELIST 200
QueueHandle_t sendList; 
#define SIZE_SENDLIST 200 



//*********************************************************
void vSendTask(void *pvParameters); 


//**configure pins**
void UartDirPin(void);
void SetModeRx(void); 
void SetModeTx(void); 
void UartRxTxPin(void); 
//*******************
int send8(uint8_t data); 

QueueHandle_t* GetReciveList(void)
{
		return &reciveList; 
}

QueueHandle_t* GetSendList(void)
{
		return &sendList; 
}

xSemaphoreHandle* GetCounterSemaphor(void)
{
		return &counterSemaphor; 
}


void vSendTask(void *pvParameters)
{
	for(;;)
	{
		
		#if ( configDEBUG_SEND_ECHO == DEF_ON)
		char b = 0;
		while(xQueueReceive(sendList, &b, portMAX_DELAY)==pdPASS)
		{
				send8(b); 
				ShotLed(LED1); 
		}
		
		#else
			char b = 0;
			if(uxQueueMessagesWaiting(sendList)>=congigCOUNT_BYTE_MESSAGE)
			{
					int i = congigCOUNT_BYTE_MESSAGE; 
					while(i>0)
					{
						i--; 
						if(xQueueReceive(sendList, &b, portMAX_DELAY)==pdPASS)
						{
							send8(b); 
							ShotLed(LED3); 
						}
					}
			}
				taskYIELD();
		#endif 
	}
	vTaskDelete(NULL); 
	
}







xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud)//, unsigned portBASE_TYPE uxQueueLength )
{
		xComPortHandle xReturn = portHandle;
		StatusSerial = 0; 
	
		UartDirPin(); 
		UartRxTxPin();
		
		
	
		sendList = xQueueCreate(SIZE_SENDLIST, ( unsigned portBASE_TYPE ) sizeof(uint8_t));
		reciveList = xQueueCreate(SIZE_RECIVELIST, ( unsigned portBASE_TYPE ) sizeof(uint8_t));
		if( ( sendList != NULL ) && ( reciveList != NULL ) )
		{
				// Enables the CPU_CLK clock on UART1,UART2 
				RST_CLK_PCLKcmd(USE_UART_CLK, ENABLE);
			
				// Set the HCLK division factor = 1 for UART1,UART2 
				UART_BRGInit(USE_UART, UART_HCLKdiv1);
				
				NVIC_EnableIRQ(USE_UART_IRQ);
		
				UART_StructInit(&UART_InitStructure);
				// Initialize UART_InitStructure 
				UART_InitStructure.UART_BaudRate            = ulWantedBaud;
				UART_InitStructure.UART_WordLength          = UART_WordLength8b;
				UART_InitStructure.UART_StopBits            = UART_StopBits1;
				UART_InitStructure.UART_Parity              = UART_Parity_No;
				UART_InitStructure.UART_FIFOMode            = UART_FIFO_OFF;
				UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_TXE|UART_HardwareFlowControl_RXE ;
				
				UART_Init (USE_UART,&UART_InitStructure);
			
				//		UART_IT_OE  = ((uint32_t)0x0400), /*!< Buffer overflow interrupt (UARTOEINTR) */
				//    UART_IT_BE  = ((uint32_t)0x0200), /*!< Line break interrupt (UARTBEINTR) */
				//    UART_IT_PE  = ((uint32_t)0x0100), /*!< Parity error interrupt (UARTPEINTR) */
				//    UART_IT_FE  = ((uint32_t)0x0080), /*!< Frame structure error interrupt (UARTFEINTR) */
				//    UART_IT_RT  = ((uint32_t)0x0040), /*!< Data input timeout interrupt (UARTRTINTR) */
				//    UART_IT_TX  = ((uint32_t)0x0020), /*!< Transmitter interrupt (UARTTXINTR) */
				//    UART_IT_RX 
		
				// Enable Receiver interrupt 
				UART_ITConfig (USE_UART, UART_IT_RX|UART_IT_OE, ENABLE);	
				// Enables UART2 peripheral 
				UART_Cmd(USE_UART,ENABLE); 
				SetModeRx(); 	
				
				StatusSerial = 1; // ok  
				//*******************************
				counterSemaphor = xSemaphoreCreateCounting(MAX_COUNT_S, DEFAULT_VALUE); 
				if(counterSemaphor == NULL)
				{
						xReturn = ( xComPortHandle ) 0;
				}
				else 
				{
						StatusSerial = StatusSerial<<1;
						if(xTaskCreate(vSendTask, "sendUart",  configMINIMAL_STACK_SIZE*2, NULL, PRIORITY_TASK_SEND_SERIAL, (TaskHandle_t *) NULL)!=pdTRUE) 
						{
								xReturn = ( xComPortHandle ) 0;
						}
						else 
						{
								StatusSerial = StatusSerial<<1;
						}
						UartTimerInit(); 
						
				}
				
		}
		else 
		{
				xReturn = ( xComPortHandle ) 0;
		}
		
		return xReturn; 
}

//**********configure pins****************************
void UartDirPin(void)
{
	
	PORT_StructInit(&PortInit);
	RST_CLK_PCLKcmd(RS485_DIR_CLK, ENABLE);
	PortInit.PORT_Pin = (PORT_Pin_All & ~(E_PINS)); 
	PORT_Init(MDR_PORTE, &PortInit);

	// Configure PORT  pins  for output to switch LEDs on/off 
	PortInit.PORT_Pin = (RS485_DIR_PIN); 
	PortInit.PORT_OE = PORT_OE_OUT; 
	PortInit.PORT_FUNC = PORT_FUNC_PORT; 
	PortInit.PORT_MODE = PORT_MODE_DIGITAL; 
	PortInit.PORT_SPEED = PORT_SPEED_FAST; 

	PORT_Init(RS485_DIR_PORT, &PortInit);
	SetModeRx(); 
	
}


void SetModeRx(void)
{
		PORT_ResetBits(RS485_DIR_PORT, RS485_DIR_PIN);
		flagModeRx = 1; 	
}

void SetModeTx(void)
{ 
		PORT_SetBits(RS485_DIR_PORT, RS485_DIR_PIN);
		flagModeRx = 0;  
}

void UartRxTxPin(void)
{
		RST_CLK_PCLKcmd(RS485_RXTX_CLK, ENABLE);
		PORT_StructInit(&PortInit);
	
		 // Fill PortInit structure 
    PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PortInit.PORT_PD = PORT_PD_DRIVER;
    PortInit.PORT_GFEN = PORT_GFEN_OFF;
    PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
    PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;
		
		
		// Configure PORTF pins 0 (UART_RX) as input 
    PortInit.PORT_OE = PORT_OE_IN;
    PortInit.PORT_Pin = RS485_RX_PIN;
    PORT_Init(RS485_RXTX_PORT, &PortInit);
		
		// Configure PORTF pins 1 (UART_TX) as output 
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = RS485_TX_PIN;
    PORT_Init(RS485_RXTX_PORT, &PortInit);
}

//**********************************************************************************

int send8(uint8_t data)
{ 
		
		int res = 0; 
		SetModeTx();
		UART_SendData(USE_UART, data); 
		while(UART_GetFlagStatus(USE_UART, UART_FLAG_TXFE)!=SET); 
		while(MDR_UART2->FR & UART_FR_BUSY) {;}												//////////////////////////////////!  MDR_UART2->FR
		res = 1; 
	  SetModeRx();
		return res; 
}


//************************************************************************************
//									irq
//************************************************************************************
void Timer2_IRQHandler(void)
{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		if(TIMER_GetITStatus(UART_TIMER, TIMER_STATUS_CNT_ARR) == SET)
		{
				TIMER_ClearITPendingBit(UART_TIMER, TIMER_STATUS_CNT_ARR); 
				StopUartTimer();	
				
				xSemaphoreGiveFromISR(counterSemaphor, &xHigherPriorityTaskWoken);
				
				
		}

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);	 
		
}





void UART2_IRQHandler(void)
{	 
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	char cChar;
				
	 if(UART_GetITStatusMasked(USE_UART, UART_IT_RX) == SET)
	 {
			UART_ClearITPendingBit(USE_UART, UART_IT_RX);
			cChar = (uint8_t) UART_ReceiveData(USE_UART);
			if(flagModeRx==1)
			{		 
					xQueueSendToBackFromISR(reciveList, &cChar, &xHigherPriorityTaskWoken);
					StartUartTimer();
							

					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);	 
				
					
			}
	 }
	
	if(UART_GetITStatusMasked(USE_UART, UART_IT_TX) == SET)
	{
			UART_ClearITPendingBit(MDR_UART2, UART_IT_TX);						
	}	

	if(UART_GetITStatusMasked(USE_UART, UART_IT_OE) == SET)
	{		
			UART_ClearITPendingBit(USE_UART, UART_IT_OE);
	}

	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}	

