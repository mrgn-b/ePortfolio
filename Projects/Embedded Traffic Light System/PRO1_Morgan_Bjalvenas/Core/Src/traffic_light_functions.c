/**
******************************************************************************
@brief Traffic light functions, functions for traffic light system software
@file traffic_light_functions.c
@author Morgan Bjälvenäs
@version 1.0
@date 20-December-2023
@brief Functions and structures for PRO1 project software
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "spi.h"
#include "traffic_light_functions.h"
#include "stdbool.h"

/**
@brief set_traffic_leds, sends SPI data to shift registers controlling traffic light LEDs
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@return void, no return value
*/
void set_traffic_leds(uint8_t data[]){
	//Reset the buffer
	HAL_GPIO_WritePin(Reset_GPIO_Port, Reset_Pin, GPIO_PIN_RESET );

	//Disable output
	HAL_GPIO_WritePin(Enable_GPIO_Port, Enable_Pin, GPIO_PIN_SET );

	//Open buffer for transmission
	HAL_GPIO_WritePin(Reset_GPIO_Port, Reset_Pin, GPIO_PIN_SET );

	//Transmit data, 8 bits 3 times in succession resulting in 24 bits
	HAL_SPI_Transmit(&hspi3, &data[2], 1, 10);
	HAL_SPI_Transmit(&hspi3, &data[1], 1, 10);
	HAL_SPI_Transmit(&hspi3, &data[0], 1, 10);

	// Set LEDs
	HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_SET ); // set STCP high
	HAL_Delay(10); // wait 10 ms
	HAL_GPIO_WritePin(STCP_GPIO_Port, STCP_Pin, GPIO_PIN_RESET ); // set STCP low


	// Enable output
	HAL_GPIO_WritePin(Enable_GPIO_Port, Enable_Pin, GPIO_PIN_RESET );

} // end of function set_traffic_leds

/**
@brief reset_traffic_leds, set all traffic LEDs to low
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@return void, no return value
*/
void reset_traffic_leds(uint8_t data[]){

	// manipulate byte sequence in first shift register
	uint8_t modifiedSR1 = modify_bit(data[0], 7, 0); // set red traffic light LED low
	modifiedSR1 = modify_bit(modifiedSR1, 6, 0); // set yellow traffic light LED low
	modifiedSR1 = modify_bit(modifiedSR1, 5, 0); // set green traffic light LED low

	// manipulate byte sequence in second shift register
	uint8_t modifiedSR2 = modify_bit(data[1], 7, 0); // set red traffic light LED low
	modifiedSR2 = modify_bit(modifiedSR2, 6, 0); // set yellow traffic light LED low
	modifiedSR2 = modify_bit(modifiedSR2, 5, 0); // set green traffic light LED low

	data[0] = modifiedSR1; // set first shift register manipulated byte sequence in data array
	data[1] = modifiedSR2; // set second shift register manipulated byte sequence in data array
	data[2] = 0; // set third shift register zero

	set_traffic_leds(data); // transmit SPI data

} // end of function reset_traffic_leds

/**
@brief reset_pedestrian_leds, set all pedestrian LEDs to low
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@return void, no return value
*/
void reset_pedestrian_leds(uint8_t data[]){

	// manipulate byte sequence in first shift register
	uint8_t modifiedSR1 = modify_bit(data[0], 4, 0); // set red pedestrian light LED low
	modifiedSR1 = modify_bit(modifiedSR1, 3, 0); // set green pedestrian light LED low
	modifiedSR1 = modify_bit(modifiedSR1, 2, 0); // set blue pedestrian light LED low

	// manipulate byte sequence in second shift register
	uint8_t modifiedSR2 = modify_bit(data[1], 4, 0); // set red pedestrian light LED low
	modifiedSR2 = modify_bit(modifiedSR2, 3, 0); // set green pedestrian light LED low
	modifiedSR2 = modify_bit(modifiedSR2, 2, 0); // set blue pedestrian light LED low

	data[0] = modifiedSR1; // set first shift register manipulated byte sequence in data array
	data[1] = modifiedSR2; // set second shift register manipulated byte sequence in data array

	set_traffic_leds(data); // transmit SPI data
} // end of function reset_pedestrian_leds

/**
@brief TL1_car_active, returns true when car by first traffic light is active
@param1 void, no parameter value
@return bool, returns true or false
*/
bool TL1_car_active(void){

	  if(HAL_GPIO_ReadPin(TL1_Car_GPIO_Port, TL1_Car_Pin) == 1){ // check if car is active
		  return true;
	  }else{ // if car not active
		  return false;
	  }

} // end of function TL1_car_active

/**
@brief TL2_car_active, returns true when car by second traffic light is active
@param1 void, no parameter value
@return bool, returns true or false
*/
bool TL2_car_active(void){

	  if(HAL_GPIO_ReadPin(TL2_Car_GPIO_Port, TL2_Car_Pin) == 1){ // check if car is active
		  return true;
	  }else{ // if car not active
		  return false;
	  }

} // end of function TL2_car_active

/**
@brief TL3_car_active, returns true when car by third traffic light is active
@param1 void, no parameter value
@return bool, returns true or false
*/
bool TL3_car_active(void){

	  if(HAL_GPIO_ReadPin(TL3_Car_GPIO_Port, TL3_Car_Pin) == 1){ // check if car is active
		  return true;
	  }else{ // if car is not active
		  return false;
	  }

} // end of function TL3_car_active

/**
@brief TL4_car_active, returns true when car by fourth traffic light is active
@param1 void, no parameter value
@return bool, returns true or false
*/
bool TL4_car_active(void){

	  if(HAL_GPIO_ReadPin(TL4_Car_GPIO_Port, TL4_Car_Pin) == 1){ // check if car is active
		  return true;
	  }else{ // if car is not active
		  return false;
	  }

} // end of function TL4_car_active

/**
@brief PL1_switch_hit, returns true when first pedestrian button is hit
@param1 void, no parameter value
@return bool, returns true or false
*/
bool PL1_switch_hit(void){

	if (HAL_GPIO_ReadPin(PL1_Switch_GPIO_Port, PL1_Switch_Pin) == 0){ // check if button is hit
		return true;
	}else{ // if button is not hit
		return false;
	}

} // end of function PL1_switch_hit

/**
@brief PL2_switch_hit, returns true when second pedestrian button is hit
@param1 void, no parameter value
@return bool, returns true or false
*/
bool PL2_switch_hit(void){

	if (HAL_GPIO_ReadPin(PL2_Switch_GPIO_Port, PL2_Switch_Pin) == 0){ // check if button is hit
		return true;
	}else{ // if button is not hit
		return false;
	}

} // end of function PL1_switch_hit

/**
@brief set_red_traffic_led, sets one of the red traffic lights high or low
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@param2 bool power, sets LED high (true) or low (false)
@param3 uint8_t ledNr, LED number
@return void, no return value
*/
void set_red_traffic_led (uint8_t data[], bool power, uint8_t ledNr){

	uint8_t modifiedBitSequence; // 8 bit sequence, representing modified bit sequence

	switch (ledNr){
	case 1: // if LED number is 1

		if (power == true){ // check if LED should be set to high

			// set first red traffic light LED to high
			modifiedBitSequence = modify_bit(data[0], 7, 1); // set red LED to high
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set first red traffic light LED to low
			modifiedBitSequence = modify_bit(data[0], 7, 0); // set red LED to low
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 2: // if LED number is 2

		if (power == true){// check if LED should be set to high

			// set second red traffic light LED to high
			modifiedBitSequence = modify_bit(data[1], 7, 1); // set red LED to high
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set second red traffic light LED to low
			modifiedBitSequence = modify_bit(data[1], 7, 0); // set red LED to low
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 3: // if LED number is 3

		if (power == true){ // check if LED should be set to high

			// set third red traffic light LED to high
			modifiedBitSequence = modify_bit(data[2], 7, 1); // set red LED to high
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set third red traffic light LED to low
			modifiedBitSequence = modify_bit(data[2], 7, 0); // set red LED to low
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 4: // if LED number is 4

		if (power == true){ // check if LED should be set to high

			// set fourth red traffic light LED to high
			modifiedBitSequence = modify_bit(data[2], 4, 1); // set red LED to high
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set fourth red traffic light LED to low
			modifiedBitSequence = modify_bit(data[2], 4, 0); // set red LED to low
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	default:
		// do nothing
	}

} // end of function set_red_traffic_led

/**
@brief set_yellow_traffic_led, sets one of the yellow traffic lights high or low
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@param2 bool power, sets LED high (true) or low (false)
@param3 uint8_t ledNr, LED number
@return void, no return value
*/
void set_yellow_traffic_led (uint8_t data[], bool power, uint8_t ledNr){

	uint8_t modifiedBitSequence; // 8 bit sequence, representing modified bit sequence

	switch (ledNr){
	case 1: // if LED number is 1

		if (power == true){ // check if LED should be set to high

			// set first yellow traffic light LED to high
			modifiedBitSequence = modify_bit(data[0], 6, 1); // set yellow LED to high
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set first yellow traffic light LED to low
			modifiedBitSequence = modify_bit(data[0], 6, 0); // set yellow LED to low
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 2: // if LED number is 2

		if (power == true){// check if LED should be set to high

			// set second yellow traffic light LED to high
			modifiedBitSequence = modify_bit(data[1], 6, 1); // set yellow LED to high
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set second yellow traffic light LED to low
			modifiedBitSequence = modify_bit(data[1], 6, 0); // set yellow LED to low
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 3: // if LED number is 3

		if (power == true){ // check if LED should be set to high

			// set third yellow traffic light LED to high
			modifiedBitSequence = modify_bit(data[2], 6, 1); // set yellow LED to high
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set third yellow traffic light to low
			modifiedBitSequence = modify_bit(data[2], 6, 0); // set yellow LED to low
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data
		}

	break;
	case 4: // if LED number is 4

		if (power == true){ // check if LED should be set to high

			// set fourth yellow traffic light LED to high
			modifiedBitSequence = modify_bit(data[2], 3, 1); // set yellow LED to high
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set fourth yellow traffic light LED to low
			modifiedBitSequence = modify_bit(data[2], 3, 0); // set yellow LED to low
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	default:
		// do nothing
	}

} // end of function set_yellow_traffic_led

/**
@brief set_green_traffic_led, sets one of the green traffic lights high or low
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@param2 bool power, sets LED high (true) or low (false)
@param3 uint8_t ledNr, LED number
@return void, no return value
*/
void set_green_traffic_led (uint8_t data[], bool power, uint8_t ledNr){

	uint8_t modifiedBitSequence; // 8 bit sequence, representing modified bit sequence

	switch (ledNr){
	case 1: // if LED number is 1

		if (power == true){ // check if LED should be set to high

			// set first green traffic light LED should be set to high
			modifiedBitSequence = modify_bit(data[0], 5, 1); // set green LED to high
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set first green traffic light LED to low
			modifiedBitSequence = modify_bit(data[0], 5, 0); // set green LED to low
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 2: // if LED number is 2

		if (power == true){ // check if traffic light LED should be set to high

			// set second green traffic light LED to high
			modifiedBitSequence = modify_bit(data[1], 5, 1); // set green LED to high
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set second green traffic light LED to low
			modifiedBitSequence = modify_bit(data[1], 5, 0); // set green LED to low
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 3: // if LED number is 3

		if (power == true){ // check if traffic light LED should be set to high

			// set third green traffic light LED to high
			modifiedBitSequence = modify_bit(data[2], 5, 1); // set green LED to high
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set third green traffic light LED to low
			modifiedBitSequence = modify_bit(data[2], 5, 0); // set green LED to low
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 4:

		if (power == true){ // check if traffic light LED should be set to high

			// set fourth green traffic light LED to high
			modifiedBitSequence = modify_bit(data[2], 2, 1); // set green LED to high
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set fourth green traffic light LED to low
			modifiedBitSequence = modify_bit(data[2], 2, 0); // set green LED to low
			data[2] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data);// transmit SPI data

		}

	break;
	default:
		// do nothing
	}

} // end of function set_green_traffic_led

/**
@brief toggle_PL_led, toggles one of the blue pedestrian LEDs for certain amount of time in a specific frequency
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@param2 uint8_t ledNr, LED number
@param3 uint32_t freq, toggle frequency in ms
@param4 uint32_t time, toggle time in ms
@return void, no return value
*/
void toggle_PL_led(uint8_t data[], uint8_t ledNr, uint32_t freq, int32_t time){

	uint8_t modifiedBitSequence; // 8 bit sequence, representing modified bit sequence

	switch(ledNr){
	case 1: // if LED number is 1

		while (time > 0){ // keep executing until time has ended

			// set blue LED to high
			modifiedBitSequence = modify_bit(data[0], 2, 1);
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

			HAL_Delay(freq/2); // wait half of toggle frequnecy

			// set blue LED to low
			modifiedBitSequence = modify_bit(data[0], 2, 0);
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

			if(time - freq/2 <= 0){ // check if current time is has passed the toggle time
				break; // stop toggling
			}

			HAL_Delay(freq/2); // wait half of toggle frequency

			time -= freq; // subtract frequency time from total time
		}

	break;
	case 2: // if LED number is 2

		while (time > 0){ // keep executing until time has ended

			// set blue pedestrian LED to high
			modifiedBitSequence = modify_bit(data[1], 2, 1);
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

			HAL_Delay(freq/2); // wait half of toggle frequency

			// set blue pedestrian LED to low
			modifiedBitSequence = modify_bit(data[1], 2, 0);
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

			if(time - freq/2 <= 0){ // check if current time is has passed the toggle time
				break; // stop toggling
			}

			HAL_Delay(freq/2); // wait half of toggle frequency

			time -= freq; // subtract frequency time from total time
		}
	break;
	default:
		// do nothing
	}

} // end of function toggle_PL_led

/**
@brief set_red_PL_led, sets one of the red pedestrian LEDs high or low
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@param2 bool power, sets LED high (true) or low (false)
@param3 uint8_t ledNr, LED number
@return void, no return value
*/
void set_red_PL_led(uint8_t data[], bool power, uint8_t ledNr){

	uint8_t modifiedBitSequence; // 8 bit sequence, representing modified bit sequence

	switch(ledNr){
	case 1: // if LED number is 1

		if(power == true){ // check if pedestrian LED should be set to high

			// set first red pedestrian LED to high
			modifiedBitSequence = modify_bit(data[0], 4, 1); // set red LED to high
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set first red pedestrian LED to low
			modifiedBitSequence = modify_bit(data[0], 4, 0); // set red LED to low
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data
		}
	break;
	case 2: // if LED number is 2

		if(power == true){ // check if pedestrian LED should be set to high

			// set second red pedestrian LED to high
			modifiedBitSequence = modify_bit(data[1], 4, 1); // set red LED to high
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set second red pedestrian LED to low
			modifiedBitSequence = modify_bit(data[1], 4, 0); // set red LED to low
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	default:
		// do nothing
	}
} // end of function set_red_PL_led

/**
@brief set_green_PL_led, sets one of the green pedestrian LEDs high or low
@param1 uint8_t[] data, SPI data to transmit, representing traffic light LEDs
@param2 bool power, sets LED high (true) or low (false)
@param3 uint8_t ledNr, LED number
@return void, no return value
*/
void set_green_PL_led(uint8_t data[], bool power, uint8_t ledNr){

	uint8_t modifiedBitSequence; // 8 bit sequence, representing modified bit sequence

	switch(ledNr){
	case 1: // if LED number is 1

		if(power == true){ // check if pedestrian LED should be set to high

			// set first green pedestrian LED to high
			modifiedBitSequence = modify_bit(data[0], 3, 1); // set green LED to high
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be set to low

			// set first green pedestrian LED to low
			modifiedBitSequence = modify_bit(data[0], 3, 0); // set green LED to low
			data[0] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	case 2: // if LED number is 2

		if(power == true){ // check if pedestrian LED should be set to high

			// set second green pedestrian LED to high
			modifiedBitSequence = modify_bit(data[1], 3, 1); // set green LED to high
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		} else{ // if LED should be low

			// set second green pedestrian LED to low
			modifiedBitSequence = modify_bit(data[1], 3, 0); // set green LED to low
			data[1] = modifiedBitSequence; // set modified byte in data array
			set_traffic_leds(data); // transmit SPI data

		}

	break;
	default:
		// do nothing
	}
} // end of function set_green_PL_led

/**
@brief modify_bit, manipulates one bit in a byte
@param1 uint8_t nr, byte to be manipulated
@param2 uint8_t pos, position number of bit
@param3 uint8_t value, the desired value of the bit
@return uint8_t, the modified byte
*/
uint8_t modify_bit(uint8_t nr, uint8_t pos, uint8_t value)
{
    uint8_t mask = 1 << pos; // a byte representing 1, shifted left pos times

    return ((nr & ~mask) | (value << pos)); // return manipulated byte
}

