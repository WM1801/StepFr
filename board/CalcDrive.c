#include "CalcDrive.h"


// C0 = sqrt(2/acc)
//Cn = C0 *(sqrt(n+1)-sqrt(n))

//Cn = Cn-1 - ((2*Cn-1)/(4n+1))
// if n = 1 -> Cn = Cn-1 - ((2*Cn-1)/(4n+1))*0.676
QueueHandle_t timeList; 
#define SIZE_TIMELIST 50
uint32_t StatusCalc; 
SHD_Info Info; 

void reCalc(void); 

//void vCalcTask(void *pvParameters);
//void vRestartTmrTask(void *pvParameters); 
void setStatusCalc(uint8_t data); 


//xSemaphoreHandle xSemTimer;


void initCalcDrive(uint8_t sel )
{
	timeList = xQueueCreate(SIZE_TIMELIST, ( unsigned portBASE_TYPE ) sizeof(SHD_step));
	StatusCalc = 0;
	
	
	//vSemaphoreCreateBinary(xSemTimer);
	//if(xSemTimer==NULL)
	//{
	//		setStatusCalc(ER_CREATE_SEMAFOR);
	//}
	
	//if(xTaskCreate(vRestartTmrTask, "RestartTmr",  configMINIMAL_STACK_SIZE*3, NULL, PRIORITY_TASK_TMR3_TASK, (TaskHandle_t *) NULL)!=pdTRUE) 
	//{
	//		setStatusCalc(ER_CREATE_RST_TMR_TASK); 
	//}

	uint32_t speedUs = 1000000>>sel;  
	
	setAcc(1); 
	Info.Dcc = Info.Acc; 
	Info.VmaxUs = speedUs; 
	Info.VminUs = speedUs; 
	Info.currentVal.dir = SHD_DIR_FWD; 
	Info.currentVal.speed = Info.VminUs; // 1_000_000 us
	Info.nextVal.dir = SHD_DIR_FWD; 
	Info.nextVal.speed = Info.VminUs; 
	Info.flag.runStop = 0; 
	Info.status.state = SHD_MOVE_STOP; 
}

SHD_Info* GetShdInfo(void)
{
		return &Info; 
}

void getPosition(uint8_t * data32) {
	
		data32[0] = (uint8_t)((Info.valueStep>>24)&0xFF);
	  data32[1] = (uint8_t)((Info.valueStep>>16)&0xFF);
	  data32[2] = (uint8_t)((Info.valueStep>>8)&0xFF);
		data32[3] = (uint8_t)(Info.valueStep&0xFF);
}

void setStatusCalc(uint8_t data)
{
		StatusCalc <<= 4; 
		StatusCalc |= data; 
}



void getStatusCalc(uint8_t * data32)
{
		data32[0] = (uint8_t)((StatusCalc>>24)&0xFF);
	  data32[1] = (uint8_t)((StatusCalc>>16)&0xFF);
	  data32[2] = (uint8_t)((StatusCalc>>8)&0xFF);
		data32[3] = (uint8_t)(StatusCalc&0xFF);
}


QueueHandle_t* getPtrTimeList()
{
		return &timeList; 
}


void SetSpeed(uint32_t speed)
{
		Info.currentVal.speed = speed;  
		Info.nextVal.speed = speed;  
}	

uint32_t GetSpeed(uint8_t sel)
{
		switch(sel)
		{
			case 0: { return Info.oldVal.speed;  }
			case 1: { return Info.currentVal.speed;  }
			case 2: { return Info.nextVal.speed;  }
			default: { return Info.currentVal.speed; }
		}
		
}

void StartMove( SHD_dir dir, int32_t position)
{
	if(Info.status.state == SHD_MOVE_STOP)
	{
		Info.currentVal.dir = dir; 
		Info.nextVal.dir = dir; 
		Info.n = 0; 
		
		Info.oldVal.speed = Info.C0; 
		Info.currentVal.speed = Info.C0; 
		Info.nextVal.speed = Info.C0; 
		
		Info.targetStep = (int32_t)position; 
		if(Info.targetStep!=Info.valueStep)
		{
			stepClock(Info.currentVal.dir); 
			startMoveTimer(); 
		}
	}
}


void StopMove(void)
{
		Info.flag.runStop = 1;  
}

void reCalc(void){
	//LedOn(LED1);
	Info.C0 = (uint32_t)((sqrt(2.0/Info.Acc))*1000000); // 1_000_0000 ctoby perevesty v mikrosekundy
	uint64_t Speed2 = ((uint64_t)Info.Vmax) * Info.Vmax; 
	Info.countStepToVmax = (uint32_t)(Speed2/(Info.Acc<<1)); 
	//LedOff(LED1); 
}


void setAcc(uint32_t data){	 
		Info.Acc = data; 
		reCalc(); 
}

uint32_t getAcc(void){
		return Info.Acc;
}

uint32_t getC0(void){
		return Info.C0; 
}

void setVmax(uint32_t speed){
	Info.Vmax = speed; 
	Info.VmaxUs = (uint32_t)((1.0/speed)*1000000);; 
	reCalc(); 
}	
void setVmin(uint32_t speed){
	Info.Vmin = speed; 
	Info.VminUs = (uint32_t)((1.0/speed)*1000000); 
	reCalc(); 
}

uint32_t getVmax(void){
		return Info.VmaxUs; 
}

uint32_t getVmin(void){
		return Info.VminUs; 
}
uint32_t getCountSteToVmax(void){
		return Info.countStepToVmax; 
}

void calcCn(void){
		// 
		//LedOn(LED1); 
		//Info.nextVal.speed = (uint32_t)(((double)Info.currentVal.speed)*(sqrt((double)Info.n+1.0) - sqrt((double)Info.n))); 
		Info.nextVal.speed =  (Info.currentVal.speed);  
		//LedOff(LED1); 
}



//*********timer 
void Timer3_IRQHandler(void)
{				
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		if(TIMER_GetITStatus(L6470_TIMER, TIMER_STATUS_CNT_ZERO) == SET)
		{
			TIMER_ClearITPendingBit(L6470_TIMER, TIMER_STATUS_CNT_ZERO);
				 
				if(restartTimerMoreFFFF())
				{
						LedToggle(LED1);
					//TIMER_Cmd(L6470_TIMER, DISABLE);
					//xSemaphoreGiveFromISR(xSemTimer, &xHigherPriorityTaskWoken);
						
									 
						Info.oldVal = Info.currentVal; 
							
						if(Info.currentVal.dir == SHD_DIR_FWD)
						{
								Info.valueStep++; 
						}
						else 
						{
								Info.valueStep--; 
						}	
						if((Info.flag.runStop==1)|(Info.targetStep==Info.valueStep))
						{
								Info.flag.runStop = 0; 
								TIMER_Cmd(L6470_TIMER, DISABLE);
								Info.status.state = SHD_MOVE_STOP; 
						}
						else 
						{
								TIMER_Cmd(L6470_TIMER, ENABLE);
						} 
					 	 
				}
				//LedOff(LED1); 

				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);	 
				
		}
}


























