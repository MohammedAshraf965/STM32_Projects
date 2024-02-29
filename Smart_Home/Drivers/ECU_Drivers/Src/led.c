/*		Project: Smart Home Application
 * 		File: led.c
 *
 *      Author: Mohammed Ashraf
 *      Description: C source file for the led module
 */

/************************************************************************************************
*************************************** Section: Includes ***************************************
************************************************************************************************/

#include "../Inc/led.h"
#include "../Inc/led_cfgType.h"

/********************************************************************************************************************
*************************************** Section: Static Function Declarations ***************************************
********************************************************************************************************************/


/********************************************************************************************************
*************************************** Section: Global Variables ***************************************
********************************************************************************************************/


/********************************************************************************************************************
*************************************** Section: Static Functions Definitions ***************************************
*********************************************************************************************************************/


/************************************************************************************************************
*************************************** Section: Function Definitions ***************************************
************************************************************************************************************/

Std_ReturnType LED_Init(const LED_t *led)
{
	Std_ReturnType ret_status = E_NOK;

	if(led != NULL)
	{
		if(led -> LED_GPIO == GPIOA)
			__HAL_RCC_GPIOA_CLK_ENABLE();
		else if(led -> LED_GPIO == GPIOB)
			__HAL_RCC_GPIOB_CLK_ENABLE();
		else if(led -> LED_GPIO == GPIOC)
			__HAL_RCC_GPIOB_CLK_ENABLE();

		HAL_GPIO_Init(led -> LED_GPIO, led -> LED_STATES);
		ret_status = E_OK;
	}
	else
	{
		ret_status = E_NOK;
	}

	return ret_status;
}

Std_ReturnType LED_Status(const LED_t *led, uint16_t *led_status)
{
	Std_ReturnType ret_status = E_NOK;

	if(led != NULL)
	{
		*led_status = HAL_GPIO_ReadPin(led -> LED_GPIO, (led -> LED_STATES) -> Pin);
		ret_status = E_OK;
	}
	else
	{
		ret_status = E_NOK;
	}

	return ret_status;
}

Std_ReturnType LED_Switch_On(const LED_t *led)
{
	Std_ReturnType ret_status = E_NOK;

	if(led != NULL)
	{
		HAL_GPIO_WritePin(led -> LED_GPIO, (led -> LED_STATES) -> Pin, LED_ON);
		ret_status = E_OK;
	}
	else
	{
		ret_status = E_NOK;
	}

	return ret_status;
}

Std_ReturnType LED_Switch_Off(const LED_t *led)
{
	Std_ReturnType ret_status = E_NOK;

	if(led != NULL)
	{
		HAL_GPIO_WritePin(led -> LED_GPIO, (led -> LED_STATES) -> Pin, LED_OFF);
		ret_status = E_OK;
	}
	else
	{
		ret_status = E_NOK;
	}

	return ret_status;
}

Std_ReturnType LED_Toggle(const LED_t* led)
{
	Std_ReturnType ret_status = E_NOK;

	if(led != NULL)
	{
		HAL_GPIO_TogglePin(led -> LED_GPIO, (led -> LED_STATES) -> Pin);
		ret_status = E_OK;
	}
	else
	{
		ret_status = E_NOK;
	}

	return ret_status;
}
