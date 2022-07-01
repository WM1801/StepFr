#include "CalcDrive2.h"

//C0 = sqrt(2/acc)
//Cn = C0 *(sqrt(n+1)-sqrt(n))
//Cn = Cn-1 - ((2*Cn-1)/(4n+1))
//if n = 1 -> Cn = Cn-1 - ((2*Cn-1)/(4n+1))*0.676

//step_to_Vmax = (speedMax*speedMax/(2*acc)) 

SHD_Info Info; 
speedRampData srd; 
uint32_t last_accel_delay; 
uint32_t step_count; 
uint32_t rest;
void speed_cntr_Move(int32_t step);


void calcC0(void){
		//LedOn(LED1);
	Info.C0 = (uint32_t)((sqrt(2.0/Info.Acc))*1000000); // 1_000_0000 ctoby perevesty v mikrosekundy
	Info.C1 = (uint32_t) ((double)(Info.C0-( ((Info.C0<<1)-1)/ 5))*0.676);  //((n<<2)+1) )); n=1 4n+1 == 5 
	rest = ((Info.C0<<1)/5);
	uint64_t Speed2 = ((uint64_t)Info.Vmax) * Info.Vmax; 
	Info.countStepToVmax = (uint32_t)(Speed2/(Info.Acc<<1)); 
	Info.countStepToV0 = (uint32_t)(((uint64_t)Info.Vmax)*Info.Dcc/(Info.Acc+Info.Dcc)); 
	//LedOff(LED1);  
}

// count - number step 
// if ACC - countStep>0
// if MOVE_RUN - count = const 
// if DCC - countStep<0
uint32_t calcCn(uint32_t oldCn, uint32_t countStep){
		uint32_t res =  (oldCn - ((oldCn<<1)+rest)/((countStep<<2)+1)); 
		rest = ((oldCn<<1)+rest)/((countStep<<2)+1);
	//new_step_delay = srd.step_delay - (((2 * (long)srd.step_delay) + rest)/(4 * srd.accel_count + 1));
  //rest = ((2 * (long)srd.step_delay)+rest)%(4 * srd.accel_count + 1);
		return res; 
}


void initCalcDrive(uint8_t stepMode){
	
	
	
		Info.currentVal.dir = SHD_DIR_FWD; 
		Info.currentVal.speed = 100000; 
	
		Info.nextVal.dir = SHD_DIR_FWD; 
		Info.nextVal.speed = 100000; 
		Info.status.state = SHD_MOVE_STOP;
	
		rest = 0; 
		setAcc(200); 
		setVmax(200<<stepMode); 
	
}	

SHD_Info* GetShdInfo(void){
	return &Info; 
}	

void StartMove(int32_t position){
	
		speed_cntr_Move(position);
}

void StopMove(void){
	Info.flag.runStop = 1;  
}

void SetSpeed(uint32_t speed){
		Info.currentVal.speed = speed;  
		Info.nextVal.speed = speed;  
}

uint32_t GetSpeed( uint8_t sel){
		return Info.currentVal.speed; 
}
//acc
void setAcc(uint32_t data){
	Info.Acc = data; 
	Info.Dcc = data; 
	calcC0(); 
}

uint32_t getAcc(void){
	return Info.Acc;
}

uint32_t getC0(uint8_t sel){
	switch(sel)
	{
		case 0: {return Info.C0; }
		default : {return Info.C1; }
	}
	
}

void setVmax(uint32_t speed){
	Info.Vmax = speed; 
	Info.VmaxUs = (uint32_t)((1.0/speed)*1000000);
	calcC0(); 
}

uint32_t getVmax(void){
	return Info.Vmax; 
}

//Vmin 
void setVmin(uint32_t speed){
	Info.Vmin = speed; 
	Info.VminUs = (uint32_t)((1.0/speed)*1000000); 
	calcC0(); 
}

uint32_t getVmin(void){
	return Info.VminUs; 
}	

uint32_t getPosition(uint8_t sel ){
		switch(sel)
		{
			case 0: { return Info.Position;  }
			case 1: { return Info.targetPosition;  }
			case 2: { return Info.nextTargetPosition;  }
			default: { return Info.Position;  }
		}
}	
// -aposition absolute value step
void speed_cntr_Move(int32_t aposition)   ///!!!!!!! time first step, start step potom preryvanie snovo step 
{
			if(Info.status.state == SHD_MOVE_STOP)
			{
					Info.targetPosition = aposition;
					if(aposition>Info.Position) /// move -->
					{
						if(Info.currentVal.dir!=SHD_DIR_FWD)						{
								Info.currentVal.dir = SHD_DIR_FWD; 
								Info.nextVal.dir = SHD_DIR_FWD;
						}
					}
					else // move <---
					{
						if(Info.currentVal.dir!=SHD_DIR_REV)						{
								Info.currentVal.dir = SHD_DIR_REV; 
								Info.nextVal.dir = SHD_DIR_REV;
						}
						
					}				
					Info.currentVal.speed = Info.C0; 
					Info.nextVal.speed =Info.C0; 
					stepClock(Info.currentVal.dir); 	
			}
			else
			{
						// wait stop for 
					 Info.nextTargetPosition = aposition; 
					 Info.flag.waitPosition = 1; 
			}
			
		//	if(Info.targetPosition!=Info.Position)
			{
					startMoveTimer(); 
			}
}

/*void speed_cntr_Move2(int32_t step)
{
		uint32_t max_s_lim; 
		uint32_t accel_lim; 
		
		if(step < 0){
				srd.dir = SRD_DIR_REV; 
				step = -step; 
		}
		else {
				srd.dir = SRD_DIR_FWD;  
		}
		
		
		if(step == 1){
				srd.accel_count = -1; 
			  srd.run_state = SRD_MOVE_DCC; 
				srd.step_delay = Info.C0; 
				SetSpeed(srd.step_delay); 
				Info.nextVal.dir = (uint32_t)srd.dir; 
				Info.currentVal.dir = (uint32_t)srd.dir; 
				startMoveTimer(); 
		}
		else
		{
				i
				
						//srd.min_delay = A_T_x100 / speed;
						srd.min_delay = Info.VmaxUs; 
					
						// Set accelration by calc the first (c0) step delay .
						// step_delay = 1/tt * sqrt(2*alpha/accel)
						// step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
						//srd.step_delay = (T1_FREQ_148 * sqrt(A_SQ / accel))/100;
						srd.step_delay = Info.C0; 
					
						// Find out after how many steps does the speed hit the max speed limit.
						// max_s_lim = speed^2 / (2*alpha*accel)
						// max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
						max_s_lim = Info.countStepToVmax; 
					
						if(max_s_lim == 0){
							max_s_lim = 1; 
						}
						
						
						// Find out after how many steps we must start deceleration.
						// n1 = (n1+n2)decel / (accel + decel)
						//accel_lim = ((long)step*decel) / (accel+decel);
						accel_lim = Info.countStepToV0; 
						
						if(accel_lim == 0){
								accel_lim = 1; 
						}
						
						
						if(accel_lim <= max_s_lim) {
								srd.decel_val = accel_lim - step; 
						}
						else{
								srd.decel_val = -(int32_t) (((uint64_t) max_s_lim)*Info.Acc)/Info.Dcc; 
						}
						// We must decelrate at least 1 step to stop.
						if(srd.decel_val == 0){
							srd.decel_val = -1;
						}
						// Find step to start decleration.
						srd.decel_start = step + srd.decel_val;
						
						 // If the maximum speed is so low that we dont need to go via accelration state.
						if(srd.step_delay <= srd.min_delay){
						srd.step_delay = srd.min_delay;
						srd.run_state = SRD_MOVE_RUN;
						}
						else{
						srd.run_state = SRD_MOVE_ACC;
						}
						
						// Reset counter.
						srd.accel_count = 0;
						SetSpeed(srd.step_delay); 
						Info.nextVal.dir = (uint32_t) srd.dir; 
						Info.currentVal.dir = (uint32_t) srd.dir; 
						startMoveTimer(); 
						
						

						
						
				}
			
		}
		
	
	
}*/

void reTakt(void)
{
		if(Info.currentVal.dir == SHD_DIR_FWD)
		{
				Info.Position++; 
		}
		else 
		{
				Info.Position--; 
		}
		if(Info.Position == Info.targetPosition)
		{
				stopTimerL6470(); 
				Info.status.state = SHD_MOVE_STOP; 
				rest = 0; 
		}
		
		if(Info.status.state == SHD_MOVE_STOP){
				if(Info.flag.waitPosition == 1)
				{
						//speed_cntr_Move(Info.nextTargetPosition); 
						Info.flag.waitPosition = 0; 
				}
		}
		
		if(Info.status.state == SHD_MOVE_DCC){
		
		}
		
		if(Info.status.state == SHD_MOVE_ACC){
		
		}
		
		if(Info.status.state == SHD_MOVE_RUN){
		
		}
		
}
/*void reTakt2(void)
{
		uint32_t new_step_delay; 

		if(srd.run_state==SRD_MOVE_STOP){
			step_count = 0; 
			rest =0; 
			StopMove(); 
		}
		
		if(srd.run_state==SRD_MOVE_ACC){
			stepClock(srd.dir); 
      step_count++;
      srd.accel_count++;
      new_step_delay = srd.step_delay - (((2 * (long)srd.step_delay) + rest)/(4 * srd.accel_count + 1));
      rest = ((2 * (long)srd.step_delay)+rest)%(4 * srd.accel_count + 1);
      // Chech if we should start decelration.
      if(step_count >= srd.decel_start) {
        srd.accel_count = srd.decel_val;
        srd.run_state = SRD_MOVE_DCC;
      }
      // Chech if we hitted max speed.
      else if(new_step_delay <= srd.min_delay) {
        last_accel_delay = new_step_delay;
        new_step_delay = srd.min_delay;
        rest = 0;
        srd.run_state = SRD_MOVE_RUN;
      }
			
		}
		
		if(srd.run_state==SRD_MOVE_RUN){
		stepClock(srd.dir); 
		 step_count++;
      new_step_delay = srd.min_delay;
      // Chech if we should start decelration.
      if(step_count >= srd.decel_start) {
        srd.accel_count = srd.decel_val;
        // Start decelration with same delay as accel ended with.
        new_step_delay = last_accel_delay;
        srd.run_state = SRD_MOVE_DCC;	
		}
	}
		if(srd.run_state==SRD_MOVE_DCC){
			stepClock(srd.dir);
      step_count++;
      srd.accel_count++;
      new_step_delay = srd.step_delay - (((2 * (long)srd.step_delay) + rest)/(4 * srd.accel_count + 1));
      rest = ((2 * (long)srd.step_delay)+rest)%(4 * srd.accel_count + 1);
      // Check if we at last step
      if(srd.accel_count >= 0){
        srd.run_state = SRD_MOVE_STOP;
      }
		}
		srd.step_delay = new_step_delay;
		Info.nextVal.speed = srd.step_delay; 
		Info.nextVal.dir = srd.dir; 
		
}*/

























