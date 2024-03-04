/*
 * 		File: SCB.h
 *
 *      Author: Mohammed Ashraf
 *      Description: C header file for the System Control Block Module
 */

#ifndef CORTEXM4_CORE_INC_NVIC_H_
#define CORTEXM4_CORE_INC_NVIC_H_


/************************************************************************************************
*************************************** Section: Includes ***************************************
************************************************************************************************/

#include "Std_Types.h"

/**********************************************************************************************************
*************************************** Section: Macro Declarations ***************************************
**********************************************************************************************************/

#define SCB_BASE					0xE000ED00UL
#define SCB							((SCB_t *)(SCB_BASE))

#define SCB_PRIORITYGROUP_0			0x00000007U
#define SCB_PRIORITYGROUP_1			0x00000006U
#define SCB_PRIORITYGROUP_2			0x00000005U
#define SCB_PRIORITYGROUP_3			0x00000004U
#define SCB_PRIORITYGROUP_4			0x00000003U

#define SCB_AIRCR_PRIOGROUP_Pos		8U
#define SCB_AIRCR_PRIOGROUP_Msk		(0x0007UL << SCB_AIRCR_PRIOGROUP_Pos)

#define SCB_AIRCR_VECTKEY_Pos	16U
#define SCB_AIRCR_VECTKEY_Msk		(0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)

/*******************************************************************************************************
*************************************** Section: Macro Functions ***************************************
*******************************************************************************************************/


/**************************************************************************************************
*************************************** Section: Data Types ***************************************
**************************************************************************************************/

typedef struct
{
	uint32_t CPUID;
	uint32_t ICSR;
	uint32_t VTOR;
	uint32_t AIRCR;
	uint32_t SCR;
	uint32_t CCR;
	uint32_t SHPR1;
	uint32_t SHPR2;
	uint32_t SHPR3;
	uint32_t SHCSR;
	uint32_t CFSR;
	/*
	uint32_t MMSR;
	uint32_t BFSR;
	uint32_t UFSR;
	uint32_t HFSR;
	uint32_t MMAR;
	uint32_t BFAR;
	uint32_t AFSR;
	*/
}SCB_t;

/*************************************************************************************************************
*************************************** Section: Function Declarations ***************************************
*************************************************************************************************************/

/*
 * @brief 	Set Priority Grouping
 * @details	Sets the priority grouping field using the required unlock sequence
 * 			The parameter PriorityGroup is assigned to the field SCB -> AIRCR [10:8] PRIGROUP field.
 * 			Only values from 0..7 are used
 * @param	PriorityGroup: Priority grouping field
 */
void SCB_SetPriorityGrouping(uint32_t PriorityGroup);

#endif /* CORTEXM4_CORE_INC_NVIC_H_ */
