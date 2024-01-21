
#ifndef BOOTLOADER_H
#define BOOTLOADER_H

/***************************** Section: Includes *****************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "usart.h"
#include "crc.h"

/***************************** Section: Macro Declarations *****************************/

/*** Debugging features ***/

#define BL_DEBUG_UART 					&huart1

#define DEBUG_DISABLE					0
#define DEBUG_ENABLE					1
#define BL_DEBUG_ENABLE					DEBUG_ENABLE

#define BL_DEBUG_METHOD 				BL_ENABLE_UART_DEBUG_MODE

#define BL_ENABLE_UART_DEBUG_MODE		0x00
#define BL_ENABLE_SPI_DEBUG_MODE		0x01
#define BL_ENABLE_CAN_DEBUG_MODE		0x02

/*** Communication peripherals ***/

#define BL_HOST_COMMUNICATION_UART 		&huart2
#define CRC_ENGINE_OBJECT				&hcrc

#define BL_HOST_BUFFER_RX_SIZE			200

/*** BootLoader Supported Commands ***/
#define CBL_GET_VER_CMD					0x10
#define CBL_GET_HELP_CMD				0x11
#define CBL_GET_CID_CMD					0x12
#define CBL_GET_RDP_STATUS_CMD			0x13
#define CBL_GO_TO_ADDRESS_CMD			0x14
#define CBL_FLASH_ERASE_CMD				0x15
#define CBL_MEM_WRITE_CMD				0x16
#define CBL_EN_R_W_PRTOECT_CMD			0x17
#define CBL_MEM_READ_CMD				0x18
#define CBL_READ_SECTOR_STATUS			0x19
#define CBL_OTP_READ_CMD				0x20
#define CBL_CHANGE_ROP_LEVEL_CMD		0x21

/* CRC Commands */

#define CRC_TYPE_BYTE_SIZE 				4

#define CRC_VERIFICATION_FAILED			0x00
#define CRC_VERIFICATION_PASSED			0x01

/* CBL_GET_VER_CMD */

#define CBL_VENDOR_ID					100
#define CBL_BOOTLOADER_MAJOR_VERSION
#define CBL_BOOTLOADER_MINOR_VERSION
#define CBL_SW_MAJOR_VERSION			1
#define CBL_SW_MINOR_VERSION			0
#define CBL_SW_PATCH_VERSION			0

#define CBL_SEND_NACK				0xAB
#define CBL_SEND_ACK				0xCD

/* CBL_GO_TO_ADDRESS_CMD */

#define FLASH_SECTOR2_BASE_ADDRESS	0x80008000U
//#define FLASH_SECTOR4_BASE_ADDRESSS		0x08010000U

#define ADDRESS_INVALID				0x00
#define ADDRESS_VALID				0x01

#define STM32F401_SRAM1_SIZE		(64 * 1024)
#define STM32F401_FLASH_SIZE		(1024 * 1024)
#define STM32F401_SRAM1_END			(SRAM1_BASE + STM32F401_SRAM1_SIZE)
#define STM32F401_FLASH_END			(FLASH_BASE * STM32F401_FLASH_SIZE)

/* CBL_FLASH_ERASE_CMD */

#define CBL_FLASH_MAX_SECTOR_NUMBER	12
#define CBL_FLASH_MASS_ERASE		0xFF

#define	ERASE_SUCCESSFUL			0x01
#define	ERASE_UNSUCCESSFUL			0x00

#define SECTOR_NUMBER_VALID			0x02
#define SECTOR_NUMBER_INVALID		0x03

#define HAL_SUCCESSFUL_ERASE		0xFFFFFFFFU

/* CBL_MEM_WRITE_CMD */

#define FLASH_PAYLOAD_WRITE_FAILED	0x00
#define FLASH_PAYLOAD_WRITE_PASSED	0x01

/* CBL_GET_RDP_STATUS_CMD */
#define ROP_LEV_READ_INVALID		0x00
#define ROP_LEV_READ_VALID			0x01

/* CBL_CHANGE_ROP_LEVEL_CMD */
#define ROP_LEV_CHANGE_INVALID		0x00
#define ROP_LEV_CHANGE_VALID		0x01

/***************************** Section: Macro Functions *****************************/


/***************************** Section: Data Type Definitions *****************************/

typedef enum
{
	BL_OK = 0,
	BL_NOK
}BL_Status_t;

typedef void(*pMainApp)(void);
typedef void(*Jump_Ptr)(void);

/***************************** Section: Function Declarations *****************************/

void BL_Print_Message(char *format, ...);
BL_Status_t BL_UART_Fetch_Host_Command(void);

#endif /* BOOTLOADER_H */








