/**
 * Title 	Int Flash module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_clk.h"
#include "KinetisKE_csl_flash.h"


/**
 * @brief 	Flash Initialization
 * @param	None
 * @return	CSL_StatusTypeDef
 * @note	This function must be called after SystemClock_Init()!
**/
CSL_StatusTypeDef CSL_Flash_Init(void)
{
	//Enable Flash Clock
	__CSL_FLASH_CLK_ENABLE();
	
	//bus_clk >= 0.8MHz 
	uint32_t flash_clk = SystemBusClock;
	if(flash_clk < 800000)
		return CSL_Error;
	
	//Set FCLK Divider(FCLK = Bus_CLK / FTMRE->FCLKDIV)
	while(!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK));
	FTMRE->FCLKDIV = FTMRE_FCLKDIV_FDIV(flash_clk/(1000000)) | FTMRE_FCLKDIV_FDIVLCK_MASK;
		
	//Clear Flags
	FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;
	
	return CSL_OK;
}

/**
 * @brief 	Erase Flash Sector
 * @param	uint16_t SectorNum
				Sector number will be Erased
 * @return	None
 * @note	None
**/
void CSL_Flash_EraseSector(uint16_t SectorNum)
{
	assert_param(IS_FLASH_SECTOR_NUM(SectorNum));
	
	//Get address
	uint32_t addr = (uint32_t)SectorNum * FLASH_SECTOR_SIZE;

	//Write command & address
    FTMRE->FCCOBIX = 0;
    FTMRE->FCCOBHI = FC_Erase_Sector;
    FTMRE->FCCOBLO = addr >> 16;
    
    FTMRE->FCCOBIX = 1;
    FTMRE->FCCOBHI = (addr & 0xffff) >> 8;
    FTMRE->FCCOBLO = addr & 0xff;
	
	//execute cmd
	__FLASH_CLEAR_CMD_CPLT_IFLAG();
}

/**
 * @brief 	Write Data to Flash Sector
 * @param	uint16_t SectorNum
				Sector number will be Erased
 * @param	const uint8_t* pBuffer
				Data cache
 * @param	uint32_t size
				Data size
 * @param	uint32_t offset
				Address offset
 * @return	None
 * @note	None
**/
void CSL_Flash_WriteSector(uint16_t SectorNum, const uint8_t* pBuffer, uint32_t size, uint32_t offset)
{
	assert_param(IS_FLASH_SECTOR_NUM(SectorNum));
	
	//Get address
	uint32_t addr = (uint32_t)SectorNum * FLASH_SECTOR_SIZE;
	uint16_t step = 4;
	
	for(uint16_t i = 0; i < size; i += step)
	{
        FTMRE->FCCOBIX = 0;
        FTMRE->FCCOBHI = FC_Program_Flash;
        FTMRE->FCCOBLO = addr >> 16;
        
        FTMRE->FCCOBIX = 1;
        FTMRE->FCCOBHI = (addr & 0xFFFF) >> 8;
        FTMRE->FCCOBLO = addr & 0xFC;
        
        FTMRE->FCCOBIX = 2;
        FTMRE->FCCOBLO = pBuffer[0];
        FTMRE->FCCOBHI = pBuffer[1];
        
        FTMRE->FCCOBIX = 3;
        FTMRE->FCCOBLO = pBuffer[2];
        FTMRE->FCCOBHI = pBuffer[3];

        pBuffer += step;
        addr += step;

        //execute cmd
		__FLASH_CLEAR_CMD_CPLT_IFLAG();
    }
}

/**
 * @brief	FTMRE_IRQHandler ISR function
 * @note	called by FTMRE_IRQHandler()
**/
void CSL_FTMRE_IRQHandler(void)
{
    //Clear Interrupt 
	__FLASH_CLEAR_CMD_CPLT_IFLAG();
    
    //Error Occured
    if(FTMRE->FSTAT & (FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_MGSTAT_MASK))
    {
        CSL_FTMRE_ErrorCallback();
    }
    //no Error
	else
    {
        CSL_FTMRE_CMD_CpltCallback();
    }
}

/**
 * @brief	FTMRE_IRQHandler User-defined function after Command Complete
 * @note	do not modify this fucntion, but you can rewrite it in other file
**/
__weak void CSL_FTMRE_CMD_CpltCallback(void)
{
	__no_operation();
}

/**
 * @brief	FTMRE_IRQHandler User-defined function after Flash Command Error
 * @note	do not modify this fucntion, but you can rewrite it in other file
**/
__weak void CSL_FTMRE_ErrorCallback()
{
    __no_operation();
}

//EOF
