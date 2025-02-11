/**
******************************************************************************
@brief Traffic light functions, function declarations for traffic system
@file traffic_light_functions.h
@author Morgan Bjälvenäs
@version 1.0
@date 18-November-2023
@brief Functions for traffic_light_functions.c
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#ifndef INC_TRAFFIC_LIGHT_FUNCTIONS_H_
#define INC_TRAFFIC_LIGHT_FUNCTIONS_H_
#include "gpio.h"
#include "stdbool.h"

void set_traffic_leds(uint8_t data[]);

void reset_traffic_leds(uint8_t data[]);

void reset_pedestrian_leds(uint8_t data[]);

bool TL1_car_active(void);

bool TL2_car_active(void);

bool TL3_car_active(void);

bool TL4_car_active(void);

bool PL1_switch_hit(void);

bool PL2_switch_hit(void);

void set_red_traffic_led (uint8_t data[], bool power, uint8_t ledNr);

void set_yellow_traffic_led (uint8_t data[], bool power, uint8_t ledNr);

void set_green_traffic_led (uint8_t data[], bool power, uint8_t ledNr);

void toggle_PL_led(uint8_t data[], uint8_t ledNr, uint32_t freq, int32_t time);

void set_red_PL_led(uint8_t data[], bool power, uint8_t ledNr);

void set_green_PL_led(uint8_t data[], bool power, uint8_t ledNr);

uint8_t modify_bit(uint8_t nr, uint8_t pos, uint8_t value);

#endif /* INC_TRAFFIC_LIGHT_FUNCTIONS_H_ */
