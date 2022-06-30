#include "Reset.h"



void softReset()
{
	RST_CLK_PCLKcmd(RST_CLK_PCLK_WWDG, ENABLE);
	MDR_WWDG->CFR = 0x00;
	MDR_WWDG->CR = 0xFF;
	MDR_WWDG->CR = 0xFF;
}






