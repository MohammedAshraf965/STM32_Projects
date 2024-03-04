/*		Project: Smart Home Application
 * 		File: ultrasonic.c
 *
 *      Author: Mohammed Ashraf
 *      Description: C source file for the ultrasonic sensor module
 */

/************************************************************************************************
*************************************** Section: Includes ***************************************
************************************************************************************************/

#include "../Inc/ultrasonic.h"
#include "../Inc/ultrasonic_cfgType.h"

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

Std_ReturnType HCSR04_Init(uint8_t HCSR04_Instance, TIM_HandleTypeDef *TIM_Handle)
{
	Std_ReturnType ret_status = E_NOK;

	if(TIM_Handle == NULL)
	{
		ret_status = E_NOK;
	}
	else
	{

	}

	return ret_status;
}

Std_ReturnType HCSR04_Trigger(uint8_t HCSR04_Instance)
{
	Std_ReturnType ret_status = E_NOK;



	return ret_status;
}

Std_ReturnType HCSR04_TIM_OVF_ISR(TIM_HandleTypeDef *htim)
{
	Std_ReturnType ret_status = E_NOK;

	if(htim == NULL)
	{
		ret_status = E_NOK;
	}
	else
	{

	}

	return ret_status;
}

Std_ReturnType HCSR04_TIM_IC_ISR(TIM_HandleTypeDef *htim)
{
	Std_ReturnType ret_status = E_NOK;

	if(htim == NULL)
	{
		ret_status = E_NOK;
	}
	else
	{

	}

	return ret_status;
}

float HCSR04_Read(uint8_t HCSR04_Instance)
{

	return 0;
}
