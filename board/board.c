#include "board.h"

#define ALL_PORTS_CLK (RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | \
                       RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD | \
                       RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTF)
											 
void Init_All_Ports(void);

static PORT_InitTypeDef PORT_InitStructureLed; 
void reset_cpu(void)
{
		RST_CLK_PCLKcmd(RST_CLK_PCLK_IWDG, ENABLE);
		IWDG_Enable(); 
		IWDG_WriteAccessEnable(); 
		IWDG_SetPrescaler(IWDG_Prescaler_4); 
		while (IWDG_GetFlagStatus(IWDG_FLAG_PVU) != 1)
    {
			
    }
		IWDG_SetReload(0xF);
		
}

void Init_All_Ports(void)
{
    /* Enable the RTCHSE clock on all ports */
    RST_CLK_PCLKcmd(ALL_PORTS_CLK, ENABLE);

    /* Configure all ports to the state as after reset, i.e. low power consumption */
    PORT_StructInit(&PORT_InitStructureLed);
    PORT_Init(MDR_PORTA, &PORT_InitStructureLed);
    PORT_Init(MDR_PORTB, &PORT_InitStructureLed);
    PORT_Init(MDR_PORTC, &PORT_InitStructureLed);
    PORT_Init(MDR_PORTD, &PORT_InitStructureLed);
    PORT_Init(MDR_PORTE, &PORT_InitStructureLed);
    PORT_Init(MDR_PORTF, &PORT_InitStructureLed);

    /* Disable the RTCHSE clock on all ports */
    RST_CLK_PCLKcmd(ALL_PORTS_CLK, DISABLE);
}


void DisableIrqDma(void)
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP1, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_SSP2, ENABLE);
	/* Init NVIC */
  SCB->AIRCR = 0x05FA0000 | ((uint32_t)0x500);
  SCB->VTOR = 0x08000000;
  /* Disable all interrupt */
  NVIC->ICPR[0] = 0xFFFFFFFF;
  NVIC->ICER[0] = 0xFFFFFFFF;

  /* Disable all DMA request */
  MDR_DMA->CHNL_REQ_MASK_CLR = 0xFFFFFFFF;
  MDR_DMA->CHNL_USEBURST_CLR = 0xFFFFFFFF;
	Init_All_Ports(); 
}

void SetInputMode(MDR_PORT_TypeDef* MDR_PORTx, uint32_t PORT_Pin, uint32_t CLK )
{
	RST_CLK_PCLKcmd(CLK, ENABLE);
	PORT_StructInit(&PORT_InitStructureLed); 

	/* Configure PORT  pins  for output to switch LEDs on/off */
	PORT_InitStructureLed.PORT_Pin = (PORT_Pin); 
	PORT_InitStructureLed.PORT_OE = PORT_OE_IN; 
	PORT_InitStructureLed.PORT_FUNC = PORT_FUNC_PORT; 
	PORT_InitStructureLed.PORT_MODE = PORT_MODE_DIGITAL; 
	PORT_InitStructureLed.PORT_SPEED = PORT_SPEED_FAST; 
	PORT_Init(MDR_PORTx, &PORT_InitStructureLed);

}	
void SetOutputMode(MDR_PORT_TypeDef* MDR_PORTx, uint32_t PORT_Pin, uint32_t CLK )
{
	RST_CLK_PCLKcmd(CLK, ENABLE);
	PORT_StructInit(&PORT_InitStructureLed); 
	
	/* Configure PORT  pins  for output to switch LEDs on/off */
	PORT_InitStructureLed.PORT_Pin = (PORT_Pin); 
	PORT_InitStructureLed.PORT_OE = PORT_OE_OUT; 
	PORT_InitStructureLed.PORT_FUNC = PORT_FUNC_PORT; 
	PORT_InitStructureLed.PORT_MODE = PORT_MODE_DIGITAL; 
	PORT_InitStructureLed.PORT_SPEED = PORT_SPEED_FAST; 
	PORT_Init(MDR_PORTx, &PORT_InitStructureLed);
}	



