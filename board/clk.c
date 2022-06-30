#include "clk.h"

RST_CLK_FreqTypeDef CLK_FrequencyStructure; 


int initClk(void) // 8mhz (hse)
{
	// clk 
	RST_CLK_DeInit(); 
	/* Enable HSI clock source */
	RST_CLK_HSIcmd(ENABLE);
	if (RST_CLK_HSIstatus() == SUCCESS)                     /* Good HSI clock */
  {
			
			/* Enable HSE clock oscillator */
			RST_CLK_HSEconfig(RST_CLK_HSE_ON); // MDR_RST_CLK->HS_CONTROL  = 1 
			if(RST_CLK_HSEstatus() == SUCCESS) // MDR_RST_CLK->CLOCK_STATUS
			{
				 /* Select HSE clock as CPU_PLL input clock source */
				 /* Set PLL multiplier to                         */
					// input freq HSE / 1 * (4+1)  = kvarz 8mhz, 16/1*10 = 80mhz (max)
				 
				 //RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv2, RST_CLK_CPU_PLLmul15);//4); 60
				 RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);//4); // 80
				 RST_CLK_CPU_PLLcmd(ENABLE);
				 if (RST_CLK_HSEstatus() == SUCCESS)                     /* Good CPU PLL */
				 {
					   /* Set CPU_C3_prescaler to 2 */
						 RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1); // set CPU_C3_SEL (CPU_CLOCK)
						/* Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock */
             RST_CLK_CPU_PLLuse(ENABLE);  									// CPU_C2_SEL
						/* Select CPU_C3 clock on the CPU clock MUX */
             RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3); //CPU_HCLK_SEL (CPU_CLOCK)
				 }
				 else 
				 {
						return 0;
				 }
				
				 
		  }
			
	}
	else
  {                                                    /* HSI timeout */
    return 0;
  }
	
	return 1;

}	




int initClk8(void) // 8mhz (hse)
{
	// clk 
	RST_CLK_DeInit(); 
	/* Enable HSI clock source */
	RST_CLK_HSIcmd(ENABLE);
	if (RST_CLK_HSIstatus() == SUCCESS)                     /* Good HSI clock */
  {
			
			/* Enable HSE clock oscillator */
			RST_CLK_HSEconfig(RST_CLK_HSE_ON); // MDR_RST_CLK->HS_CONTROL  = 1 
			if(RST_CLK_HSEstatus() == SUCCESS) // MDR_RST_CLK->CLOCK_STATUS
			{
				 /* Select HSE clock as CPU_PLL input clock source */
				 /* Set PLL multiplier to                         */
					// input freq HSE / 1 * (4+1)  = kvarz 16mhz, 16/1*(4+1) = 80mhz (max)
				 RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv2, 0);//4); // set CPU_C1_SELclr (CPU_CLOCK) , PLLCPUmulclr (PLL_CONTROL)
					/* Enable CPU_PLL */
				 RST_CLK_CPU_PLLcmd(ENABLE);
				 if (RST_CLK_HSEstatus() == SUCCESS)                     /* Good CPU PLL */
				 {
					   /* Set CPU_C3_prescaler to 2 */
						 RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1); // set CPU_C3_SEL (CPU_CLOCK)
						/* Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock */
             RST_CLK_CPU_PLLuse(ENABLE);  									// CPU_C2_SEL
						/* Select CPU_C3 clock on the CPU clock MUX */
             RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3); //CPU_HCLK_SEL (CPU_CLOCK)
				 }
				 else 
				 {
						return 0;
				 }
				
				 
		  }
			
	}
	else
  {                                                    /* HSI timeout */
    return 0;
  }
	
	return 1;

}	




int initClk80(void)
{
	// clk 
	RST_CLK_DeInit(); 
	/* Enable HSI clock source */
	RST_CLK_HSIcmd(ENABLE);
	if (RST_CLK_HSIstatus() == SUCCESS)                     /* Good HSI clock */
  {
			
			/* Enable HSE clock oscillator */
			RST_CLK_HSEconfig(RST_CLK_HSE_ON); // MDR_RST_CLK->HS_CONTROL  = 1 
			if(RST_CLK_HSEstatus() == SUCCESS) // MDR_RST_CLK->CLOCK_STATUS
			{
				 /* Select HSE clock as CPU_PLL input clock source */
				 /* Set PLL multiplier to                         */
					// input freq HSE / 1 * (4+1)  = kvarz 16mhz, 16/1*(4+1) = 80mhz (max)
				 RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, 4); // set CPU_C1_SELclr (CPU_CLOCK) , PLLCPUmulclr (PLL_CONTROL)
					/* Enable CPU_PLL */
				 RST_CLK_CPU_PLLcmd(ENABLE);
				 if (RST_CLK_HSEstatus() == SUCCESS)                     /* Good CPU PLL */
				 {
					   /* Set CPU_C3_prescaler to 2 */
						 RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1); // set CPU_C3_SEL (CPU_CLOCK)
						/* Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock */
             RST_CLK_CPU_PLLuse(ENABLE);  									// CPU_C2_SEL
						/* Select CPU_C3 clock on the CPU clock MUX */
             RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3); //CPU_HCLK_SEL (CPU_CLOCK)
				 }
				 else 
				 {
						return 0;
				 }
				
				 
		  }
			
	}
	else
  {                                                    /* HSI timeout */
    return 0;
  }
	
	return 1;

}	


