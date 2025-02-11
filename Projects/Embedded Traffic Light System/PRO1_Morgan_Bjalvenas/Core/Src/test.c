/**
******************************************************************************
@brief Test file for PRO1 project
@file test.c
@author Morgan Bjälvenäs
@version 1.0
@date 20-December-2023
@brief Test functions and structures for PRO1 project
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "test.h"
#include "traffic_light_functions.h"
#include "stdbool.h"

uint8_t data[] = {0b10000000, 0b10000000, 0b10010000}; // representation of traffic and pedestrian LEDs

/**
@brief Test_program, main test function (to test remove comment on one of the test functions)
@param1 void, no parameter value
@return void, no return value
*/
void Test_program(void){
//	test_sending_SPI_data(); // test for sending SPI data
//	test_LEDs(); // testing LEDs
//	test_switches(); // testing switches
//	test_toggle(); // testing toggle of blue LEDs
} // end of function Test_program

/**
@brief test_sending_SPI_data, test sending specific SPI data to traffic light shield
@param1 void, no parameter value
@return void, no return value
*/
void test_sending_SPI_data(void){
	set_traffic_leds(data); // transmit SPI data
	HAL_Delay(10000); // wait 10 seconds
} // end of function test_sending_SPI_data


/**
@brief test_LEDs, test lighting up all LEDs on traffic light shield (excluding blue LEDs)
@param1 void, no parameter value
@return void, no return value
*/
void test_LEDs(void){
	uint8_t i;

	reset_traffic_leds(data); // reset traffic LEDs
	reset_pedestrian_leds(data); // reset pedestrian LEDs

	// light up all traffic LEDs in two second intervals
	for(i = 1; i < 5; i++){
		set_red_traffic_led(data, true, i); // light up red LED
		HAL_Delay(2000); // wait two seconds
		set_yellow_traffic_led(data, true, i); // light up yellow LED
		HAL_Delay(2000); // wait two seconds
		set_green_traffic_led(data, true, i); // light up green LED
		HAL_Delay(2000); // wait two seconds
	}

	// light up all pedestrian LEDs in two second intervals
	for(i = 1; i < 3; i++){
		set_red_PL_led(data, true, i); // light up red LED
		HAL_Delay(2000); // wait two seconds
		set_green_PL_led(data, true, i); // light up green LED
		HAL_Delay(2000); // wait two seconds
	}
} // end of function test_LEDs

/**
@brief test_switches, test all switches on traffic light shield, corresponding red LED will light up when switch is active
@param1 void, no parameter value
@return void, no return value
*/
void test_switches(void){

	reset_traffic_leds(data); // reset traffic LEDs
	reset_pedestrian_leds(data); // reset pedestrian LEDs

	for(;;){ // infinite loop
		bool PL1Hit = PL1_switch_hit() == true ? true : false; // if first pedestrian button is hit activate corresponding red LED
		set_red_PL_led(data, PL1Hit, 1); // set red LED

		bool PL2Hit = PL2_switch_hit() == true ? true : false; // if second pedestrian button is hit activate corresponding red LED
		set_red_PL_led(data, PL2Hit, 2); // set red LED

		bool TL1Car = TL1_car_active() == true ? true : false; // if first car is active activate corresponding red LED
		set_red_traffic_led(data, TL1Car, 1); // set red LED

		bool TL2Car = TL2_car_active() == true ? true : false; // if second car is active activate corresponding red LED
		set_red_traffic_led(data, TL2Car, 2); // set red LED

		bool TL3Car = TL3_car_active() == true ? true : false; // if third car is active activate corresponding red LED
		set_red_traffic_led(data, TL3Car, 3); // set red LED

		bool TL4Car = TL4_car_active() == true ? true : false; // if fourth car is active activate corresponding red LED
		set_red_traffic_led(data, TL4Car, 4); // set red LED

		HAL_Delay(100); // wait 100 ms

	}
} // end of function test_switches

/**
@brief test_toggle, test toggling blue LED when corresponding pedestrian button is hit
@param1 void, no parameter value
@return void, no return value
*/
void test_toggle(){

	for(;;){ // infinite loop

		if(PL1_switch_hit() == true){ // if first pedestrian button is hit
			toggle_PL_led(data, 1, 400, 8000); // toggle blue LED for 8 seconds with a frequency of 400 ms
		}

		if(PL2_switch_hit() == true){ // if second pedestrian button is hit
			toggle_PL_led(data, 2, 400, 8000); // toggle blue LED for 8 seconds with a frequency of 400 ms
		}
	}

} // end of function test_toggle
