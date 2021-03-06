/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>
#include "MDR32Fx.h"


extern uint32_t SystemCoreClock;

//#define cdblog
#ifdef cdblog
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configCPU_CLOCK_HZ                    (80000000)
#define configTICK_RATE_HZ                    ((TickType_t)1000)
#define configTOTAL_HEAP_SIZE                 ((size_t)(10 * 1024))
#define configMINIMAL_STACK_SIZE              ((unsigned short)130)
#define configCHECK_FOR_STACK_OVERFLOW        0
#define configMAX_PRIORITIES                  (5)
#define configUSE_PREEMPTION                  1
#define configIDLE_SHOULD_YIELD               1
#define configMAX_TASK_NAME_LEN               (10)

#define configUSE_TIMERS                      0
#define configTIMER_TASK_PRIORITY             (2)
#define configTIMER_QUEUE_LENGTH              5
#define configTIMER_TASK_STACK_DEPTH          (configMINIMAL_STACK_SIZE * 2)

#define configUSE_MUTEXES                     1
#define configUSE_RECURSIVE_MUTEXES           1
#define configUSE_COUNTING_SEMAPHORES         1
#define configUSE_QUEUE_SETS                  1
#define configUSE_IDLE_HOOK                   0
#define configUSE_TICK_HOOK                   0
#define configUSE_MALLOC_FAILED_HOOK          0
#define configUSE_16_BIT_TICKS                0

#define INCLUDE_vTaskPrioritySet              1
#define INCLUDE_uxTaskPriorityGet             1
#define INCLUDE_vTaskDelete                   1
#define INCLUDE_vTaskSuspend                  1
#define INCLUDE_vTaskDelayUntil               1
#define INCLUDE_vTaskDelay                    1
#define INCLUDE_eTaskGetState                 1

#ifdef __NVIC_PRIO_BITS
  #define configPRIO_BITS                     __NVIC_PRIO_BITS
#else
  #define configPRIO_BITS                     4
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY   5
#define configKERNEL_INTERRUPT_PRIORITY   ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY   (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); } 

#define xPortPendSVHandler                    PendSV_Handler
#define vPortSVCHandler                       SVC_Handler
#define xPortSysTickHandler                   SysTick_Handler

#else

#define DEF_ON    1
#define DEF_OFF   0
// uart 
#define configDEBUG_SEND_ECHO        DEF_OFF    // on/off mode ECHO or obmen po 6 bait
#define configMODE_ALLIGN_6_BAIT		 DEF_ON		// allign po 6 bait v odnoi posylke, on/off clear data ne kratn 6 
#define congigCOUNT_BYTE_MESSAGE     6

#define TRACE_ON DEF_OFF // traceanalyzer 



#define configUSE_PREEMPTION		1 // 1 - 
#define configUSE_IDLE_HOOK			0 ///!!!!// zadacha bezdeistviya  1 - ONN, 0 - OFF
#define configUSE_TICK_HOOK			0 			// void vApplicationTickHook( void ); 1 - on, 0 - off. use functions only FromISR
#define configCPU_CLOCK_HZ			( ( unsigned long ) 80000000)//SystemCoreClock )	
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 5 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 130 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 17 * 1024 ) )
#define configMAX_TASK_NAME_LEN		( 16 )
//#define configUSE_TRACE_FACILITY	0    //**************************************TRACE**********************
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

#define configUSE_MUTEXES                     1
#define configUSE_RECURSIVE_MUTEXES           1
#define configUSE_COUNTING_SEMAPHORES         1


// timer 
#define configUSE_TIMERS                      1    // 1-on, 0 -off
#define configTIMER_TASK_PRIORITY             (1)   
#define configTIMER_QUEUE_LENGTH              10
#define configTIMER_TASK_STACK_DEPTH          (configMINIMAL_STACK_SIZE * 2)

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_eTaskGetState			1
 

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 		255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15


//#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); } 

//// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
// neobxodimo dobavit
#define xPortPendSVHandler                    PendSV_Handler
#define vPortSVCHandler                       SVC_Handler
#define xPortSysTickHandler                   SysTick_Handler

//
#if ( TRACE_ON == DEF_ON )
#define configUSE_TRACE_FACILITY	1
#include "trcRecorder.h"
#define INCLUDE_xTaskGetCurrentTaskHandle     1   
#else 
#define configUSE_TRACE_FACILITY	0    //**************************************TRACE**********************
#endif



// MY PRIORITY TASK
#define PRIORITY_TASK_OBMEN_SERIAL   2
#define PRIORITY_TASK_SEND_SERIAL    1
#define PRIORITY_TASK_SEND_SPI       5
#define PRIORITY_TASK_READ_SPI			 5
#define PRIORITY_TASK_CALK_TASK			 3
#define PRIORITY_TASK_TMR3_TASK			 4





#endif

//include "freertos_evr.h"

#endif /* FREERTOS_CONFIG_H */

