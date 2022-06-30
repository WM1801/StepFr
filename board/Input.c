#include "Input.h"

uint32_t dip;
uint32_t sensorValue; 


 
void initInput(void)
{
		SetInputMode(DIP1_PORT, 
								(DIP1_PIN|DIP2_PIN|DIP3_PIN|DIP4_PIN|DIP5_PIN|DIP6_PIN|DIP7_PIN|DIP8_PIN), 
								 DIP1_CLK );
	
		SetInputMode(DIP9_PORT,	 
								(DIP9_PIN|DIP10_PIN|DIP11_PIN|DIP12_PIN), 
								 DIP10_CLK );
	
		SetInputMode(SENSOR_HOME_PORT, SENSOR_HOME_PIN, SENSOR_HOME_CLK );
		SetInputMode(SENSOR_NL_PORT, SENSOR_NL_PIN, SENSOR_NL_CLK );
		SetInputMode(SENSOR_PL_PORT, SENSOR_PL_PIN, SENSOR_PL_CLK );
	
		dip = 0; 
	  sensorValue = 0; 
	
}



// signaly na plate invertirovany, i 9-11 peretasovany
uint16_t getDipValue(void)
{
		uint32_t temp7_0 = (PORT_ReadInputData(MDR_PORTA));
		temp7_0 = (~temp7_0)&0xFF; 
		uint32_t temp11_8 = PORT_ReadInputData(DIP12_PORT);
		temp11_8 = (~temp11_8)&0x780; 
	
	  if(temp11_8&DIP9_PIN){ temp7_0|= (DIP12_PIN<<1); }
		if(temp11_8&DIP10_PIN){ temp7_0|= (DIP11_PIN<<1); }
		if(temp11_8&DIP11_PIN){ temp7_0|= (DIP10_PIN<<1); }
		if(temp11_8&DIP12_PIN){ temp7_0|= (DIP9_PIN<<1); }
		return  temp7_0 ;  
}

uint32_t getSensorRead(void)
{
		sensorValue = (PORT_ReadInputData(SENSOR_HOME_PORT)&0xC8); 
		return sensorValue; 
}

uint32_t getSensorMask(SENSOR sens)
{
		return (sensorValue&sens);
}





