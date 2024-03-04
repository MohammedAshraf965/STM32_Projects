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
	if((uint32_t)IRQn >= 0)
	{
		NVIC->ISER[(IRQn/32)] |= (1 << (IRQn%32));
		//NVIC->ISER[(uint32_t)IRQn >> 5] = (1UL << (((uint32_t)IRQn) & 0x1F));
	}
}

/**
 * @brief 		Disable Interrupt
 * @detail 		Disables a device specific interrupt in the NVIC
 * @param IRQn:	Device specific interrupt number
 * @note		IRQn must not be negative
 **/
void NVIC_DisableIRQ(IRQn_Type IRQn)
{
	if((uint32_t)IRQn >= 0)
	{
		NVIC->ICER[(IRQn/32)] |= (1 << (IRQn%32));
		//NVIC->ISER[(uint32_t)IRQn >> 5] = (1UL << (((uint32_t)IRQn) & 0x1F));
	}
}

/**
 * @brief 		Set Pending Interrupt
 * @detail 		Sets the pending bit of a device specific interrupt in the NVIC pending register
 * @param IRQn:	Device specific interrupt number
 * @note		IRQn must not be negative
 **/
void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
	if((uint32_t)IRQn >= 0)
	{
		NVIC->ISPR[(IRQn/32)] |= (1 << (IRQn%32));
		//NVIC->ISER[(uint32_t)IRQn >> 5] = (1UL << (((uint32_t)IRQn) & 0x1F));
	}
}

/**
 * @brief 		Clear Pending Interrupt
 * @detail 		Clears the pending bit of a device specific interrupt in the NVIC pending register
 * @param IRQn:	Device specific interrupt number
 * @note		IRQn must not be negative
 **/
void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
	if((uint32_t)IRQn >= 0)
	{
		NVIC->ICER[(IRQn/32)] |= (1 << (IRQn%32));
		//NVIC->ISER[(uint32_t)IRQn >> 5] = (1UL << (((uint32_t)IRQn) & 0x1F));
	}
	//NVIC->ISER[(uint32_t)IRQn >> 5] = (1UL << (((uint32_t)IRQn) & 0x1F));
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
	if((uint32_t)IRQn >= 0)
	{
		//IRQn_status = NVIC->ISER[(uint32_t)IRQn >> 5] & (1UL << (((uint32_t)IRQn) & 0x1F));
		return NVIC->IABR[(IRQn/32)] & ((uint32_t)(1 << (IRQn%32))) ? 1UL: 0UL;
	}
	else
	{
		return 0;
	}
}
