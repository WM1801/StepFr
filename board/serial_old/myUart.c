#include "myUart.h"





uint16_t m_echoBytesCounter = 0;


static PORT_InitTypeDef PortInit;
static UART_InitTypeDef UART_InitStructure;
uint32_t uart2_IT_TX_flag = RESET;
uint32_t uart2_IT_RX_flag = RESET;
uint32_t uart2_IT_RX_timeout = RESET; 

#define SIZE_RECIVE_BUF 1300
uint16_t ReciveData[SIZE_RECIVE_BUF];

uint16_t SizeRxMesg; 


uint16_t CurrentModeTx; 

// rx 
uint8_t DataRx[SIZE_RECIVE_BUF]; 
uint16_t counterRxData; 
uint8_t StatusUart = 0; 


// tx 
uint8_t DataTx[30]; 
uint16_t SizeDataTx = 0;
uint16_t SizeDataRx = 0;
uint8_t flagEndSend; 

uint8_t NewFlag; 

uint16_t GetNewFlag(void)
{
		if(NewFlag==1)
		{
			 NewFlag = 0; 
			return 1; 
		}
		else 
			return 0; 
}

void UartDirPin2(void)
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
	SetModeRx2(); 
}

void SetModeRx2(void)
{
		
		PORT_ResetBits(MDR_PORTE, PORT_Pin_0);
		counterRxData = 0; 
}

void SetModeTx2(void)
{
		PORT_SetBits(MDR_PORTE, PORT_Pin_0);
}


uint16_t GetSizeRxData(void)
{
		return counterRxData; 
}

uint8_t *GetReadBuffer(void)
{
		return DataRx; 
}

void ResetCounterRxData(void)
{
		counterRxData = 0; 
		 
}




void UartInit(void)
{
		UartDirPin(); 
	
		RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);
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
		
		
	
		// Configure PORTF pins 0 (UART1_RX) as input 
    PortInit.PORT_OE = PORT_OE_IN;
    PortInit.PORT_Pin = PORT_Pin_0;
    PORT_Init(MDR_PORTF, &PortInit);
		
		// Configure PORTF pins 1 (UART1_TX) as output 
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = PORT_Pin_1;
    PORT_Init(MDR_PORTF, &PortInit);
		
		// Enables the CPU_CLK clock on UART1,UART2 
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);
		
		  // Set the HCLK division factor = 1 for UART1,UART2 
    UART_BRGInit(MDR_UART2, UART_HCLKdiv1);
		
		 NVIC_EnableIRQ(UART2_IRQn);
		
		// Initialize UART_InitStructure 
    UART_InitStructure.UART_BaudRate            = 115000;
    UART_InitStructure.UART_WordLength          = UART_WordLength8b;
    UART_InitStructure.UART_StopBits            = UART_StopBits1;
    UART_InitStructure.UART_Parity              = UART_Parity_No;
    UART_InitStructure.UART_FIFOMode            = UART_FIFO_OFF;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_TXE|UART_HardwareFlowControl_RXE;
		
		UART_Init (MDR_UART2,&UART_InitStructure);
		
		
//		UART_IT_OE  = ((uint32_t)0x0400), /*!< Buffer overflow interrupt (UARTOEINTR) */
//    UART_IT_BE  = ((uint32_t)0x0200), /*!< Line break interrupt (UARTBEINTR) */
//    UART_IT_PE  = ((uint32_t)0x0100), /*!< Parity error interrupt (UARTPEINTR) */
//    UART_IT_FE  = ((uint32_t)0x0080), /*!< Frame structure error interrupt (UARTFEINTR) */
//    UART_IT_RT  = ((uint32_t)0x0040), /*!< Data input timeout interrupt (UARTRTINTR) */
//    UART_IT_TX  = ((uint32_t)0x0020), /*!< Transmitter interrupt (UARTTXINTR) */
//    UART_IT_RX 
		
		 // Enable Receiver interrupt 
    UART_ITConfig (MDR_UART2, UART_IT_RX, ENABLE);	
		// Enables UART2 peripheral 
    UART_Cmd(MDR_UART2,ENABLE); 
}


	
int UartFlagRxIrqReset(void)
{
		if(uart2_IT_RX_flag == SET)
		{
			uart2_IT_RX_flag = RESET;
				return 1; 
		}
		else
		{		
			return 0;
		}
}




void ReadInBuf(uint16_t data)
{
		DataRx[counterRxData] = (uint8_t)data; 
		counterRxData++; 	
}





uint16_t GetCurrentModeTx(void)
{
		return CurrentModeTx; 
}

void send8(uint8_t data)
{
		CurrentModeTx = 1; 
		SetModeTx();
		UART_SendData(MDR_UART2, data); 
		while(UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE)!=SET); 
		while(MDR_UART2->FR & UART_FR_BUSY) {;}
		SetModeRx(); 		
		CurrentModeTx = 0; 
}


void send16(uint16_t d)
{
		uint8_t data = (uint8_t)(d>>8); 
		send8(data); 
		data = (uint8_t)(d&0xFF); 
		send8(data); 
	
}

void answerCom(uint8_t adr, uint8_t com)
{
		uint8_t data[2]; 
		data[0]=adr; 
		data[1]=com; 
		SendUart(data, 2); 
}	

void answer1X8(uint8_t adr, uint8_t com, uint8_t d1)
{
		uint8_t data[3]; 
		data[0]=adr; 
		data[1]=com;
		data[2]=d1; 
		SendUart(data, 3); 	
}
void answer2X8(uint8_t adr, uint8_t com ,uint8_t d1, uint8_t d2)
{
		uint8_t data[4]; 
		data[0]=adr; 
		data[1]=com;
		data[2]=d1; 
		data[3]=d2; 
		SendUart(data, 4); 
}

void answer4X8(uint8_t adr, uint8_t com ,uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4)
{
		uint8_t data[6]; 
		data[0]=adr; 
		data[1]=com;
		data[2]=d1; 
		data[3]=d2; 
		data[4]=d3; 
		data[5]=d4;
		SendUart(data, 6); 
}


void answer1X32(uint8_t adr, uint8_t com ,uint32_t d)
{
		uint8_t data[6]; 
		data[0]=adr; 
		data[1]=com;
		data[2]=(uint8_t)(d>>24)&0xFF; 
		data[3]=(uint8_t)(d>>16)&0xFF; 
		data[4]=(uint8_t)(d>>8)&0xFF;
		data[5]=(uint8_t)d&0xFF;
		SendUart(data, 6); 
	
}




void answerBuf(uint8_t adr, uint8_t com, uint8_t * buf, uint32_t size)
{
		uint8_t data[2]; 
		data[0]=adr; 
		data[1]=com; 
		SendUart(data, 2); 
		SendUart(buf, size); 
}

void SendUart(uint8_t * data, uint16_t index)
{
		CurrentModeTx = 1; 
	
		if(index>0)
		{
				SizeDataTx = index; 
				memcpy(DataTx, data, index); 
				SetModeTx();
				for(int i=0; i<index; i++)
				{
						UART_SendData(MDR_UART2, DataTx[i]);
						while(UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE)!=SET); 
					
				}			
		}
		while(MDR_UART2->FR & UART_FR_BUSY) {;}
		SetModeRx(); 		
		CurrentModeTx = 0; 
}

void SendRow(uint8_t * data, uint16_t indexSt)
{
		CurrentModeTx = 1; 
	  uint16_t index = indexSt; 
		if(index>0)
		{
				SizeDataTx = index; 
				memcpy(DataTx, data, index); 
				SetModeTx();
				for(int i=0; i<index; i++)
				{
						UART_SendData(MDR_UART2, DataTx[i]);
						while(UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE)!=SET); 
					
				}			
		}
		DataTx[0]=(uint8_t)0xD;
		DataTx[1]=(uint8_t)0xA;
		for(int i=0; i<2; i++)
		{
				UART_SendData(MDR_UART2, DataTx[i]);
				while(UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE)!=SET); 
					
		}
		while(MDR_UART2->FR & UART_FR_BUSY) {;}
		SetModeRx(); 		
		CurrentModeTx = 0; 
}
















