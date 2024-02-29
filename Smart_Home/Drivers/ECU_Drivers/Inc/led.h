/*		Project: Smart Home Application
 * 		File: led.h
 *
 *      Author: Mohammed Ashraf
 *      Description: C header file for the led module
 */

#ifndef ECU_DRIVERS_INC_LED_H_
#define ECU_DRIVERS_INC_LED_H_

/************************************************************************************************
*************************************** Section: Includes ***************************************
************************************************************************************************/

#include "stm32f4xx_hal.h"
#include "../Std_Types.h"

/**********************************************************************************************************
*************************************** Section: Macro Declarations ***************************************
**********************************************************************************************************/


/*******************************************************************************************************
*************************************** Section: Macro Functions ***************************************
*******************************************************************************************************/


/**************************************************************************************************
*************************************** Section: Data Types ***************************************
**************************************************************************************************/

typedef enum
{
	LED_OFF,
	LED_ON
}LED_Status_t;

typedef struct
{
	GPIO_TypeDef *LED_GPIO;
	GPIO_InitTypeDef *LED_STATES;
}LED_t;

/*************************************************************************************************************
*************************************** Section: Function Declarations ***************************************
*************************************************************************************************************/

Std_ReturnType LED_Init(const LED_t *led);
Std_ReturnType LED_Status(const LED_t *led, uint16_t *led_status);
Std_ReturnType LED_Switch_On(const LED_t *led);
Std_ReturnType LED_Switch_Off(const LED_t *led);
Std_ReturnType LED_Toggle(const LED_t *led);


#endif /* ECU_DRIVERS_INC_LED_H_ */