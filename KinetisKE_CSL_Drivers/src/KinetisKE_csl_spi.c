/**
 * Title 	SPI module in CSL for KEAZxxx(Head File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_spi.h"

/* Private Functions Declerations of SPI */
static void SPI_Config(SPI_HandleTypeDef* cspi);
static CSL_StatusTypeDef SPI_WaitFlag(SPI_HandleTypeDef* cspi, uint8_t Flag, Flag_Status Status, uint32_t TickStart, uint32_t Timeout);
static void SPI_TxISR(struct __SPI_HandleTypeDef* cspi);
static void SPI_RxISR(struct __SPI_HandleTypeDef* cspi);
static void SPI_2LinesTxISR(struct __SPI_HandleTypeDef* cspi);
static void SPI_2LinesRxISR(struct __SPI_HandleTypeDef* cspi);
static void SPI_CloseTxISR(struct __SPI_HandleTypeDef* cspi);
static void SPI_CloseRxISR(struct __SPI_HandleTypeDef* cspi);
static void SPI_CloseTxRxISR(struct __SPI_HandleTypeDef* cspi);


/* Public Functions Definations of SPI */
/**
 * @brief	Initialize the SPI
 * @param
 * @return 
 * @note
**/
CSL_StatusTypeDef CSL_SPI_Init(SPI_HandleTypeDef* cspi)
{
	//parameters check
	if(cspi == NULL)
		return CSL_Error;
	
	assert_param(IS_SPI_BAUDRATEPSC(cspi->Init.PreScaler));
	assert_param(IS_SPI_BAUDRATESC(cspi->Init.Scaler));
	assert_param(IS_SPI_MODE(cspi->Init.Mode));
	assert_param(IS_SPI_POLARITY(cspi->Init.CLKPolarity));
	assert_param(IS_SPI_PHASE(cspi->Init.CLKPhase));
	assert_param(IS_SPI_DIRECTIONS(cspi->Init.Direction));
	assert_param(IS_SPI_NSS(cspi->Init.NSS));
	
	//Configure SPI
	SPI_Config(cspi);
	
	//Msp Init
	if(cspi->gState == CSL_SPI_STATE_RESET)
	{
		__CSL_UNLOCK(cspi);
		CSL_SPI_MspInit(cspi);
	}
	
	//Enable SPI
	__CSL_SPI_ENABLE(cspi);
	
	//Set SPI gState
	cspi->gState = CSL_SPI_STATE_READY;
	cspi->ErrorCode = CSL_SPI_ERROR_NONE;
	
	return CSL_OK;
}

/**
 * @brief	Deinitialize the SPI
 * @param
 * @return 
 * @note
**/
CSL_StatusTypeDef CSL_SPI_DeInit(SPI_HandleTypeDef* cspi)
{
	if(cspi == NULL)
		return CSL_Error;
	
	//Flush the SPI Control Registers
	cspi->Instance->BR = 0x00u;
	cspi->Instance->M = 0x00u;
	cspi->Instance->C1 = 0x00u;
	cspi->Instance->C2 = 0x00u;
	
	//Lock SPI
	__CSL_LOCK(cspi);
	
	//Reset gState & Error Code
	cspi->gState = CSL_SPI_STATE_RESET;
	cspi->ErrorCode = CSL_SPI_ERROR_NONE;
	
	//Msp DeInit
	CSL_SPI_MspDeInit(cspi);
	
	return CSL_OK;
}

/**
 * @brief	Msp Init of SPI
 * @param
 * @return 
 * @note
**/
__weak void CSL_SPI_MspInit(SPI_HandleTypeDef* cspi)
{
	UNUSED(cspi);
}

/**
 * @brief	Msp DeInit of SPI
 * @param
 * @return 
 * @note
**/
__weak void CSL_SPI_MspDeInit(SPI_HandleTypeDef* cspi)
{
	UNUSED(cspi);
}

/**
 * @brief	
 * @param
 * @return 
 * @note
**/
CSL_StatusTypeDef CSL_SPI_Transmit(SPI_HandleTypeDef* cspi, uint8_t* TxBuffer, uint16_t Size, uint32_t Timeout)
{
	//Parameters Check
	assert_param(IS_SPI_DIRECTIONS(cspi->Init.Direction));
	
	uint32_t tickstart = 0u;
	CSL_StatusTypeDef errcode = CSL_OK;
	
	//Process Lock
	__CSL_LOCK(cspi);
	
	//Init TickStart for Timeout Management
	tickstart = CSL_GetTick();
	
	if(cspi->gState != CSL_SPI_STATE_READY)
	{
		errcode = CSL_Busy;
		goto ERROR;
	}
	
	if((TxBuffer == NULL) || (Size == 0u))
	{
		errcode = CSL_Error;
		goto ERROR;
	}
	
	//Set transcation Information
	cspi->gState = CSL_SPI_STATE_BUSY_TX;
	cspi->ErrorCode = CSL_SPI_ERROR_NONE;
	cspi->pTxBuffer = TxBuffer;
	cspi->pTxXferCount = Size;
	cspi->pTxXferSize = Size;
	
	//Init field not used in handle to zero
	cspi->pRxBuffer = NULL;
	cspi->pRxXferCount = 0u;
	cspi->pRxXferSize = 0u;
	cspi->TxISR = NULL;
	cspi->RxISR = NULL;
	
	//Commucate Directions: 1Line
	if(cspi->Init.Direction == SPI_DIRECTION_1LINES)
	{
		__CSL_SPI_1Line_Tx(cspi);
	}
	
	//Check SPI is enabled or not
	if((cspi->Instance->C1 & SPI_C1_SPE_MASK) == 0x00u)
	{
		__CSL_SPI_ENABLE(cspi);
	}
	
	//Transmit in 8-bit Mode
	if((cspi->Init.Mode == SPI_MODE_SLAVE)|| (cspi->pTxXferCount == 0x01u))
	{
		*((__IO uint8_t*)&cspi->Instance->D) = (*(cspi->pTxBuffer));
		cspi->pTxBuffer += sizeof(uint8_t);
		cspi->pTxXferCount--;
	}
	while (cspi->pTxXferCount > 0u)
	{
		// Wait until TXE flag is set to send data 
		if(__CSL_SPI_GET_FLAG(cspi, SPI_S_SPTEF_MASK))
		{
			*((__IO uint8_t*)&cspi->Instance->D) = (*(cspi->pTxBuffer));
			cspi->pTxBuffer += sizeof(uint8_t);
			cspi->pTxXferCount--;
		}
		else
		{
			/* Timeout management */
			if((Timeout == 0U) || ((Timeout != CSL_MAX_DELAY) && ((CSL_GetTick()-tickstart) >=  Timeout)))
			{
				errcode = CSL_Timeout;
				goto ERROR;
			}
		}
	}
	
	//Wait TXE Flag
	if(SPI_WaitFlag(cspi, SPI_S_SPTEF_MASK, SET, tickstart, Timeout) != CSL_OK)
	{
		errcode = CSL_Timeout;
		goto ERROR;
	}
	
ERROR:
	cspi->gState = CSL_SPI_STATE_READY;
	//Process Unlock
	__CSL_UNLOCK(cspi);
	return errcode;
}

/**
 * @brief	
 * @param
 * @return 
 * @note
**/
CSL_StatusTypeDef CSL_SPI_Receive(SPI_HandleTypeDef* cspi, uint8_t* RxBuffer, uint16_t Size, uint32_t Timeout)
{
	//Parameters Check
	assert_param(IS_SPI_DIRECTIONS(cspi->Init.Direction));
	
	uint32_t tickstart = 0u;
	CSL_StatusTypeDef errcode = CSL_OK;
	
	//Process Lock
	__CSL_LOCK(cspi);
	
	//Init TickStart for Timeout Management
	tickstart = CSL_GetTick();
	
	if(cspi->gState != CSL_SPI_STATE_READY)
	{
		errcode = CSL_Busy;
		goto ERROR;
	}
	
	if((RxBuffer == NULL) || (Size == 0u))
	{
		errcode = CSL_Error;
		goto ERROR;
	}
	
	//Set receipient Information
	cspi->gState = CSL_SPI_STATE_BUSY_RX;
	cspi->ErrorCode = CSL_SPI_ERROR_NONE;
	cspi->pRxBuffer = RxBuffer;
	cspi->pRxXferCount = Size;
	cspi->pRxXferSize = Size;
	
	//Init field not used in handle to zero
	cspi->pTxBuffer = NULL;
	cspi->pTxXferCount = 0u;
	cspi->pTxXferSize = 0u;
	cspi->TxISR = NULL;
	cspi->RxISR = NULL;
	
	//Commucate Directions: 1Line
	if(cspi->Init.Direction != SPI_DIRECTION_2LINES)
	{
		__CSL_SPI_1Line_Rx(cspi);
	}
	
	//Check SPI is enabled or not
	if((cspi->Instance->C1 & SPI_C1_SPE_MASK) == 0x00u)
	{
		__CSL_SPI_ENABLE(cspi);
	}
	
	//Receive in 8-bit Mode
	if((cspi->Init.Mode == SPI_MODE_SLAVE) || (cspi->pRxXferCount == 0x01u))
	{
		*(cspi->pRxBuffer) = *((__IO uint8_t*)&cspi->Instance->D);
		cspi->pRxBuffer += sizeof(uint8_t);
		cspi->pRxXferCount--;
	}
	while (cspi->pRxXferCount > 0u)
	{
		// Wait until SPRF flag is set to send data 
		if(__CSL_SPI_GET_FLAG(cspi, SPI_S_SPRF_MASK))
		{
			*(cspi->pRxBuffer) = *((__IO uint8_t*)&cspi->Instance->D);
			cspi->pRxBuffer += sizeof(uint8_t);
			cspi->pRxXferCount--;
		}
		else
		{
			/* Timeout management */
			if((Timeout == 0U) || ((Timeout != CSL_MAX_DELAY) && ((CSL_GetTick()-tickstart) >=  Timeout)))
			{
				errcode = CSL_Timeout;
				goto ERROR;
			}
		}
	}
	
	//Wait SPRF Flag
	if(SPI_WaitFlag(cspi, SPI_S_SPRF_MASK, RESET, tickstart, Timeout) != CSL_OK)
	{
		errcode = CSL_Timeout;
		goto ERROR;
	}
	
ERROR:
	cspi->gState = CSL_SPI_STATE_READY;
	//Process unlock
	__CSL_UNLOCK(cspi);
	return errcode;
}

/**
 * @brief	
 * @param
 * @return 
 * @note
**/
CSL_StatusTypeDef CSL_SPI_TransmitReceive(SPI_HandleTypeDef* cspi, uint8_t* TxBuffer, uint8_t* RxBuffer, uint16_t Size, uint32_t Timeout)
{
	//Parameters Check
	assert_param(IS_SPI_DIRECTIONS(cspi->Init.Direction));
	
	uint8_t tmp = 0u, tmp1 = 0u;
	uint32_t tickstart = 0u;
	// Variable used to alternate Rx and Tx during transfer 
  	uint32_t txallowed = 1u;
	CSL_StatusTypeDef errcode = CSL_OK;
	
	//Process lock
	__CSL_LOCK(cspi);
	
	//Init tick start for timeout management
	tickstart = CSL_GetTick();
	
	tmp = cspi->gState;
	tmp1 = cspi->Init.Mode;
	
	if(!((tmp == CSL_SPI_STATE_READY) || \
		((tmp1 == SPI_MODE_MASTER) && (cspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmp == CSL_SPI_STATE_BUSY_RX))))
	{
		errcode = CSL_Busy;
		goto ERROR;
	}
	
	if((TxBuffer == NULL) || (RxBuffer == NULL) || (Size == 0u))
	{
		errcode = CSL_Error;
		goto ERROR;
	}
	
	//Do not overwrite in case of CSL_SPI_STATE_BUSY_RX
	if(cspi->gState == CSL_SPI_STATE_READY)
	{
		cspi->gState = CSL_SPI_STATE_BUSY_TX_RX;
	}
	
	/* Set the transaction information */
	cspi->ErrorCode   	= CSL_SPI_ERROR_NONE;
	cspi->pRxBuffer  	= RxBuffer;
	cspi->pRxXferCount 	= Size;
	cspi->pRxXferSize  	= Size;
	cspi->pTxBuffer  	= TxBuffer;
	cspi->pTxXferCount 	= Size;
	cspi->pTxXferSize  	= Size;
	
	/*Init field not used in handle to zero */
	cspi->RxISR       	= NULL;
	cspi->TxISR       	= NULL;
	
	//Check SPI is enabled or not
	if((cspi->Instance->C1 & SPI_C1_SPE_MASK) == 0x00u)
	{
		__CSL_SPI_ENABLE(cspi);
	}
	
	// Transmit and Receive data in 8 Bit mode 
	if((cspi->Init.Mode == SPI_MODE_SLAVE) || (cspi->pTxXferCount == 1u))
	{
		*((__IO uint8_t*)&cspi->Instance->D) = *(cspi->pTxBuffer);
		cspi->pTxBuffer += sizeof(uint8_t);
		cspi->pTxXferCount--;
	}
	while((cspi->pRxXferCount > 0u) || (cspi->pTxXferCount > 0u))
	{
		//Check SPTEF flag
		if(txallowed && (cspi->pTxXferCount > 0u) && (__CSL_SPI_GET_FLAG(cspi, SPI_S_SPTEF_MASK)))
		{
			*((__IO uint8_t*)&cspi->Instance->D) = *(cspi->pTxBuffer++);
			cspi->pTxXferCount--;
			txallowed = 0u;
		}
		
		//wait until SPRF flag is RESET
		if((cspi->pRxXferCount > 0u) && (__CSL_SPI_GET_FLAG(cspi, SPI_S_SPRF_MASK)))
		{
			(*(uint8_t *)RxBuffer++) = cspi->Instance->D;
			cspi->pRxXferCount--;
			/* Next Data is a Transmission (Tx). Tx is allowed */ 
			txallowed = 1u;
		}
		if((Timeout != CSL_MAX_DELAY) && ((CSL_GetTick()-tickstart) >=  Timeout))
		{
			errcode = CSL_Timeout;
			goto ERROR;
		}
	}
	
	/* Wait until TXE flag */
	
	if(SPI_WaitFlag(cspi, SPI_S_SPTEF_MASK, SET, Timeout, tickstart) != CSL_OK)
	{
		errcode = CSL_Timeout;
		goto ERROR;
	}
	
ERROR:
	cspi->gState = CSL_SPI_STATE_READY;
	//Process unlock
	__CSL_UNLOCK(cspi);
	return errcode;
}

/**
 * @brief 	Get SPI current Status
 * @param
 * @return 
 * @note
**/
SPI_StateTypeDef CSL_SPI_GetState(SPI_HandleTypeDef* cspi)
{
	return cspi->gState;
}

/**
 * @brief	Get SPI Error Code
 * @param
 * @return 
 * @note
**/
uint8_t CSL_SPI_GetError(SPI_HandleTypeDef* cspi)
{
	return cspi->ErrorCode;
}

/**
 * @brief 	Get SPI BaudRate in bps(bits/s)
 * @param
 * @return 
 * @note
**/
uint32_t CSL_SPI_GetBaudRate(SPI_HandleTypeDef* cspi)
{
	uint16_t psc = 0, sc = 0;
	
	psc = (cspi->Init.PreScaler) >> SPI_BR_SPPR_SHIFT;
	sc = 1 << (cspi->Init.Scaler + 1u);
	return (uint32_t)(SystemBusClock / (psc+1) / sc);
}

/**
 * @brief 	Get SPI match value
 * @param
 * @return 
 * @note
**/
uint8_t CSL_SPI_GetMatchValue(SPI_HandleTypeDef* cspi)
{
	return cspi->Instance->M;
}

/**
 * @brief	Set SPI Match Value
 * @param
 * @return 
 * @note
**/
void CSL_SPI_SetMatchValue(SPI_HandleTypeDef* cspi, uint8_t match)
{
	if(cspi->Init.Match == SPI_MATCH_ENABLE)
	{
		cspi->Init.MatchValue = match;
		cspi->Instance->M = match;
	}
	else return;
}

/* Private Functions Definations of SPI */
/**
 * @brief 	Configure SPI
**/
static void SPI_Config(SPI_HandleTypeDef* cspi)
{
	/* write SPIx->C1 */
	cspi->Instance->C1 = cspi->Init.Mode | cspi->Init.CLKPolarity | cspi->Init.CLKPhase | cspi->Init.FirstBit;
	
	/* write SPIx->BR */
	cspi->Instance->BR = cspi->Init.PreScaler | cspi->Init.Scaler;
	
	/* configure NSS pin */
	//no NSS(SS pin is GPIO)
	if(cspi->Init.NSS == SPI_NSS_NONE)
	{
		CLEAR_BIT(cspi->Instance->C2, SPI_C2_MODFEN_MASK);
		CLEAR_BIT(cspi->Instance->C1, SPI_C1_SSOE_MASK);
	}
	//NSS is Mode Fault Input
	else if(cspi->Init.NSS == SPI_NSS_MFIN)
	{
		SET_BIT(cspi->Instance->C2, SPI_C2_MODFEN_MASK);
		CLEAR_BIT(cspi->Instance->C1, SPI_C1_SSOE_MASK);
	}
	//NSS is Chip Select Output
	else
	{
		SET_BIT(cspi->Instance->C2, SPI_C2_MODFEN_MASK);
		SET_BIT(cspi->Instance->C1, SPI_C1_SSOE_MASK);
	}
	
	/* configure SPI communication directions */
	if(cspi->Init.Direction != SPI_DIRECTION_2LINES)
	{
		SET_BIT(cspi->Instance->C2, SPI_C2_SPC0_MASK);
	}
	
	/* configure Match Value */
	if(cspi->Init.Match == SPI_MATCH_ENABLE)
	{
		cspi->Instance->M = cspi->Init.MatchValue;
	}
	else 
	{
		CLEAR_BIT(cspi->Instance->M, SPI_M_Bits_MASK);
	}
	
	/* Pin Configuration */
	if(cspi->Instance == SPI0)
	{
		assert_param(IS_SPI0_PINS(cspi->Init.Pins));
		SIM->PINSEL &= ~SIM_PINSEL_SPI0PS_MASK;
		SIM->PINSEL |= cspi->Init.Pins;
	}
	else
	{
		assert_param(IS_SPI1_PINS(cspi->Init.Pins));
		SIM->PINSEL1 &= ~SIM_PINSEL1_SPI1PS_MASK;
		SIM->PINSEL1 |= cspi->Init.Pins;
	}
}

/** 
 * @brief 	Wait for SPI flags until Timeout
**/
static CSL_StatusTypeDef SPI_WaitFlag(SPI_HandleTypeDef* cspi, uint8_t Flag, Flag_Status Status, uint32_t TickStart, uint32_t Timeout)
{
	while(__CSL_SPI_GET_FLAG(cspi, Flag) != Status)
	{
		//Check for Timeout
		if(Timeout != CSL_MAX_DELAY)
		{
			if((Timeout == 0u) || (CSL_GetTick() - TickStart > Timeout))
			{
				//Disable SPI interrupts
				__CSL_SPI_SPIE_DISBALE(cspi);
				__CSL_SPI_SPTIE_DISBALE(cspi);
				__CSL_SPI_SPMIE_DISBALE(cspi);
				
				//Reset the gState of SPI
				cspi->gState = CSL_SPI_STATE_READY;
				
				return CSL_Timeout;
			}
		}
	}
	
	return CSL_OK;
}

//EOF
