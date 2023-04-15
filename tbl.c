#include "tbl.h"
#ifdef _USE_HAL_
_button_state Button_Proc(Button* button ,GPIO_InitTypeDef* GPIO_button_config){

/*---------------------- Phase 1,Sensor charging ------------------------------*/

	// CAP OD Hi-Z
	HAL_GPIO_WritePin(button->CAP_port, button->CAP_pin, GPIO_PIN_SET);

	// SENSOR OD +3.3v, set Hi-Z PU
	GPIO_button_config->Pin = button->SENSOR_pin;
	GPIO_button_config->Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_button_config->Pull = GPIO_PULLUP;
	HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);
	HAL_GPIO_WritePin(button->SENSOR_port, button->SENSOR_pin, GPIO_PIN_SET);

/*----------------- Phase 2, Charging storage capacitor -----------------------*/

	// SENSOR OD Hi-Z, set Hi-Z NoPull
	GPIO_button_config->Pull = GPIO_NOPULL;
	HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);

	// connecting CAP to GND
	HAL_GPIO_WritePin(button->CAP_port, button->CAP_pin, GPIO_PIN_RESET);

/*------------------------- Phase 3, Voltage measurement -----------------------*/

	// SENSOR input mode
	GPIO_button_config->Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);

	button->counter++;

	if (HAL_GPIO_ReadPin(button->SENSOR_port, button->SENSOR_pin) == GPIO_PIN_SET){

		if(button->counter < button->max_count*button->sensivity ){

			// Discharge CAP and SENSOR
			GPIO_button_config->Mode = GPIO_MODE_OUTPUT_OD;
			HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);
			// connecting to GND
			HAL_GPIO_WritePin(button->SENSOR_port, button->SENSOR_pin, GPIO_PIN_RESET);

			/* Click handling user code */

			/* Click handling user code */

			button->counter = 0;
			return _button_pressed;
		}

		// Discharge CAP and SENSOR
		GPIO_button_config->Mode = GPIO_MODE_OUTPUT_OD;
		HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);
		// connecting to GND
		HAL_GPIO_WritePin(button->SENSOR_port, button->SENSOR_pin, GPIO_PIN_RESET);

		button->counter = 0;
		return _button_not_pressed;
	}

	return _button_not_pressed;
}

void Button_Init(Button* button,GPIO_InitTypeDef* GPIO_button_config){

	GPIO_button_config->Pin = button->CAP_pin;
	GPIO_button_config->Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_button_config->Pull = GPIO_NOPULL;
	GPIO_button_config->Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(button->CAP_port, GPIO_button_config);
	HAL_GPIO_WritePin(button->CAP_port, button->CAP_pin, GPIO_PIN_SET);

	GPIO_button_config->Pin = button->SENSOR_pin;
	HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);
	HAL_GPIO_WritePin(button->SENSOR_port, button->SENSOR_pin, GPIO_PIN_SET);

}

void Button_Calibration(Button* button,GPIO_InitTypeDef* GPIO_button_config){

	uint16_t temp = 0;
	uint16_t i=0;
	for(i; i<10; i++)
	{
		while(1){

			HAL_GPIO_WritePin(button->CAP_port, button->CAP_pin, GPIO_PIN_SET);

			GPIO_button_config->Pin = button->SENSOR_pin;
			GPIO_button_config->Mode = GPIO_MODE_OUTPUT_OD;
			GPIO_button_config->Pull = GPIO_PULLUP;
			HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);
			HAL_GPIO_WritePin(button->SENSOR_port, button->SENSOR_pin, GPIO_PIN_SET);

			GPIO_button_config->Pull = GPIO_NOPULL;
			HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);

			HAL_GPIO_WritePin(button->CAP_port, button->CAP_pin, GPIO_PIN_RESET);

			GPIO_button_config->Mode = GPIO_MODE_INPUT;
			HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);

			button->counter++;

			if (HAL_GPIO_ReadPin(button->SENSOR_port, button->SENSOR_pin) == GPIO_PIN_SET){

				GPIO_button_config->Mode = GPIO_MODE_OUTPUT_OD;
				HAL_GPIO_Init(button->SENSOR_port, GPIO_button_config);
				HAL_GPIO_WritePin(button->SENSOR_port, button->SENSOR_pin, GPIO_PIN_RESET);

				temp += button->counter;
				button->counter = 0;
				break;
			}
		}
	}

	button->max_count = temp/i;
}
#endif
