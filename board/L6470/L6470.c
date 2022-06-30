#include "L6470.h"

//    pins - schema -	datasheet   -  milandr            -    mode_L6470																-	 pullUp_schema        --    JTAG	
//PB3 - 3  -    rst   -   nRST       -   output            -  logical input( connected VCC) 							-			not           --   JA_TDI
//PB2 - 4  -    dir   -   SW					-   not use (input)  -  logical input(if not use, connected VCC)   -     not           --   JA_TCK
//PB0 - 22 -    sync	 -   nBUSY\SYNC -   input            -  open drain output                          -     10K           --   JA_TDO
//PB1 - 24 -    flag  -   nFLAG      -   input             -  open drain output                          -     10K            --   JA_TMS


// file L6470Timer, PWM
//PB5 - 25 -      -   STCK       -   output            - step-clock input                            -     not            --   -


//spi 18,19,20,23

static uint32_t ErrorCountL6470;
static uint32_t ErrorTypeL6470; 
static uint32_t StatusL6470;


uint8_t currentInit; 
uint8_t stepInit; 
static PORT_InitTypeDef PortInit;

//rtos 
#define TIME_RESET_L6470 50

static xTimerHandle RstL6470Tmr; 

dSPIN_Direction_TypeDef L6470Dir; 

uint16_t StateTask; 



#define TOK_DEFAULT 28 
//*****************************
// init HW
void initPin(void); 
void resetL6470(void); 
void addErrorL6470(uint8_t data); 
void addStatusL6470(uint32_t data); 


void SlotL6470Tmr( TimerHandle_t xTimer ); 

//*****************************
void initPin(void)
{
		//RESET 
		RST_CLK_PCLKcmd(RST_CLK, ENABLE);
		PORT_StructInit(&PortInit);
	
		// Fill PortInit structure 
    PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PortInit.PORT_PD = PORT_PD_DRIVER;
    PortInit.PORT_GFEN = PORT_GFEN_OFF;
    PortInit.PORT_FUNC = PORT_FUNC_PORT;
    PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;
		
		// Configure pin as output 
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = RST_PIN;
    PORT_Init(RST_PORT, &PortInit);
	  
	
		//SW  // not use 
		RST_CLK_PCLKcmd(DIRST_CLK, ENABLE);
		
		PortInit.PORT_PULL_UP = PORT_PULL_UP_ON; 
		PortInit.PORT_OE = PORT_OE_IN;
		PortInit.PORT_Pin = DIRST_PIN;
		PORT_Init(DIRST_PORT, &PortInit); 
		
		//SYNC
		RST_CLK_PCLKcmd(SYNC_CLK , ENABLE);
		
		PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF; 
		PortInit.PORT_OE = PORT_OE_IN;
		PortInit.PORT_Pin = SYNC_PIN;
		PORT_Init(SYNC_PORT, &PortInit); 
		
		//FLAG
		RST_CLK_PCLKcmd(FLAG_CLK , ENABLE);
		
		PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF; 
		PortInit.PORT_OE = PORT_OE_IN;
		PortInit.PORT_Pin = FLAG_PIN;
		PORT_Init(FLAG_PORT, &PortInit); 
			
		 // defaul value 
		PORT_SetBits(RST_PORT, RST_PIN);
		
}


void addStatusL6470(uint32_t data)
{
		StatusL6470 |= data;
}


//*********************************
void addErrorL6470(uint8_t data)
{
		ErrorCountL6470++; 
		ErrorTypeL6470<<=8; 
		ErrorTypeL6470|=data; 
}

void getErrorL6470(uint8_t *data)
{
		data[0] = (uint8_t)((ErrorTypeL6470>>24)&0xFF);
	  data[1] = (uint8_t)((ErrorTypeL6470>>16)&0xFF);
	  data[2] = (uint8_t)((ErrorTypeL6470>>8)&0xFF);
		data[3] = (uint8_t)(ErrorTypeL6470&0xFF);
}
void getCountErrorL6470(uint8_t *data)
{
		data[0] = (uint8_t)((ErrorCountL6470>>24)&0xFF);
	  data[1] = (uint8_t)((ErrorCountL6470>>16)&0xFF);
	  data[2] = (uint8_t)((ErrorCountL6470>>8)&0xFF);
		data[3] = (uint8_t)(ErrorCountL6470&0xFF);
}

void getStatusL6470(uint8_t *data)
{
		data[0] = (uint8_t)((StatusL6470>>24)&0xFF);
	  data[1] = (uint8_t)((StatusL6470>>16)&0xFF);
	  data[2] = (uint8_t)((StatusL6470>>8)&0xFF);
		data[3] = (uint8_t)(StatusL6470&0xFF);
}



void resetL6470(void){ 
		PORT_ResetBits(RST_PORT, RST_PIN);
		if(xTimerStart(RstL6470Tmr, 100)!=pdPASS)
		{
				addErrorL6470(ER_L6470_CREATE_TMR); 				
		}
}

void SlotL6470Tmr( TimerHandle_t xTimer )
{
		if(xTimer == RstL6470Tmr)
		{
				addStatusL6470(0x04);
				switch(StateTask)
				{
					case 0: {	PORT_SetBits(RST_PORT, RST_PIN);		
										addStatusL6470(0x08);
										StateTask = 1; 
										xTimerStart(RstL6470Tmr, 100); 
										break; }
					case 1: { initL6470Register(); 
										StateTask = 2; 
										addStatusL6470(0x020);
										break; }
					default:{}
										
				}
					 	
		}
}


void initL6470Hw(uint8_t mStep, uint8_t current)
{
	addStatusL6470(0x01); 
	initPin();
	initTimerL6470();
	ErrorCountL6470 =0; 
	ErrorTypeL6470 =0;	
	currentInit = current; 
	stepInit = mStep; 
	
	
	StateTask = 0; 	
	RstL6470Tmr = xTimerCreate("RstL6470", TIME_RESET_L6470, pdFALSE, (void*)0, SlotL6470Tmr);
	
	
	resetL6470();
	addStatusL6470(0x02);
}

//****************************************


void initL6470Register(void){
	
		uint8_t tok = currentInit; 
		writeRegister(dSPIN_KVAL_HOLD, &tok, 1); 
		writeRegister(dSPIN_KVAL_RUN, &tok, 1); 
		writeRegister(dSPIN_KVAL_ACC, &tok, 1); 
		writeRegister(dSPIN_KVAL_DEC, &tok, 1);
		uint8_t step = stepInit; 
		writeRegister(dSPIN_STEP_MODE, &step, 1); 
	
		L6470Dir = dSPIN_DIR_FWD;	
		stepClock(dSPIN_DIR_FWD); 
		addStatusL6470(0x10);
}


void stepL6470(uint32_t time)
{
		startTimerL6470(time);  
}
















