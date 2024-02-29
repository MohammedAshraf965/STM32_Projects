/*		Project: Smart Home Application
 * 		File: led_cfgType.c
 *
 *      Author: Mohammed Ashraf
 *      Description: C source file for the led configuration
 */

/************************************************************************************************
*************************************** Section: Includes ***************************************
************************************************************************************************/

#include "../Inc/led.h"

/********************************************************************************************************************
*************************************** Section: Static Function Declarations ***************************************
********************************************************************************************************************/


/********************************************************************************************************
*************************************** Section: Global Variables ***************************************
********************************************************************************************************/

static GPIO_InitTypeDef GPIO_InitStruct = {
		.Pin = GPIO_PIN_2,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW
};

const LED_t LED_CfgType =
{
		.LED_GPIO = GPIOB,
		.LED_STATES = &GPIO_InitStruct,
};

/********************************************************************************************************************
*************************************** Section: Static Functions Definitions ***************************************
*********************************************************************************************************************/


/************************************************************************************************************
*************************************** Section: Function Definitions ***************************************
************************************************************************************************************/

