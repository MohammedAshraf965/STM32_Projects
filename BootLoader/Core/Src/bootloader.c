
/***************************** Section: Includes *****************************/

#include "bootloader.h"

/***************************** Section: Macro Declarations *****************************/

/***************************** Section: Macro Functions *****************************/

/***************************** Section: Global Variable Declarations *****************************/

/*** Host Buffer that contains the packets for the commands ***/
static uint8_t BL_Host_Buffer[BL_HOST_BUFFER_RX_SIZE];

/*** Bootloader supported commands list ***/
static uint8_t BootLoader_Supported_CMD[12] =
{
		CBL_GET_VER_CMD,
		CBL_GET_HELP_CMD,
		CBL_GET_CID_CMD,
		CBL_GET_RDP_STATUS_CMD,
		CBL_GO_TO_ADDRESS_CMD,
		CBL_FLASH_ERASE_CMD,
		CBL_MEM_WRITE_CMD,
		CBL_EN_R_W_PRTOECT_CMD,
		CBL_MEM_READ_CMD,
		CBL_READ_SECTOR_STATUS,
		CBL_OTP_READ_CMD,
		CBL_CHANGE_ROP_LEVEL_CMD
};

/***************************** Section: Static Functions Declarations *****************************/

/*** Supported Commmnds ***/
static void BootLoader_Get_Version(uint8_t *Host_Buffer);
static void BootLoader_Get_Help(uint8_t *Host_Buffer);
static void BootLoader_Get_Chip_ID(uint8_t *Host_Buffer);
static void BootLoader_Read_Protection_Level(uint8_t *Host_Buffer);
static void BootLoader_Jump_To_Address(uint8_t *Host_Buffer);
static void BootLoader_Erase_Flash(uint8_t *Host_Buffer);
static void BootLoader_Memory_Write(uint8_t *Host_Buffer);
static void BootLoader_Enable_RW_Protection(uint8_t *Host_Buffer);
static void BootLoader_Memory_Read(uint8_t *Host_Buffer);
static void BootLoader_Get_Sector_Protection_Status(uint8_t *Host_Buffer);
static void BootLoader_Read_OTP(uint8_t *Host_Buffer);
static void BootLoader_Change_Read_Protection_Level(uint8_t *Host_Buffer);

/*** Helper Functions ***/
static void BootLoader_Send_ACK(uint8_t Reply_Message);
static void BootLoader_Send_NACK(void);
static void BootLoader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_Length);
static uint8_t BootLoader_CRC_Verification(uint8_t *P_Data, uint32_t Data_Len, uint32_t CRC_Value);
static uint8_t Host_Jump_To_Address_Verification(uint32_t Jump_Addresss);
static uint8_t Perform_Flash_Erase(uint8_t Sector_Number, uint8_t Number_of_Sectors);
static uint8_t Flash_Memory_Write_Payload(uint8_t *Host_Payload, uint32_t Host_Address, uint16_t Payload_Length);
static uint8_t CBL_STM32F401_Get_RDP_Level(void);
static uint8_t Change_ROP_Level(uint32_t ROP_Level);

/*** Transition to Applications ***/
static void bootloader_jump_to_user_app(void);

/***************************** Section: Function Definitions *****************************/

static void bootloader_jump_to_user_app(void)
{
	/*	Value of the main stack pointer of  main application   */
	uint32_t MSP_Value = *((volatile uint32_t *) FLASH_SECTOR2_BASE_ADDRESS);
	/*  Reset Handler definition function of main application  */
	uint32_t MainAppAddr = *((volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS + 4));
	/* Fetch the Reset Handler Address of the user application */
	pMainApp Reset_HandlerAddr = (pMainApp)MainAppAddr;
	/* Set Main Stack Pointer */
	__set_MSP(MSP_Value);
	/* Deinitialization of modules */
	HAL_RCC_DeInit(); /* Resets the RCC clock configuration to the default reset state*/
	/* Jump to Application Reset Handler */
	Reset_HandlerAddr();
}

void BL_Print_Message(char *format, ...)
{
	char Message[100] = {0};

	va_list args;
	/* Enables access to variable arguments */
	va_start(args, format);
	/* Write formatted data from variable argument list to string */
	vsprintf(Message, format, args);
#if (BL_DEBUG_METHOD == BL_ENABLE_UART_DEBUG_MODE)
	/* Transmit formatted data through the defined UART */
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)Message, sizeof(Message), HAL_MAX_DELAY);
#elif (BL_DEBUG_METHOD == BL_ENABLE_SPI_DEBUG_MODE)

#elif (BL_DEBUG_METHOD == BL_ENABLE_CAN_DEBUG_MODE)

#endif
	/* Performs clean for an object initialized by the call of va_start */
	va_end(args);
}

BL_Status_t BL_UART_Fetch_Host_Command(void)
{
	BL_Status_t BL_Status = BL_NOK;
	HAL_StatusTypeDef Hal_Status = HAL_ERROR;
	/* Length of the frame */
	uint8_t Data_Length = 0;
	/* Initialize 0 values to the buffer */
	memset(BL_Host_Buffer, 0, BL_HOST_BUFFER_RX_SIZE);
	/* Receive the first byte containing the size the of rest of the frame */
	Hal_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, BL_Host_Buffer, 1, HAL_MAX_DELAY);

	if(Hal_Status != HAL_OK)
		BL_Status = BL_NOK;
	else
	{
		Data_Length = BL_Host_Buffer[0];
		/* Receive the remaining bytes containing the frame */
		Hal_Status = HAL_UART_Receive(BL_HOST_COMMUNICATION_UART, &BL_Host_Buffer[1], Data_Length, HAL_MAX_DELAY);

		if(Hal_Status != HAL_OK)
			BL_Status = BL_NOK;
		else
		{
			switch(BL_Host_Buffer[1])
			{
				case CBL_GET_VER_CMD:
					BootLoader_Get_Version(BL_Host_Buffer);
					break;
				case CBL_GET_HELP_CMD:
					BootLoader_Get_Help(BL_Host_Buffer);
					break;
				case CBL_GET_CID_CMD:
					BootLoader_Get_Chip_ID(BL_Host_Buffer);
					break;
				case CBL_GET_RDP_STATUS_CMD:
					BootLoader_Read_Protection_Level(BL_Host_Buffer);
					break;
				case CBL_GO_TO_ADDRESS_CMD:
					BL_Print_Message("Jump Bootloader to specified address \r\n");
					BootLoader_Jump_To_Address(BL_Host_Buffer);
					break;
				case CBL_FLASH_ERASE_CMD:
					BL_Print_Message("Mass Erase or Sector Erase of the user Flash Memory \r\n");
					BootLoader_Erase_Flash(BL_Host_Buffer);
					break;
				case CBL_MEM_WRITE_CMD:
					BootLoader_Memory_Write(BL_Host_Buffer);
					break;
				case CBL_EN_R_W_PRTOECT_CMD:
					BL_Print_Message("Enable Read/Write protection on different sectors of the user Flash Memory \r\n");
					BootLoader_Enable_RW_Protection(BL_Host_Buffer);
					break;
				case CBL_MEM_READ_CMD:
					BL_Print_Message("Read data from different memories of the MCU \r\n");
					BootLoader_Memory_Read(BL_Host_Buffer);
					break;
				case CBL_READ_SECTOR_STATUS:
					BL_Print_Message("Read all the Sector Protection Status \r\n");
					BootLoader_Get_Sector_Protection_Status(BL_Host_Buffer);
					break;
				case CBL_OTP_READ_CMD:
					BL_Print_Message("Read the OTP contents \r\n");
					BootLoader_Read_OTP(BL_Host_Buffer);
					break;
				case CBL_CHANGE_ROP_LEVEL_CMD:
					BootLoader_Change_Read_Protection_Level(BL_Host_Buffer);
					break;
				default:
					break;
			}
		}
		BL_Status = BL_OK;
	}



	return BL_Status;
}

/***************************** Section: Static Functions Definitions *****************************/

static void BootLoader_Send_ACK(uint8_t Reply_Message)
{
	uint8_t Ack_Value[2] = {0};
	Ack_Value[0] = CBL_SEND_ACK;
	Ack_Value[1] =  Reply_Message;

	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)Ack_Value, 2, HAL_MAX_DELAY);
}

static void BootLoader_Send_NACK()
{
	uint8_t Ack_Value = CBL_SEND_NACK;
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, &Ack_Value, 1, HAL_MAX_DELAY);
}

static void BootLoader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_Length)
{
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, Host_Buffer, Data_Length, HAL_MAX_DELAY);
}

static uint8_t BootLoader_CRC_Verification(uint8_t *P_Data, uint32_t Data_Len, uint32_t CRC_Value)
{
	uint8_t CRC_Status = CRC_VERIFICATION_FAILED;
	uint32_t MCU_CRC_Calculation = 0;
	uint32_t Data_Buffer = 0;

	/*	Calculate the MCU CRC	*/
	for(int i = 0; i < Data_Len; i++)
	{
		Data_Buffer = (uint32_t)P_Data[i];
		MCU_CRC_Calculation = HAL_CRC_Accumulate(CRC_ENGINE_OBJECT, &Data_Buffer, 1);
	}

	/*	Reset the CRC Calculation Engine  */
	__HAL_CRC_DR_RESET(CRC_ENGINE_OBJECT);

	/*	Compare Host CRC to the Calculated CRC	*/
	if(MCU_CRC_Calculation == CRC_Value)
		CRC_Status = CRC_VERIFICATION_PASSED;
	else
		CRC_Status = CRC_VERIFICATION_FAILED;


	return CRC_Status;
}

static uint8_t Host_Jump_To_Address_Verification(uint32_t Jump_Addresss)
{
	uint8_t Address_Verification = ADDRESS_INVALID;

	if((Jump_Addresss >= SRAM1_BASE) && (Jump_Addresss <= STM32F401_SRAM1_END))
	{
		Address_Verification = ADDRESS_VALID;
	}
	else if((Jump_Addresss >= FLASH_BASE) && (Jump_Addresss <= STM32F401_FLASH_END))
	{
		Address_Verification = ADDRESS_VALID;
	}
	else
	{
		Address_Verification = ADDRESS_INVALID;
	}

	return Address_Verification;
}

static uint8_t Perform_Flash_Erase(uint8_t Sector_Number, uint8_t Number_of_Sectors)
{
	uint8_t Validity_Status = SECTOR_NUMBER_INVALID;
	FLASH_EraseInitTypeDef pEraseInit;
	uint8_t Remaining_Sectors = 0;
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint32_t Sector_Error = 0;

	if(Number_of_Sectors > CBL_FLASH_MAX_SECTOR_NUMBER)
	{
		/* Number of Sectors out of range */
		Validity_Status = SECTOR_NUMBER_INVALID;
	}
	else
	{
		if((Sector_Number <= (CBL_FLASH_MAX_SECTOR_NUMBER - 1)) || (Sector_Number == CBL_FLASH_MASS_ERASE))
		{
			if(Sector_Number == CBL_FLASH_MASS_ERASE)
			{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Performing Mass Erase \r\n");
#endif
				pEraseInit.TypeErase = FLASH_TYPEERASE_MASSERASE; /* Flash Mass Erase */
			}
			else
			{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Performing Sector Erase \r\n");
#endif
				Remaining_Sectors = CBL_FLASH_MAX_SECTOR_NUMBER - Sector_Number;
				if(Number_of_Sectors > Remaining_Sectors)
				{
					Number_of_Sectors = Remaining_Sectors;
				}
				else {/* Empty */}

				pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS; /* Flash Sector Erase Only*/
				pEraseInit.Sector = Sector_Number; /* Initial FLASH sector to erase when Mass erase is disabled */
				pEraseInit.NbSectors = Number_of_Sectors; /* Number of sectors to be erased */
			}

			pEraseInit.Banks = FLASH_BANK_1; /* Bank 1 */
			pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3; /* Device operating range: 2.7V to 3.6V */

			/* Unlock the Flash Control Register Access */
			HAL_Status = HAL_FLASH_Unlock();
			HAL_Status = HAL_FLASHEx_Erase(&pEraseInit, &Sector_Error);

			if(Sector_Error == HAL_SUCCESSFUL_ERASE)
			{
				Validity_Status = ERASE_SUCCESSFUL;
			}
			else
			{
				Validity_Status = ERASE_UNSUCCESSFUL;
			}
			/* Lock the Flash Control Register Access */
			HAL_Status = HAL_FLASH_Lock();
		}
		else
		{
			Validity_Status = ERASE_UNSUCCESSFUL;
		}
	}

	return Validity_Status;
}

static uint8_t Flash_Memory_Write_Payload(uint8_t *Host_Payload, uint32_t Host_Address, uint16_t Payload_Length)
{
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t Flash_Payload_Status = FLASH_PAYLOAD_WRITE_FAILED;
	uint16_t Payload_Counter = 0;

	/* Unlock the Flash Control Register Access */
	HAL_Status = HAL_FLASH_Unlock();
	if(HAL_Status != HAL_OK)
	{
		Flash_Payload_Status = FLASH_PAYLOAD_WRITE_FAILED;
	}
	else
	{
		for(Payload_Counter = 0; Payload_Counter < Payload_Length; Payload_Counter++)
		{
			HAL_Status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Host_Address + Payload_Counter, Host_Payload[Payload_Counter]);

			if(HAL_Status != HAL_OK)
			{
				Flash_Payload_Status = FLASH_PAYLOAD_WRITE_FAILED;
				break;
			}
			else
			{
				Flash_Payload_Status = FLASH_PAYLOAD_WRITE_PASSED;
			}
		}
	}

	/* Lock the Flash Control Register Access */
	if(Flash_Payload_Status == FLASH_PAYLOAD_WRITE_PASSED && HAL_Status == HAL_OK)
	{
		HAL_Status = HAL_FLASH_Lock();
		if(HAL_Status != HAL_OK)
		{
			Flash_Payload_Status = FLASH_PAYLOAD_WRITE_FAILED;
		}
		else
		{
			Flash_Payload_Status = FLASH_PAYLOAD_WRITE_PASSED;
		}
	}
	else
	{
		Flash_Payload_Status = FLASH_PAYLOAD_WRITE_FAILED;
	}

	return Flash_Payload_Status;
}

static uint8_t CBL_STM32F401_Get_RDP_Level(void)
{
	FLASH_OBProgramInitTypeDef pOBInit;

	/* Get the Option byte configuration */
	HAL_FLASHEx_OBGetConfig(&pOBInit);

	return (uint8_t)(pOBInit.RDPLevel);
}

static uint8_t Change_ROP_Level(uint32_t ROP_Level)
{
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	FLASH_OBProgramInitTypeDef pOBInit;
	uint8_t ROP_Level_Status = ROP_LEV_CHANGE_INVALID;

	/* Unlock the FLASH Option Control Register Access */
	HAL_Status = HAL_FLASH_OB_Unlock();

	if(HAL_Status != HAL_OK)
	{
		ROP_Level_Status = ROP_LEV_CHANGE_INVALID;
	}
	else
	{
		/* Initialize FLASH Option Bytes */
		pOBInit.OptionType = OPTIONBYTE_RDP; /* RDP option byte configuration */
		pOBInit.Banks = FLASH_BANK_1;		 /* Select banks for WRP activation/deactivation of all sectors */
		pOBInit.RDPLevel = ROP_Level;		 /* Set the read protection level */

		/* Program Option Bytes */
		HAL_Status = HAL_FLASHEx_OBProgram(&pOBInit);

		if(HAL_Status != HAL_OK)
		{
			HAL_Status = HAL_FLASH_OB_Lock();
			ROP_Level_Status = ROP_LEV_CHANGE_INVALID;
		}
		else
		{
			if(HAL_Status != HAL_OK)
			{
				HAL_Status = HAL_FLASH_OB_Lock();
				ROP_Level_Status = ROP_LEV_CHANGE_INVALID;
			}
			else
			{
				/* Launch the option byte loading */
				HAL_Status = HAL_FLASH_OB_Launch();

				if(HAL_Status != HAL_OK)
				{
					HAL_Status = HAL_FLASH_OB_Lock();
					ROP_Level_Status = ROP_LEV_CHANGE_INVALID;
				}
				else
				{
					/* Lock the FLASH Option Control Register Access */
					HAL_Status = HAL_FLASH_OB_Lock();

					if(HAL_Status != HAL_OK)
					{
						ROP_Level_Status = ROP_LEV_CHANGE_INVALID;
					}
					else
					{
						ROP_Level_Status = ROP_LEV_CHANGE_VALID;
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("ROP changed to Level: %i \r\n", pOBInit.RDPLevel);
#endif
					}
				}
			}
		}
	}

	return ROP_Level_Status;
}

static void BootLoader_Get_Version(uint8_t *Host_Buffer)
{
	uint8_t BL_Version[4] = {CBL_VENDOR_ID, CBL_SW_MAJOR_VERSION, CBL_SW_MINOR_VERSION, CBL_SW_PATCH_VERSION};
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;

#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Read the Bootloader version from the MCU \r\n");
#endif

	/*	Extract Packet Length and CRC Value  */
	Host_CMD_Packet_Len = Host_Buffer[0];
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_BYTE_SIZE));

	/*	CRC Verification  */
	if(BootLoader_CRC_Verification((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_TYPE_BYTE_SIZE, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif
		BootLoader_Send_ACK(4);
		BootLoader_Send_Data_To_Host(BL_Version, 4);
	}
	else
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Get_Help(uint8_t *Host_Buffer)
{
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;

#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Read the commands supported by the Bootloader \r\n");
#endif

	/*	Extract Packet Length and CRC Value  */
	Host_CMD_Packet_Len = Host_Buffer[0];
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_BYTE_SIZE));

	/*	CRC Verification  */
	if(BootLoader_CRC_Verification((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_TYPE_BYTE_SIZE, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif
		BootLoader_Send_ACK(12);
		BootLoader_Send_Data_To_Host(BootLoader_Supported_CMD, 12);
	}
	else
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Get_Chip_ID(uint8_t *Host_Buffer)
{
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint16_t MCU_Identification_Number = 0;

#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Read the MCU Chip Identification Number \r\n");
#endif

	/*	Extract Packet Length and CRC Value  */
	Host_CMD_Packet_Len = Host_Buffer[0];
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_BYTE_SIZE));

	/*	CRC Verification  */
	if(BootLoader_CRC_Verification((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_TYPE_BYTE_SIZE, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif
		/* Get the Chip Identification Number */
		MCU_Identification_Number = (uint16_t)((DBGMCU -> IDCODE) & 0X00000FFF);
		/* Report to Host */
		BootLoader_Send_ACK(2);
		BootLoader_Send_Data_To_Host((uint8_t *)&MCU_Identification_Number, 2);
	}
	else
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Read_Protection_Level(uint8_t *Host_Buffer)
{
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint8_t RDP_Level = 0;

#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Read the Flash Protection Level on the MCU \r\n");
#endif

	/*	Extract Packet Length and CRC Value  */
	Host_CMD_Packet_Len = Host_Buffer[0];
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_BYTE_SIZE));

	/*	CRC Verification  */
	if(BootLoader_CRC_Verification((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_TYPE_BYTE_SIZE, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif
		BootLoader_Send_ACK(1);
		/* Read Protection Level */
		RDP_Level = CBL_STM32F401_Get_RDP_Level();
		/* Report Protection Level */
		BootLoader_Send_Data_To_Host((uint8_t *)&RDP_Level, 1);
	}
	else
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Jump_To_Address(uint8_t *Host_Buffer)
{
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint32_t Host_Jump_Addr = 0;
	uint8_t Address_Verification = ADDRESS_INVALID;

#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Read the Flash Read Protection Level \r\n");
#endif

	/*	Extract Packet Length and CRC Value  */
	Host_CMD_Packet_Len = Host_Buffer[0];
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_BYTE_SIZE));

	/*	CRC Verification  */
	if(BootLoader_CRC_Verification((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_TYPE_BYTE_SIZE, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif
		/* Send Acknowledge */
		BootLoader_Send_ACK(1);
		/* Extract the address from the Host Packet */
		Host_Jump_Addr = *((uint32_t *)&Host_Buffer[2]);
		/* Verify Address Validity */
		Address_Verification = Host_Jump_To_Address_Verification(Host_Jump_Addr);

		if(Address_Verification == ADDRESS_VALID)
		{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
			BL_Print_Message("Address Verification Succeeded\r\n");
#endif
			/* Report Address Verification Succeeded */
			BootLoader_Send_Data_To_Host((uint8_t *)&Address_Verification, 1);
			/* Jump to Address */
			Jump_Ptr Jump_Address = (Jump_Ptr)(Host_Jump_Addr + 1);
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
			BL_Print_Message("Jump to: 0x%X \r\n", Jump_Address);
#endif
			Jump_Address();
		}
		else
		{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
			BL_Print_Message("Address Verification Failed\r\n");
#endif
			/* Report Address Verification Failed */
			BootLoader_Send_Data_To_Host((uint8_t *)&Address_Verification, 1);
		}
	}
	else
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif
		/* Send Not Acknowledge */
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Erase_Flash(uint8_t *Host_Buffer)
{
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint8_t Erase_Status = 0;

#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Mass Erase or Sector Erase of the user Flash Memory \r\n");
#endif

	/*	Extract Packet Length and CRC Value  */
	Host_CMD_Packet_Len = Host_Buffer[0];
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_BYTE_SIZE));

	/*	CRC Verification  */
	if(BootLoader_CRC_Verification((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_TYPE_BYTE_SIZE, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif
		BootLoader_Send_ACK(1);
		Erase_Status = Perform_Flash_Erase(Host_Buffer[2], Host_Buffer[3]);

		if(Erase_Status == ERASE_SUCCESSFUL)
		{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("Sector Erase succeeded \r\n");
#endif
			BootLoader_Send_Data_To_Host((uint8_t *)&Erase_Status, 1);
		}
		else
		{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("Sector Erase failed \r\n");
#endif
			BootLoader_Send_Data_To_Host((uint8_t *)&Erase_Status, 1);
		}
	}
	else
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Memory_Write(uint8_t *Host_Buffer)
{
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint32_t Host_Address = 0;
	uint8_t Payload_Length = 0;
	uint8_t Address_Verification = ADDRESS_INVALID;
	uint8_t Flash_Payload_Status = FLASH_PAYLOAD_WRITE_FAILED;

#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message(" Write data into different memories of the MCU \r\n");
#endif

	/*	Extract Packet Length and CRC Value  */
	Host_CMD_Packet_Len = Host_Buffer[0];
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_BYTE_SIZE));

	/*	CRC Verification  */
	if(BootLoader_CRC_Verification((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_TYPE_BYTE_SIZE, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif

		BootLoader_Send_ACK(1);
		Host_Address = *((uint32_t *)(&Host_Buffer[2]));
		Payload_Length = Host_Buffer[6];
		Address_Verification = Host_Jump_To_Address_Verification(Host_Address);

		if(Address_Verification == ADDRESS_VALID)
		{
			Flash_Payload_Status = Flash_Memory_Write_Payload((uint8_t *)&Host_Buffer[7], Host_Address, Payload_Length);

			if(Flash_Payload_Status == FLASH_PAYLOAD_WRITE_PASSED)
			{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
			BL_Print_Message("Payload Write Succeeded \r\n");
#endif
			/* Report Address Verification Failed */
			BootLoader_Send_Data_To_Host((uint8_t *)&Address_Verification, 1);
			}
			else
			{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
			BL_Print_Message("Payload Write Failed\r\n");
#endif
			/* Report Address Verification Failed */
			BootLoader_Send_Data_To_Host((uint8_t *)&Address_Verification, 1);
			}
		}
		else
		{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
			BL_Print_Message("Address Verification Failed\r\n");
#endif
			/* Report Address Verification Failed */
			BootLoader_Send_Data_To_Host((uint8_t *)&Address_Verification, 1);
		}
	}
	else
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif
		BootLoader_Send_NACK();
	}
}

static void BootLoader_Change_Read_Protection_Level(uint8_t *Host_Buffer)
{
	uint16_t Host_CMD_Packet_Len = 0;
	uint32_t Host_CRC32 = 0;
	uint8_t RDP_Level_Status = ROP_LEV_CHANGE_INVALID;
	uint8_t Host_ROP_Level = 0;

#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
	BL_Print_Message("Change Read Protection Level of the user Flash Memory \r\n");
#endif

	/*	Extract Packet Length and CRC Value  */
	Host_CMD_Packet_Len = Host_Buffer[0];
	Host_CRC32 = *((uint32_t *)((Host_Buffer + Host_CMD_Packet_Len) - CRC_TYPE_BYTE_SIZE));

	/*	CRC Verification  */
	if(BootLoader_CRC_Verification((uint8_t *)&Host_Buffer[0], Host_CMD_Packet_Len - CRC_TYPE_BYTE_SIZE, Host_CRC32) == CRC_VERIFICATION_PASSED)
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Passed \r\n");
#endif
		BootLoader_Send_ACK(1);
		/* Change the Read Protection Level */
		Host_ROP_Level = Host_Buffer[2];

		if(Host_ROP_Level == 0)
		{
			Host_ROP_Level = OB_RDP_LEVEL_0;
		}
		else if(Host_ROP_Level == 1)
		{
			Host_ROP_Level = OB_RDP_LEVEL_1;
		}
		else
		{
			RDP_Level_Status = ROP_LEV_CHANGE_INVALID;
		}

		RDP_Level_Status = Change_ROP_Level(Host_ROP_Level);
		/* Report Protection Level */
		BootLoader_Send_Data_To_Host((uint8_t *)&RDP_Level_Status, 1);
	}
	else
	{
#if (BL_DEBUG_ENABLE == DEBUG_ENABLE)
		BL_Print_Message("CRC Verification Failed \r\n");
#endif
		BootLoader_Send_NACK();
	}
}


/***************************** Sections: Unimplemented Static Functions *****************************/
static void BootLoader_Enable_RW_Protection(uint8_t *Host_Buffer)
{

}

static void BootLoader_Memory_Read(uint8_t *Host_Buffer)
{

}

static void BootLoader_Get_Sector_Protection_Status(uint8_t *Host_Buffer)
{

}

static void BootLoader_Read_OTP(uint8_t *Host_Buffer)
{

}
