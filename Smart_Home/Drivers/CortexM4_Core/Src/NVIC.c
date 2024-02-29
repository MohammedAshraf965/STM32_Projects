/*		Project: Smart Home Application
 * 		File: NVIC.c
 *
 *      Author: Mohammed Ashraf
 *      Description: C source file for the NVIC module
 */

/************************************************************************************************
*************************************** Section: Includes ***************************************
************************************************************************************************/

#include "../Inc/NVIC.h"

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

/**
 * @brief 		Enable Interrupt
 * @detail 		Enables a device specific interrupt in the NVIC
 * @param IRQn:	Device specific interrupt number
 * @note		IRQn must not be negative
 **/
void NVIC_EnableIRQ(IRQn_Type IRQn)
{

}

/**
 * @brief 		Disable Interrupt
 * @detail 		Disables a device specific interrupt in the NVIC
 * @param IRQn:	Device specific interrupt number
 * @note		IRQn must not be negative
 **/
void NVIC_DisableIRQ(IRQn_Type IRQn)
{

}

/**
 * @brief 		Set Pending Interrupt
 * @detail 		Sets the pending bit of a device specific interrupt in the NVIC pending register
 * @param IRQn:	Device specific interrupt number
 * @note		IRQn must not be negative
 **/
void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{

}

/**
 * @brief 		Clear Pending Interrupt
 * @detail 		Clears the pending bit of a device specific interrupt in the NVIC pending register
 * @param IRQn:	Device specific interrupt number
 * @note		IRQn must not be negative
 **/
void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{

}

/**
 * @brief 		Get Active Interrupt
 * @detail 		Reads the active register in the NVIC and returns the active bit for a device specific interrupt in the NVIC
 * @param IRQn:	Device specific interrupt number
 * @return 0:	Interrupt status is not active
 * @return 1:	Interrupt status is active
 * @note		IRQn must not be negative
 **/
uint32_t NVIC_GetActive(IRQn_Type IRQn)
{

	return 0;
}
