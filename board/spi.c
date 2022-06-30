#include "spi.h"


#define USE_SPI           MDR_SSP2 
#define USE_SPI_CLK       RST_CLK_PCLK_SSP2
#define USE_CLD_SPI_DIV   SSP_HCLKdiv2 // 80/2 = 40Mhz

SSP_InitTypeDef sSSP;
static PORT_InitTypeDef PortInit;

int counterRecive; 


//*********HAL***************
void initSpiPin(void);
void initSpi_HW(void); 
// return 1 - ok send, 0 - not send
uint8_t sendSpi16(uint16_t data);
// return 1 - ok read, 0 - no data
uint8_t readSpi16(uint16_t *readData); 




//**************************
void initSpiPin(void)
 {
		//CS_PIN      -    C0  - OWERID SSP2_FSS
		//SDO_PIN     -    D6  - ALTER  SSP2_TXD
		//CK_SPI_PIN  -    D5  - ALTER  SSP2_CLK
		//SDI_PIN     -    C2  - OWERID SSP2_RXD
		
		
		//********* pin cs **********************
		RST_CLK_PCLKcmd(CS_CLK, ENABLE);
		PORT_StructInit(&PortInit);
	
		// Fill PortInit structure 
    PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
    PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;
		
		// Configure pin as output 
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = CS_PIN;
    PORT_Init(CS_PORT, &PortInit);
		
		//*********** SDO ************************		
		RST_CLK_PCLKcmd(SDO_CLK, ENABLE);

    PortInit.PORT_FUNC = PORT_FUNC_ALTER;		
		// Configure pin as output 
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = SDO_PIN;
    PORT_Init(SDO_PORT, &PortInit);
		
		//*********** CK_SPI ************************		
		RST_CLK_PCLKcmd(CK_SPI_CLK, ENABLE);
	
    PortInit.PORT_FUNC = PORT_FUNC_ALTER;	
		// Configure pin as output 
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = CK_SPI_PIN;
    PORT_Init(CK_SPI_PORT, &PortInit);
		
		//*********** SDI ************************		
		RST_CLK_PCLKcmd(SDI_CLK, ENABLE);	
		
		// Fill PortInit structure 
    PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
  	
		// Configure pin as output 
    PortInit.PORT_OE = PORT_OE_IN;
    PortInit.PORT_Pin = SDI_PIN;
    PORT_Init(SDI_PORT, &PortInit);
		
}
 
void initSpi_HW(void)
{
		RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP2, ENABLE); 
		/* Reset all SSP settings */
		SSP_DeInit(USE_SPI); 
		SSP_BRGInit(USE_SPI, USE_CLD_SPI_DIV); 
	
		/* SSP MASTER configuration ------------------------------------------------*/
		SSP_StructInit (&sSSP);

    sSSP.SSP_SCR  = 0x2; 
    sSSP.SSP_CPSDVSR = 5;// scr=2/ CPSDVSR = 5/ F_SSP_CLK = 2666667 
    sSSP.SSP_Mode = SSP_ModeMaster;
    sSSP.SSP_WordLength = SSP_WordLength8b;
    sSSP.SSP_SPH = SSP_SPH_2Edge;
    sSSP.SSP_SPO = SSP_SPO_High;
    sSSP.SSP_FRF = SSP_FRF_SPI_Motorola;
    sSSP.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;
    SSP_Init (USE_SPI,&sSSP);
		
		//SSP_ITConfig(USE_SPI, SSP_IT_RT, ENABLE);
		/* Enable SSP */
    SSP_Cmd(USE_SPI, ENABLE);
		
		NVIC_EnableIRQ(SSP2_IRQn);
	
}

uint8_t sendSpi16(uint16_t data){
 
	 //	SSP_SendData(USE_SPI, data);
		counterRecive = 0; 
		// buffer tx not full 
		if(SSP_GetFlagStatus(USE_SPI, SSP_FLAG_TNF)==SET) {	
				SSP_SendData(USE_SPI, data);
				return 1;  }
		else {
				return 0;  }
}

uint8_t readSpi16(uint16_t *readData) {
	 // buffer rx ne pust
	while(SSP_GetFlagStatus(USE_SPI, SSP_FLAG_RNE) ==SET) {
			*readData = SSP_ReceiveData(USE_SPI); 
			readData++; 
			counterRecive++; 	
	}
	return counterRecive;
}

uint8_t readSpi8(uint8_t *readData) {
	 // buffer rx ne pust
	while(SSP_GetFlagStatus(USE_SPI, SSP_FLAG_RNE) ==SET) {
			*readData = (uint8_t)SSP_ReceiveData(USE_SPI); 
			readData++; 
			counterRecive++; 	
	}
	return counterRecive;
}


void SSP2_IRQHandler(void)
{
		 // taimaut time = recive 32 bit
		if(SSP_GetITStatusMasked(USE_SPI, SSP_IT_RT)==SET)
		{
			  SSP_ClearITPendingBit(USE_SPI, SSP_IT_RT);
			 	//LedToggle(LED1); 
		}
}

void clearReadBufSpi(void)
{
		while(SSP_GetFlagStatus(USE_SPI, SSP_FLAG_RNE) ==SET) {
			uint16_t readData = SSP_ReceiveData(USE_SPI); 
	}
}

//***************************************************************************
void initSPI(void)
{
	initSpiPin(); 
	initSpi_HW(); 
}


