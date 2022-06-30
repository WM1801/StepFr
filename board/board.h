#ifndef BOARD_H
#define BOARD_H



#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_iwdg.h"
#include "MDR32F9Qx_dma.h"


//***********************************
// example user_guide armcc
//**********************************
//typedef struct {
//int i : 1;
//int j : 2;
//int k : 1;
//} BB;
//BB value;
//void update_value(void)
//{
//value.i = 1;
//value.k = 1;
//}
//******************************
//#pragma anon_unions
//typedef union {
//int i, j;
//} U; // U identifies a reusable anonymous union.
//struct A {
//U; // Okay -- references to A::i and A::j are allowed.
//};
//int foo (struct A *a) {
//return a->i;
//}
//Note
//The example above does not work in C++.
//*********************************************
//struct X
//{
//int x:10;
//int y:20;
//};




void reset_cpu(void); 
void DisableIrqDma(void); 
void SetInputMode(MDR_PORT_TypeDef* MDR_PORTx, uint32_t PORT_Pin, uint32_t CLK ); 
void SetOutputMode(MDR_PORT_TypeDef* MDR_PORTx, uint32_t PORT_Pin, uint32_t CLK ); 
	

typedef enum {
    SHD_DIR_FWD   =((uint32_t)0x01),
    SHD_DIR_REV   =((uint32_t)0x00)
} SHD_dir;

/*typedef struct {
    uint16_t timeUs; 
    uint16_t timeMs; 
} SHD_speed;*/

typedef struct{
uint32_t speed; 
SHD_dir dir; 
}SHD_step;

typedef struct{
	int runStop:1; // trebuetsya ostanovka 
	int b1:1;
	int b2:1;
	int b3:1;
	int b4:1;
	int b5:1;
	int b6:1;
	int b7:1;
	int b8:1;
	int b9:1;
	int b10:1;
	int b11:1;
	int b12:1;
	int b13:1;
	int b14:1;
	int b15:1;
}SHD_flag; 

typedef enum {
    SHD_MOVE_STOP   =((uint8_t)0x01),
    SHD_MOVE_ACC   =((uint8_t)0x02),
		SHD_MOVE_RUN   =((uint8_t)0x04),
		SHD_MOVE_DCC   =((uint8_t)0x08)
} SHD_MOVE_STATE;

typedef struct{
	SHD_MOVE_STATE state:8; 
	int b8:1;
	int b9:1;
	int b10:1;
	int b11:1;
	int b12:1;
	int b13:1;
	int b14:1;
	int b15:1;
}SHD_status; 




typedef struct {
/*32*/		int32_t valueStep; // tekushee znachenie shaga
/*32*/		int32_t targetStep; // zadannoe znachenie shaga
/*2x32*/	SHD_step nextVal;  // novoe znachenie dlya zapisi
/*2x32*/	SHD_step currentVal;  // tekushee znachenie vypolnyaemoe
/*2x32*/	SHD_step oldVal; // proshloe zapisannoe znachenie v taimer 
/*16*/		SHD_flag flag; 
/*16*/		SHD_status status;
/*32*/	  uint32_t Acc;     // step/sec2
/*32*/	  uint32_t C0; 			// us
/*32*/    uint32_t Dcc;     // step/sec2
/*32*/    uint32_t VminUs;  //us
/*32*/	  uint32_t VmaxUs;  //us
/*32*/		uint32_t Vmin;		// step/sec2
/*32*/		uint32_t Vmax;		// step/sec2
/*32*/    uint32_t countStepToVmax; 
/*32*/		uint32_t n; 
}SHD_Info; 
; 






// TIMER
#define UART_TIMER 	    		MDR_TIMER2
#define UART_TIMER_CLK   		RST_CLK_PCLK_TIMER2
#define UART_TIMER_IRQ 	    Timer2_IRQn
// void Timer2_IRQHandler(void)    serial.c

#define L6470_TIMER					MDR_TIMER3
#define L6470_TIMER_CLK 		RST_CLK_PCLK_TIMER3
#define L6470_TIMER_IRQ			Timer3_IRQn
// void Timer3_IRQHandler(void)     L6470.c

//ERROR_TYPE
//L6470
#define ER_L6470_CREATE_TMR   							0x01
//SPI
#define ER_SPI_CREATE_SEND_TASK   					0x01
#define ER_SPI_CREATE_SEMAPH_SEND						0x02
#define ER_SPI_CREATE_QUEUE_SEND 						0x03
#define ER_SPI_CREATE_QUEUE_READ						0x04
//CALC
#define ER_WRITE_CALK_TIME									0x01
#define ER_CREATE_CALK_TASK									0x02
#define ER_CREATE_SEMAFOR 									0x03
#define ER_CREATE_RST_TMR_TASK 							0x04
#define ER_CREATE_SEMAFOR_STOP              0x05

typedef enum {
	HOME = 3,    // 
	PL   = 6, 			 // 	
	NL   = 7				 //	signal  on board  
} SENSOR; 



typedef enum {
	LED1 = 1,    // 
	LED2 = 2, 			 // 	
	LED3 = 4				 //	signal LED on board  
} LEDs; 



typedef enum {
	DIP1 = 1, 
	DIP2,
	DIP3,
	DIP4,
	DIP5,
	DIP6,
	DIP7,
	DIP8,
	DIP9,
	DIP10,
	DIP11,
	DIP12
} DIPs; 

// uart 

#define USE_UART_CLK 					RST_CLK_PCLK_UART2
#define USE_UART							MDR_UART2
#define USE_UART_IRQ					UART2_IRQn


//*************************************************************************************
// PORT F 

//RS485
#define RS485_RX_PIN  		PORT_Pin_0
#define RS485_TX_PIN  		PORT_Pin_1
#define RS485_RXTX_PORT   MDR_PORTF
#define RS485_RXTX_CLK    RST_CLK_PCLK_PORTF

#define F_PINS		(RS485_RX|RS485_TX) 

//**************************************************************************************
//port E

//NL (input)   
// BOARD INPUT = 0 -> 74hct14d OUT -> MK INPUT = 1 
#define SENSOR_NL_PIN 		PORT_Pin_7
#define SENSOR_NL_PORT 		MDR_PORTE
#define SENSOR_NL_CLK			RST_CLK_PCLK_PORTE

//PL (input)   
// BOARD INPUT = 0 -> 74hct14d OUT -> MK INPUT = 1 
#define SENSOR_PL_PIN		  PORT_Pin_6
#define SENSOR_PL_PORT 	  MDR_PORTE
#define SENSOR_PL_CLK     RST_CLK_PCLK_PORTE

//HOME (input)
// BOARD INPUT = 0 -> 74hct14d OUT -> MK INPUT = 1 
#define SENSOR_HOME_PIN   PORT_Pin_3
#define SENSOR_HOME_PORT  MDR_PORTE
#define SENSOR_HOME_CLK   RST_CLK_PCLK_PORTE

//LED2 (out)				
// LEDon set 0, LEDoff set 1
#define LED2_PIN   				PORT_Pin_2
#define LED2_PORT  				MDR_PORTE
#define LED2_CLK   				RST_CLK_PCLK_PORTE

//LED1 (out)				
// LEDon set 0, LEDoff set 1
#define LED1_PIN   				PORT_Pin_1
#define LED1_PORT  				MDR_PORTE
#define LED1_CLK   				RST_CLK_PCLK_PORTE

//DIR  (out)			
// 1 mode tx, 0 mode rx 
#define RS485_DIR_PIN     PORT_Pin_0
#define RS485_DIR_PORT    MDR_PORTE
#define RS485_DIR_CLK     RST_CLK_PCLK_PORTE

#define E_PINS (SENSOR_NL_PIN|SENSOR_PL_PIN|SENSOR_HOME_PIN|LED2_PIN|LED1_PIN|RS485_DIR_PIN)

//***********************************************************************************


//port D 
//LED (out)          // LEDon set 1, LEDoff set 0
#define LED3_PIN   				PORT_Pin_7
#define LED3_PORT  				MDR_PORTD
#define LED3_CLK   				RST_CLK_PCLK_PORTD

//SDO (out) master out data, slave input
#define SDO_PIN 	  			PORT_Pin_6
#define SDO_PORT 					MDR_PORTD
#define SDO_CLK						RST_CLK_PCLK_PORTD

//CK (out) SPI clock
#define CK_SPI_PIN 	      PORT_Pin_5
#define CK_SPI_PORT 		  MDR_PORTD
#define CK_SPI_CLK			  RST_CLK_PCLK_PORTD


#define D_PINS  (LED3_PIN| SDO_PIN| CK_SPI_PIN) 

//***********************************************************************************
//port C
//CS (out)  chip select spi
#define CS_PIN 		  PORT_Pin_0
#define CS_PORT 		MDR_PORTC
#define CS_CLK			RST_CLK_PCLK_PORTC

//SDI (input) slave out data
#define SDI_PIN 	  PORT_Pin_2
#define SDI_PORT 		MDR_PORTC
#define SDI_CLK			RST_CLK_PCLK_PORTC

#define C_PINS  (CS_PIN|SDI_PIN) 

//*************************************************************************************
// port B
//DIP9  (input)
#define DIP9_PIN 		  PORT_Pin_10
#define DIP9_PORT 		MDR_PORTB
#define DIP9_CLK			RST_CLK_PCLK_PORTB

//DIP10 (input)
#define DIP10_PIN 		PORT_Pin_9
#define DIP10_PORT 		MDR_PORTB
#define DIP10_CLK			RST_CLK_PCLK_PORTB

//DIP11 (input)
#define DIP11_PIN 		PORT_Pin_8
#define DIP11_PORT 		MDR_PORTB
#define DIP11_CLK			RST_CLK_PCLK_PORTB

//DIP12 (input)
#define DIP12_PIN 		PORT_Pin_7
#define DIP12_PORT 		MDR_PORTB
#define DIP12_CLK			RST_CLK_PCLK_PORTB

//STCK  (out)
#define STCK_PIN 		  PORT_Pin_5
#define STCK_PORT 		MDR_PORTB
#define STCK_CLK			RST_CLK_PCLK_PORTB

//RST_L6470   (out)
#define RST_PIN 		  PORT_Pin_3
#define RST_PORT 			MDR_PORTB
#define RST_CLK				RST_CLK_PCLK_PORTB

//DIRST (out)
#define DIRST_PIN     PORT_Pin_2
#define DIRST_PORT 		MDR_PORTB
#define DIRST_CLK			RST_CLK_PCLK_PORTB

//FLAG   (input)
#define FLAG_PIN 		  PORT_Pin_1
#define FLAG_PORT 		MDR_PORTB
#define FLAG_CLK			RST_CLK_PCLK_PORTB

//SYNC   (input)
#define SYNC_PIN 		  PORT_Pin_0
#define SYNC_PORT 		MDR_PORTB
#define SYNC_CLK			RST_CLK_PCLK_PORTB


#define B_PINS  (DIP9_PIN|DIP10_PIN|DIP11_PIN|DIP12_PIN|STCK_PIN|RST_PIN|DIRST_PIN|FLAG_PIN|SYNC_PIN)
//*************************************************************************************
// port A
//DIP1 (input)
#define DIP1_PIN 		  PORT_Pin_0
#define DIP1_PORT 		MDR_PORTA
#define DIP1_CLK			RST_CLK_PCLK_PORTA

//DIP2 (input)
#define DIP2_PIN 		  PORT_Pin_1
#define DIP2_PORT 		MDR_PORTA
#define DIP2_CLK			RST_CLK_PCLK_PORTA

//DIP3  (input)
#define DIP3_PIN 		  PORT_Pin_2
#define DIP3_PORT 		MDR_PORTA
#define DIP3_CLK			RST_CLK_PCLK_PORTA

//DIP4  (input)
#define DIP4_PIN 		  PORT_Pin_3
#define DIP4_PORT 		MDR_PORTA
#define DIP4_CLK			RST_CLK_PCLK_PORTA

//DIP5  (input)
#define DIP5_PIN 		  PORT_Pin_4
#define DIP5_PORT 		MDR_PORTA
#define DIP5_CLK			RST_CLK_PCLK_PORTA

//DIP6  (input)
#define DIP6_PIN 		  PORT_Pin_5
#define DIP6_PORT 		MDR_PORTA
#define DIP6_CLK			RST_CLK_PCLK_PORTA

//DIP7  (input)
#define DIP7_PIN 		  PORT_Pin_6
#define DIP7_PORT 		MDR_PORTA
#define DIP7_CLK			RST_CLK_PCLK_PORTA

//DIP8  (input)
#define DIP8_PIN 		  PORT_Pin_7
#define DIP8_PORT 		MDR_PORTA
#define DIP8_CLK			RST_CLK_PCLK_PORTA

#define A_PINS  (DIP1_PIN|DIP2_PIN|DIP3_PIN|DIP4_PIN|DIP5_PIN|DIP6_PIN|DIP7_PIN|DIP8_PIN) 

//************************************************************************************


#endif 


