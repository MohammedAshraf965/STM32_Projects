/*		Project: Smart Home Application
 * 		File: SCB.c
 *
 *      Author: Mohammed Ashraf
 *      Description: C source file for the System Control Block Module
 */

/************************************************************************************************
*************************************** Section: Includes ***************************************
************************************************************************************************/

#include "../Inc/SCB.h"

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

/*
 * @brief 	Set Priority Grouping
 * @details	Sets the priority grouping field using the required unlock sequence
 * 			The parameter PriorityGroup is assigned to the field SCB -> AIRCR [10:8] PRIGROUP field.
 * 			Only values from 0..7 are used
 * @param	PriorityGroup: Priority grouping field
 */
void SCB_SetPriorityGrouping(uint32_t PriorityGroup)
{
	uint32_t RegisterValue = 0;
	uint32_t PriorityGroupTemp = PriorityGroup & (uint32_t)0x07;

	RegisterValue = SCB -> AIRCR;
	RegisterValue &= ~((uint32_t)SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_VECTKEY_Msk);

	RegisterValue = (RegisterValue | (((uint32_t)0x05FA << SCB_AIRCR_VECTKEY_Pos) | (PriorityGroupTemp << SCB_AIRCR_PRIOGROUP_Pos)));
	SCB -> AIRCR = RegisterValue;
}
