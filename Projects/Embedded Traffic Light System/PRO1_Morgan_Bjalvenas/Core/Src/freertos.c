/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/**
******************************************************************************
@brief Main program file for PRO1 project
@file freertos.c
@author Morgan Bjälvenäs
@version 1.0
@date 20-December-2023
@brief All tasks, functions, and data structures, constituting traffic system
******************************************************************************
*/

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "traffic_light_functions.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// defines all states for traffic light FSM
typedef enum
{NorthSouth,
	EastWest,
	Orange
}TLStates;

// defines all states for pedestrian light FSM
typedef enum
{North,
	West,
	Toggle
}PLStates;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
// all configurable timing conditions
uint16_t orangeDelay = 3000; // orangeDelay set to 3 seconds
uint16_t greenDelay = 10000; // greenDelay set to 10 seconds
uint16_t walkingDelay = 8000; // greenDelay set to 8 seconds
uint16_t toggleFreq = 400; // toggleFreq set to 400 ms
uint16_t pedestrianDelay = 6000; // pedestrianDelay set to 6 seconds
uint16_t redDelayMax = 15000; // redDelayMax set to 15 seconds

// initialization of LEDs for corresponding state
// at program start everything is false

// traffic light state initializations
bool northSouthInitialized = false;
bool eastWestInitialized = false;
bool orangeInitialized = false;

// pedestrian light state initializations
bool northInitialized = false;
bool westInitialized = false;
bool toggleInitialized = false;

// booleans representing active toggle of each blue pedestrian light
bool PL1Toggle = false;
bool PL2Toggle = false;

// state memory
TLStates currentState = NorthSouth; // current state of traffic light FSM
TLStates previousState; // previous state of traffic light FSM

PLStates PLState = West; // current state of pedestrian light FSM
PLStates PLPreviousState; // previous state of pedestrian light FSM

// array representing all traffic and pedestrian light LEDs on traffic light shield
uint8_t trafficLeds[] = {0b00000000, 0b00000000, 0b00000000};

// timers
uint16_t TLFSMTimer = 0; // timer for active time in one state for traffic light FSM
uint16_t PLTimer = 0; // timer for active time in one state for pedestrian light FSM
uint16_t toggle1Timer = 0; // elapsed toggle time for first blue pedestrian light
uint16_t toggle2Timer = 0; // elapsed toggle time for second blue pedestrian light

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TrafficLightFSM */
osThreadId_t TrafficLightFSMHandle;
const osThreadAttr_t TrafficLightFSM_attributes = {
  .name = "TrafficLightFSM",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PedestrianLight */
osThreadId_t PedestrianLightHandle;
const osThreadAttr_t PedestrianLight_attributes = {
  .name = "PedestrianLight",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for PL1Toggle */
osThreadId_t PL1ToggleHandle;
const osThreadAttr_t PL1Toggle_attributes = {
  .name = "PL1Toggle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for PL2Toggle */
osThreadId_t PL2ToggleHandle;
const osThreadAttr_t PL2Toggle_attributes = {
  .name = "PL2Toggle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BinarySemaphoreLEDs */
// Semaphore for LEDs on Traffic Light Shield
osSemaphoreId_t BinarySemaphoreLEDsHandle;
const osSemaphoreAttr_t BinarySemaphoreLEDs_attributes = {
  .name = "BinarySemaphoreLEDs"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void traffic_light_FSM(void *argument);
void pedestrian_light1(void *argument);
void PL1_toggle(void *argument);
void PL2_toggle(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BinarySemaphoreLEDs */
  BinarySemaphoreLEDsHandle = osSemaphoreNew(1, 1, &BinarySemaphoreLEDs_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TrafficLightFSM */
  TrafficLightFSMHandle = osThreadNew(traffic_light_FSM, NULL, &TrafficLightFSM_attributes);

  /* creation of PedestrianLight */
  PedestrianLightHandle = osThreadNew(pedestrian_light1, NULL, &PedestrianLight_attributes);

  /* creation of PL1Toggle */
  PL1ToggleHandle = osThreadNew(PL1_toggle, NULL, &PL1Toggle_attributes);

  /* creation of PL2Toggle */
  PL2ToggleHandle = osThreadNew(PL2_toggle, NULL, &PL2Toggle_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1); // does nothing
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_traffic_light_FSM */
/**
* @brief Function implementing the TrafficLightFSM thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_traffic_light_FSM */
void traffic_light_FSM(void *argument)
{
  /* USER CODE BEGIN traffic_light_FSM */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100) ; // 100 ms task period
	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
	  switch(currentState){
	  case NorthSouth : // if current state is NorthSouth

		  if(BinarySemaphoreLEDsHandle != NULL){
			  // if semaphore in available, and northSouth LEDs have not been initialized
			  if (northSouthInitialized == false && osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){

				  // Access shared LEDs
				  //initialize LEDs
				  reset_traffic_leds(trafficLeds);
				  set_red_traffic_led(trafficLeds, true, 1);
				  set_red_traffic_led(trafficLeds, true, 3);
				  set_green_traffic_led(trafficLeds, true, 2);
			  	  set_green_traffic_led(trafficLeds, true, 4);

			  	  //set north and south lane LEDs as initialized
			  	  northSouthInitialized = true;
			  	  eastWestInitialized = false;
			  	  orangeInitialized = false;

			  	  // access over, release semaphore for LEDs
			  	  osSemaphoreRelease(BinarySemaphoreLEDsHandle);
			  }
		  }

		  // if PL2Toggle timer equals (pedestrianDelay - orangeDelay) and walking delay has passed
		  if (PL2Toggle == true && toggle2Timer >= (pedestrianDelay - orangeDelay) && TLFSMTimer >= walkingDelay){
			  // switch to Orange state
			  currentState = Orange;
			  previousState = NorthSouth;

			  // reset timer
			  TLFSMTimer = 0;
		  }

		  // if no cars active, and timer has surpassed walkingDelay and greenDelay
		  if((TL1_car_active() == false && TL2_car_active() == false && TL3_car_active() == false && TL4_car_active() == false)
				  && (PLTimer >= walkingDelay && PLState == West)){

			  if(TLFSMTimer >= greenDelay){
				  // switch to Orange state
				  currentState = Orange;
				  previousState = NorthSouth;

				  // reset timer
				  TLFSMTimer = 0;
			  }
		  }

		  // if car 1 or car 3 is active and car 2 or 4 is active then wait (redDelayMax - orangeDelay) before switching to orange
		  if ((TL2_car_active() == true || TL4_car_active() == true) && (TL1_car_active() == true || TL3_car_active() == true)){
			  if (TLFSMTimer >= (redDelayMax - orangeDelay) && TLFSMTimer >= walkingDelay){
				  currentState = Orange;
				  previousState = NorthSouth;

				  // reset timer
				  TLFSMTimer = 0;
			  }
		  }

		  // if car 1 or 3 is active and car 2 and 4 is inactive and walkingDelay has passed then change directly to orange state
		  if (((TL2_car_active() == false && TL4_car_active() == false) && (TL1_car_active() == true || TL3_car_active() == true))
				  && TLFSMTimer >= walkingDelay){
			  currentState = Orange;
			  previousState = NorthSouth;

			  //reset timer
			  TLFSMTimer = 0;
		  }


	   break;
	   case EastWest : // if current state is EastWest

		   // if semaphore in available, and EastWest LEDs have not been initialized
		   if(BinarySemaphoreLEDsHandle != NULL){

			   if(eastWestInitialized == false && osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){
				   // Access shared LEDs
				   // initialize LEDs
				   reset_traffic_leds(trafficLeds);
				   set_red_traffic_led(trafficLeds, true, 2);
				   set_red_traffic_led(trafficLeds, true, 4);
				   set_green_traffic_led(trafficLeds, true, 1);
				   set_green_traffic_led(trafficLeds, true, 3);

				   //set east and west lane LEDs as initialized
				   northSouthInitialized = false;
				   eastWestInitialized = true;
				   orangeInitialized = false;

				   // access over, release semaphore for LEDs
				   osSemaphoreRelease(BinarySemaphoreLEDsHandle);
			   }
		   }

		   // if PL2Toggle timer equals (pedestrianDelay - orangeDelay) and walking delay has passed
		   if (PL1Toggle == true && toggle1Timer >= (pedestrianDelay - orangeDelay) && TLFSMTimer >= walkingDelay){
			   // switch to Orange state
			   currentState = Orange;
			   previousState = EastWest;

			   //reset timer
			   TLFSMTimer = 0;
		   }

		   // if no active cars, and timer has surpassed walkingDelay and greenDelay
		   if((TL1_car_active() == false && TL2_car_active() == false && TL3_car_active() == false && TL4_car_active() == false)
				   && (PLTimer >= walkingDelay && PLState == North)){
			   if(TLFSMTimer >= greenDelay){
				   // switch to Orange state
				   currentState = Orange;
				   previousState = EastWest;

				   //reset timer
				   TLFSMTimer = 0;
			   }
		   }

		   // if car 1 or car 3 is active and car 2 or 4 is active then wait (redDelayMax - orangeDelay) and walkingDelay before switching to orange
		   if ((TL2_car_active() == true || TL4_car_active() == true) && (TL1_car_active() == true || TL3_car_active() == true)){
			   if (TLFSMTimer >= (redDelayMax - orangeDelay) && TLFSMTimer >= walkingDelay){
				   currentState = Orange;
				   previousState = EastWest;

				   // reset timer
				   TLFSMTimer = 0;
			   }
		   }

		   // if car 2 or 4 is active and car 1 and 3 is inactive and walkingDelay has passed then change directly to orange state
		   if (((TL2_car_active() == true || TL4_car_active() == true) && (TL1_car_active() == false && TL3_car_active() == false))
				   && TLFSMTimer >= walkingDelay){
			   currentState = Orange;
			   previousState = EastWest;

			   //reset timer
			   TLFSMTimer = 0;
		   }


	   break;
	   case Orange :
		   if(BinarySemaphoreLEDsHandle != NULL){
			   // if semaphore in available, and northSouth LEDs have not been initialized
			   if(orangeInitialized == false && osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){
				   // Access shared LEDs
				   // initialize LEDs
				   reset_traffic_leds(trafficLeds);
				   set_yellow_traffic_led(trafficLeds, true, 1);
				   set_yellow_traffic_led(trafficLeds, true, 2);
				   set_yellow_traffic_led(trafficLeds, true, 3);
				   set_yellow_traffic_led(trafficLeds, true, 4);

				   reset_pedestrian_leds(trafficLeds);
				   set_red_PL_led(trafficLeds, true, 1);
				   set_red_PL_led(trafficLeds, true, 2);


				   //set orange LEDs as initialized
				   northSouthInitialized = false;
				   eastWestInitialized = false;
				   orangeInitialized = true;

				   // access over, release semaphore for LEDs
				   osSemaphoreRelease(BinarySemaphoreLEDsHandle);
			   }
		   }


		   // if time in Orange state has surpassed orangeDelay
		   if(TLFSMTimer >= orangeDelay){
			   if(previousState == NorthSouth){ // if the previous state was NorthSouth
				   // switch to EastWest
				   currentState = EastWest;
			   } else{ // if the previous state was EastWest
				   // switch to NorthSouth
				   currentState = NorthSouth;
			   }

			   //reset timer
			   TLFSMTimer = 0;
		   }

		break;
		default:
			// do nothing (will never reach)
		}

	  // add 100 ms to the active time in state
	  TLFSMTimer += 100;

	  // delay until next period
	  vTaskDelayUntil( &xLastWakeTime, xPeriod );

  }
  /* USER CODE END traffic_light_FSM */
}

/* USER CODE BEGIN Header_pedestrian_light1 */
/**
* @brief Function implementing the PedestrianLight thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pedestrian_light1 */
void pedestrian_light1(void *argument)
{
  /* USER CODE BEGIN pedestrian_light1 */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100) ; // 100 ms task period
	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
	switch(PLState){
    case North : // if current state is North

    	// if semaphore is available and North state LEDs have not been initialized
    	if(northInitialized == false && osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){
    		// access shared LEDs
    		// initialize LEDs
    		reset_pedestrian_leds(trafficLeds);
    		set_red_PL_led(trafficLeds, true, 1);
    		set_green_PL_led(trafficLeds, true, 2);

    		// set red pedestrian LEDs as initialized
    		northInitialized = true;
    		westInitialized = false;
    		toggleInitialized = false;

    		// access over, release semaphore for LEDs
    		osSemaphoreRelease(BinarySemaphoreLEDsHandle);
    	}

    	// if first pedestrian button is hit
    	if (PL1_switch_hit() == true){
    		// switch to Toggle state
    		PLState = Toggle;
    		PLPreviousState = North;

    		// reset timer
    		PLTimer = 0;
    	}

    	// if current state of traffic light FSM is NorthSouth
    	if (currentState == NorthSouth){
    		// switch to West state
    		PLState = West;

    		// reset timer
    		PLTimer = 0;
    	}


    break;
    case West : // if current state is West

    	// if semaphore is available and West LEDs have not been initialized
    	if(westInitialized == false && osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){
    		// access shared LEDs
    		// initialize LEDs
    		reset_pedestrian_leds(trafficLeds);
    		set_green_PL_led(trafficLeds, true, 1);
    		set_red_PL_led(trafficLeds, true, 2);

    		// set green pedestrian LEDs as initialized
    		northInitialized = false;
    		westInitialized = true;
    		toggleInitialized = false;

    		// access over, release semaphore for LEDs
    		osSemaphoreRelease(BinarySemaphoreLEDsHandle);

    	}

    	// if second pedestrian button is hit
    	if (PL2_switch_hit() == true){
    		// switch to Toggle state
    		PLState = Toggle;
    		PLPreviousState = West;

    		// reset timer
    		PLTimer = 0;
    	}

    	// if current state of traffic light FSM is EastWest
    	if (currentState == EastWest){
    		// switch to North state
    		PLState = North;

    		// reset timer
    		PLTimer = 0;
    	}

    break;
    case Toggle : // if current state is Toggle

    	if(toggleInitialized == false && osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){

    		// set toggle state as initialized
    		northInitialized = false;
    		westInitialized = false;
    		toggleInitialized = true;

    		// enable toggling
    		if(PLPreviousState == North){ // if previous state was North
    			PL1Toggle = true; // task "PL1_toggle" goes from blocked to active state
    		} else if (PLPreviousState == West){ // if previous state was West
    			PL2Toggle = true; // task "PL2_toggle" goes from blocked to active state
    		}


    		// access over, release semaphore for LEDs
    		osSemaphoreRelease(BinarySemaphoreLEDsHandle);

    	}

    	// when toggle is over change state
    	if(PL1Toggle == false && PL2Toggle == false){ // if pedestrian toggle 1 or 2 is false
    		if (PLPreviousState == North){ // previous state is North
    			PLState = West; // switch to West state
    		} else if (PLPreviousState == West){ // previous state is West
    			PLState = North; // switch to North state
    		}
    		// reset timer
    		PLTimer = 0;
    	}

    break;
    default :
    	// do nothing, will never reach
    }

    // add 100 ms to timer
    PLTimer += 100;

    // delay until next period
    vTaskDelayUntil( &xLastWakeTime, xPeriod );

  }
  /* USER CODE END pedestrian_light1 */
}

/* USER CODE BEGIN Header_PL1_toggle */
/**
* @brief Function implementing the PL1Toggle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PL1_toggle */
void PL1_toggle(void *argument)
{
  /* USER CODE BEGIN PL1_toggle */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(toggleFreq) ; // toggleFreq ms task period
	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
	  if (PL1Toggle == true){ // check if PL FSM is in toggle state
		  // task has gone from blocked to active state

		  uint8_t modifiedBitSequence; // 8 bit sequence, representing modified bit sequence

		  // check if traffic light FSM current state is NorthSouth
		  if(currentState == NorthSouth){
			  // leave toggle state
			  // turn task to blocked state
			  PL1Toggle = false;
			  // reset timer
			  toggle1Timer = 0;
			  continue;
		  }

		  // check if semaphore is available, acquire it
		  if (osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){
			  // shared LEDs accessed

			  // turn blue PL LED on
			  modifiedBitSequence = modify_bit(trafficLeds[0], 2, 1);
			  trafficLeds[0] = modifiedBitSequence;
			  set_traffic_leds(trafficLeds);

			  // release semaphore
		      osSemaphoreRelease(BinarySemaphoreLEDsHandle);
		  }

	  	  // wait half of toggleFreq
	  	  osDelay(toggleFreq/2);

	  	  // check if semaphore is available, acquire it
	  	  if (osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){
	  		  // shared LEDs accessed

			  // turn blue PL LED off
			  modifiedBitSequence = modify_bit(trafficLeds[0], 2, 0);
			  trafficLeds[0] = modifiedBitSequence;
			  set_traffic_leds(trafficLeds);

			  // release semaphore
		      osSemaphoreRelease(BinarySemaphoreLEDsHandle);
	  	  }



	  	  // update timer, appending toggleFreq
	  	  toggle1Timer += toggleFreq;

		  if(toggle1Timer >= pedestrianDelay && TLFSMTimer >= (walkingDelay + orangeDelay)){ // check if timer has reached pedestrianDelay and walkingDelay has passed
			  // leave toggle state
			  // turn task to blocked state
			  PL1Toggle = false;

			  // reset timer
			  toggle1Timer = 0;
		  }

      }

	  // delay until next period
      vTaskDelayUntil( &xLastWakeTime, xPeriod );
  }
  /* USER CODE END PL1_toggle */
}

/* USER CODE BEGIN Header_PL2_toggle */
/**
* @brief Function implementing the PL2Toggle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PL2_toggle */
void PL2_toggle(void *argument)
{
  /* USER CODE BEGIN PL2_toggle */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(toggleFreq) ; // toggleFreq ms task period
	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
  for(;;)
  {
    if(PL2Toggle == true){ // check if PL FSM is in toggle state
    	// task has entered active state from blocked state

    	uint8_t modifiedBitSequence; // 8 bit sequence, representing modified bit sequence

    	// check if traffic light FSM current state is NorthSouth
		if(currentState == EastWest){
			// leave toggle state
			// turn task to blocked state
			PL2Toggle = false;
			// reset timer
			toggle2Timer = 0;
			continue;
		}

		// check if semaphore is available, acquire it
		if (osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){

			// turn blue PL LED on
			modifiedBitSequence = modify_bit(trafficLeds[1], 2, 1);
			trafficLeds[1] = modifiedBitSequence;
			set_traffic_leds(trafficLeds);

			// release semaphore
			osSemaphoreRelease(BinarySemaphoreLEDsHandle);
		}

		// wait half of toggleFreq
		osDelay(toggleFreq/2);

		// check if semaphore is available, acquire it
		if (osSemaphoreAcquire(BinarySemaphoreLEDsHandle, osWaitForever) == osOK){
			// turn blue PL LED off
			modifiedBitSequence = modify_bit(trafficLeds[1], 2, 0);
			trafficLeds[1] = modifiedBitSequence;
			set_traffic_leds(trafficLeds);

			// release semaphore
			osSemaphoreRelease(BinarySemaphoreLEDsHandle);
		  }

		// update timer, appending toggleFreq
		toggle2Timer += toggleFreq;


		if(toggle2Timer >= pedestrianDelay && TLFSMTimer >= (walkingDelay + orangeDelay)){ // check if timer has reached pedestrianDelay and walkingDelay has passed
			// leave toggle state
			// turn task to blocked state
			PL2Toggle = false;

			// reset timer
			toggle2Timer = 0;
		}

    }

    // delay until next period
    vTaskDelayUntil( &xLastWakeTime, xPeriod );

  }
  /* USER CODE END PL2_toggle */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

