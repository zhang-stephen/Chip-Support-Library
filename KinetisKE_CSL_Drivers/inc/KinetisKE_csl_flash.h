/**
 * Title 	Int Flash module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
 * version	0.0.1 alpha
**/

#ifndef __KinetisKE_CSL_FLASH_H
#define __KinetisKE_CSL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl.h"

/**
 * Definations of Flash Paramters
**/
#define	FLASH_SECTOR_SIZE		((uint16_t)(512U))
#define FLASH_SECTOR_NUMBER		((uint16_t)(FLASH_SIZE/FLASH_SECTOR_SIZE))
#define FLASH_ALIGN_ADDR		((uint8_t)(0x0100U))

/**
 * Definations of Flash Commands
**/
#define FC_Erase_Verify_All_Sectors			0x01		//Verify all of sectors is wiped or not
#define FC_Erase_Verify_Sector				0x02		//Verify specific sector is wiped or not
#define FC_Erase_Verify_Sections			0x03		//Verify specific section is wiped or not
#define FC_Read_Once						0x04		//Read specific address with 64-bit data
#define FC_Program_Flash					0x06		//Program specific flash, no more than 64-bit
#define FC_Program_Once						0x07		//Program specific address with 64-bit data
#define FC_Erase_All_Blocks					0x08		//Erase all of Flash Sectors
#define FC_Erase_Block						0x09		//Erase specific block
#define FC_Erase_Sector						0x0A		//Erase specific sector
#define FC_Decyrpt_FLash					0x0B		//Decyrpt Flash
#define FC_Verify_Backdoor_Access_Key		0x0C		//Support exits MCU encryption status by verifying a set of security keys
#define FC_User_Margin_Level				0x0D
#define FC_Factory_Margin_Level				0x0E
#define FC_Set_NVM							0x0F

/* Macros Functions */
/**
 * @brief	Read Flash specific Sector
 * @param	__sector
				sector to read
 * @param 	__offset
				address offset in this sector
 * @param	__type
				specific data type
 * @return	__type
				data read
 * @note	None
**/
#define CSL_Flash_ReadData(__sector, __offset, __type)			(*(__type *)((uint32)(((__sector)*SECTOR_SIZE)+(__offset))))

/**
 * @brief	Clear Command Complete Interrupt Flags				
**/
#define __FLASH_CLEAR_CMD_CPLT_IFLAG()						  	(FTMRE->FSTAT |= FTMRE_FSTAT_CCIF_MASK)

/**
 * @brief	Enable/Disbale Command Complete Interrupt(CCIE)
**/
#define __FLASH_ENABLE_EXTI()							        (FTMRE->FCNFG |= FTMRE_FCNFG_CCIE_MASK)
#define __FLASH_DISABLE_EXTI()							        (FTMRE->FCNFG &= ~FTMRE_FCNFG_CCIE_MASK)

/**
 * @brief	Enable/Disable Flash Block Control(MCM->PLACR[ESFC])
**/
#define __FLASH_ENABLE_ESFC()									(MCM->PLACR |= MCM_PLACR_ESFC_MASK)
#define __FLASH_DISABLE_ESFC()									(MCM->PLACR &= ~MCM_PLACR_ESFC_MASK)

/**
 * @brief	Enable/Disable Flash Control Speculated(MCM->PLACR[DFCS])
**/
#define __FLASH_ENABLE_DFCS()									(MCM->PLACR &= ~MCM_PLACR_DFCS_MASK)
#define __FLASH_DISABLE_DFCS()									(MCM->PLACR |= MCM_PLACR_DFCS_MASK)

/**
 * @brief 	Enable/Disable Flash Data Speculated(MCM->PLACR[EFDS])
**/
#define __FLASH_ENABLE_EFDS()									(MCM->PLACR |= MCM_PLACR_EFDS_MASK)
#define __FLASH_DISABLE_EFDS()									(MCM->PLACR &= ~MCM_PLACR_EFDS_MASK)

/**
 * @brief 	Enable/Disable Flash Control Cache(MCM->PLACR[DFCC])
**/
#define __FLASH_ENABLE_DFCC()									(MCM->PLACR &= ~MCM_PLACR_DFCC_MASK)
#define __FLASH_DISABLE_DFCC()									(MCM->PLACR |= MCM_PLACR_DFCC_MASK)

/**
 * @brief	Enable/Disable Flash Control Instruction Cache(MCM->PLACR[DFCIC])
**/
#define __FLASH_ENABLE_DFCIC()									(MCM->PLACR &= ~MCM_PLACR_DFCIC_MASK)
#define __FLASH_DISABLE_DFCIC()									(MCM->PLACR |= MCM_PLACR_DFCIC_MASK)

/**
 * @brief 	Enable/Disable Flash Control Data Cache(MCM->PLACR[DFCDC])
**/
#define __FLASH_ENABLE_DFCDA()									(MCM->PLACR &= ~MCM_PLACR_DFCDA_MASK)
#define __FLASH_DISABLE_DFCDA()									(MCM->PLACR |= MCM_PLACR_DFCDA_MASK)

/**
 * @brief 	Clear Flash Control Cache(MCM->PlACR[CFCC])
**/
#define __FLASH_CLEAR_CONTROL_CACHE()							(MCM->PLACR |= MCM_PLACR_CFCC_MASK)

/* Functions of Int Flash*/
CSL_StatusTypeDef CSL_Flash_Init(void);
void CSL_Flash_EraseSector(uint16_t SectorNum);
void CSL_Flash_WriteSector(uint16_t SectorNum, const uint8_t* pBuffer, uint32_t size, uint32_t offset);
uint32_t CSL_Flash_GetSectorSize(void);

//Interrupt Functions
void CSL_FTMRE_IRQHandler(void);
void CSL_FTMRE_CMD_CpltCallback(void);
void CSL_FTMRE_ErrorCallback(void);

/* Defgroup FLASH_Private_Macros CORTEX Private Macros */
#define IS_FLASH_SECTOR_NUM(SECTOR_x)			(((SECTOR_x+1) >= 1) || \
												 ((SECTOR_x) <= (FLASH_SECTOR_NUMBER-1)))

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_FLASH_H*/

//EOF
