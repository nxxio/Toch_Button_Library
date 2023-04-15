/* Note:
 * Version 1.0
 * To use the library create a variable of type Button and GPIO_InitTypeDef.
 * Call functions Button_Init() and Button_Calibration().
 * ! Do not touch the sensor during calibration.
 * Process in an infinite loop the messages,returned Button_Proc() or
 * put processing inside a function Button_Proc()
 */
#pragma once
#include "gpio.h"
#define _USE_HAL_
#define _button_state uint8_t
#define _button_pressed (uint8_t)1
#define _button_not_pressed (uint8_t)0
//#define _USE_CMSIS_

typedef struct{
	uint16_t max_count;		// ! this field is calculated by Button_Calibration()
							// initialize this field to 0
	uint16_t counter;		// initialize this field to 0
	uint16_t CAP_pin;
	uint16_t SENSOR_pin;
	float sensivity; 			// threshold = sensivity*max_count, example 0.5*max_count
	GPIO_TypeDef* CAP_port;
	GPIO_TypeDef* SENSOR_port;
}Button;

_button_state Button_Proc(Button*,GPIO_InitTypeDef*);
void Button_Init(Button*,GPIO_InitTypeDef*);
void Button_Calibration(Button*,GPIO_InitTypeDef*);
