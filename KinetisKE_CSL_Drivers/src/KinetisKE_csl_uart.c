/**
 * Title 	UART module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_uart.h"

/** UART Private Functions Declaration(do not call these in user code) **/
static void UART_Config(UART_HandleTypeDef* cuart);
static uint16_t UART_BaudRate_Cal(uint32_t BaudRate);
static CSL_StatusTypeDef UART_WaitFlag(UART_HandleTypeDef* cuart, uint8_t Flag, Flag_Status Status, uint32_t TickStart, uint32_t Timeout);
static CSL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef* cuart);
static CSL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef* cuart);
static CSL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef* cuart);

/** UART Public Functions **/
/**
 * @brief	Initialize  UART
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_Init(UART_HandleTypeDef* cuart)
{
	//Error: UART Handle is NULL
	if(cuart == NULL)
	{
		goto ERR;
	}
	
	//paramter Check
	assert_param(IS_UART_UARTx(cuart->Instance));
	
	//User Init(Msp Init)
	 if(cuart->gState == CSL_UART_STATE_RESET)
	{
		__CSL_UNLOCK(cuart);
		CSL_UART_MspInit(cuart);
	}
	
	//UART Configure
	cuart->gState = CSL_UART_STATE_BUSY;
	UART_Config(cuart);
	
	//Set State Bit
	cuart->ErrorCode = CSL_UART_ERROR_NONE;
	cuart->gState = CSL_UART_STATE_READY;
	cuart->RxState = cuart->gState;
	
	return CSL_OK;
	
	//Error
ERR:
	cuart->gState = CSL_UART_STATE_ERROR;
	cuart->RxState = CSL_UART_STATE_RESET;
	cuart->ErrorCode = CSL_UART_ERROR_CONFIG;
	return CSL_Error;
}

/**
 * @brief	Reset UART
 * @param	
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_DeInit(UART_HandleTypeDef* cuart)
{
	//paramter Check
	assert_param(IS_UART_UARTx(cuart->Instance));
	
	//Reset UARTx Registers
	cuart->Instance->BDH = 0x00u;
	cuart->Instance->BDL = 0x00u;
	cuart->Instance->C1 = 0x00u;
	cuart->Instance->C2 = 0x00u;
	cuart->Instance->C3 = 0x00u;
	cuart->Instance->S2 = 0x00u;
	cuart->Instance->D = 0x00u;
	
	//Lock UART
	__CSL_LOCK(cuart);
	
	//Reset State & Error Code
	cuart->gState = CSL_UART_STATE_RESET;
	cuart->ErrorCode = CSL_UART_ERROR_NONE;
	
	//Msp Deinit
	CSL_UART_MspDeInit(cuart);
	
	return CSL_OK;
}

/**
 * @brief	Configure UART as Internal Loop or Half Duplex
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_HalfDuplex_Init(UART_HandleTypeDef* cuart)
{
	//Error: cuart is NULL or UART mode is 2-line
	if((cuart == NULL) || (cuart->Init.UART_Line == UART_Line_2Line))
	{
		goto ERR;
	}
	
	//Msp Init(User Init)
	if(cuart->gState == CSL_UART_STATE_RESET)
	{
		__CSL_UNLOCK(cuart);
		CSL_UART_MspInit(cuart);
	}
	
	//paramter Check
	assert_param(IS_UART_UARTx(cuart->Instance));
	
	//UART Configure
	cuart->gState = CSL_UART_STATE_BUSY;
	UART_Config(cuart);
	
	//Half-Duplex or Internal-Loop Configuration
	switch(cuart->Init.UART_Line)
	{
	case UART_Line_1Line:
		cuart->Instance->C1 |= UART_C1_LOOPS_MASK | UART_C1_RSRC_MASK;
		//Confirm UART Mode to Modify UARTx->C3[TXDIR]
		switch(cuart->Init.UART_Mode)
		{
			//Transmit Only
		case UART_Mode_Tx_Only:
			CSL_UART_HalfDuplex_TransmitterEnable(cuart);
			break;
			//Receive Only
		case UART_Mode_Rx_Only:
			CSL_UART_HalfDuplex_ReceiverEnable(cuart);
			break;
			//Transmit & Receive(Configure it manually)
		case UART_Mode_TxRx:
			__no_operation();
			break;
		default:
			goto ERR;
		}
		break;
	case UART_Line_Int_Loop:
		cuart->Instance->C1 |= UART_C1_LOOPS_MASK;
		CLEAR_BIT(cuart->Instance->C1, UART_C1_RSRC_MASK);
		break;
	default:
		goto ERR;
	}
	
	//Set State Bit
	cuart->ErrorCode = CSL_UART_ERROR_NONE;
	cuart->gState = CSL_UART_STATE_READY;
	cuart->RxState = cuart->gState;
	
	return CSL_OK;
	
	//ERR Return
ERR:
	cuart->gState = CSL_UART_STATE_ERROR;
	cuart->RxState = CSL_UART_STATE_RESET;
	cuart->ErrorCode = CSL_UART_ERROR_CONFIG;
	return CSL_Error;
}

/**
 * @brief	Configure UART as Multi-Processor Communication
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_MultiProcessorInit(UART_HandleTypeDef* cuart, uint8_t WakeUp)
{
	//Check
	if(cuart == NULL)
	{
		goto ERR;
	}
	
	//parameter check
	assert_param(IS_UART_UARTx(cuart->Instance));
	
	//User Init(Msp Init)
	if(cuart->gState == CSL_UART_STATE_RESET)
	{
		__CSL_UNLOCK(cuart);
		CSL_UART_MspInit(cuart);
	}
	
	//UART Configure
	cuart->gState = CSL_UART_STATE_BUSY;
	UART_Config(cuart);
	
	//UART Wakeup Methods Configure
	CLEAR_BIT(cuart->Instance->C1, UART_C1_WAKE_MASK);
	cuart->Instance->C1 |= UART_C1_WAKE_MASK;
	
	//Enable UART IDLE Detection
	__CSL_UART_ENABLE_IDLE(cuart);
	
	cuart->gState = CSL_UART_STATE_READY;
	cuart->RxState = CSL_UART_STATE_READY;
	cuart->ErrorCode = CSL_UART_ERROR_NONE;
	
	return CSL_OK;
	
	//Error
ERR:
	cuart->gState = CSL_UART_STATE_ERROR;
	cuart->RxState = CSL_UART_STATE_RESET;
	cuart->ErrorCode = CSL_UART_ERROR_CONFIG;
	return CSL_Error;
}

/**
 * @brief	User-defined Initialize UART function
 * @param
 * @return
 * @note
**/
__weak void CSL_UART_MspInit(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}

/**
 * @brief	User-defined RESET UART function
 * @param
 * @return
 * @note
**/
__weak void CSL_UART_MspDeInit(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}

/**
 * @brief	UART Transmit Function in Polling(blocking)
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_Transmit(UART_HandleTypeDef* cuart, uint8_t* TxBuffer, uint16_t Size, uint32_t Timeout)
{
	//Check 
	if((cuart == NULL) || (Timeout == 0))
		return CSL_Error;
	
	//Parameter Check
	assert_param(IS_UART_UARTx(cuart->Instance));
	
	uint32_t tickstart = 0;
	uint16_t* tmp = 0;
	
	if(cuart->gState == CSL_UART_STATE_READY)
	{
		//Process Locked
		__CSL_LOCK(cuart);
		
		cuart->pTxBuffer = TxBuffer;
		cuart->pTxXferCount = Size;
		cuart->pTxXferSize = Size;
		
		//Init TickStart for Timeout Management
		tickstart = CSL_GetTick();

		while(cuart->pTxXferCount > 0u)
		{
			cuart->pTxXferCount--;

			//Check Timeout
			if(UART_WaitFlag(cuart, UART_Tx_TDRE, SET, tickstart, Timeout) != CSL_OK)
			{
				return CSL_Timeout;
			}
			
			//word length is 9bit
			if(cuart->Init.WordLength == UART_Word_9B)
			{
				tmp = (uint16_t*)cuart->pTxBuffer;
				//Write Data to UARTx->C3[T8] & UARTx->D
				CLEAR_BIT(cuart->Instance->C3, UART_C3_T8_MASK);
				cuart->Instance->C3 = ((*tmp >> 1) & 0x80u) | cuart->Instance->C3;
				cuart->Instance->D = (uint8_t)(*tmp & (uint8_t)0xFF);
				if(cuart->Init.Parity == UART_Parity_None)
				{
					cuart->pTxBuffer += 2u;
				}
				else
				{
					cuart->pTxBuffer += 1u;
				}
			}
			//word length is 8bit
			else 
			{
				cuart->Instance->D = (uint8_t)(*cuart->pTxBuffer++ & (uint8_t)0xFF);
			}
		}

		//Wait Transmit Complete
		if(UART_WaitFlag(cuart, UART_Tx_TC, SET, tickstart, Timeout) != CSL_OK)
		{
			return CSL_Timeout;
		}

		//Process unlock
		__CSL_UNLOCK(cuart);

		cuart->gState = CSL_UART_STATE_READY;

		return CSL_OK;
	}
	else
	{
		return CSL_Busy;
	}
	
}

/**
 * @brief	UART Receive Function in Polling(blocking)
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_Receive(UART_HandleTypeDef* cuart, uint8_t* RxBuffer, uint16_t Size, uint32_t Timeout)
{
	//Check
	if((cuart == NULL) || (Timeout == 0))
	{
		return CSL_Error;
	}

	//Parameter Check
	assert_param(IS_UART_UARTx(cuart->Instance));

	uint32_t tickstart = 0;
	uint16_t* tmp = 0;
	uint8_t data = 0;

	if(cuart->RxState == CSL_UART_STATE_READY)
	{
		cuart->pRxXferCount = Size;
		cuart->pRxXferSize = Size;

		//Process Lock
		__CSL_LOCK(cuart);

		cuart->ErrorCode = CSL_UART_ERROR_NONE;
		cuart->RxState = CSL_UART_STATE_BUSY_RX;

		//Init TickStart for Timeout Management
		tickstart = CSL_GetTick();

		while(cuart->pRxXferCount > 0u)
		{
			cuart->pRxXferCount--;

			//Clear RDRF Flag
			__CSL_UART_FLUSH_S1(cuart);

			//Check Timeout
			if(UART_WaitFlag(cuart, UART_Rx_RDRF, SET, tickstart, Timeout) != CSL_OK)
			{
				return CSL_Timeout;
			}

			//Word length is 9 bit
			if(cuart->Init.WordLength == UART_Word_9B)
			{
				tmp = (uint16_t*)RxBuffer;
				data = cuart->Instance->D;
				if(cuart->Init.Parity == UART_Parity_None)
				{
					*tmp = (uint16_t)(((cuart->Instance->C3 & 0x80u << 1u) | data) & 0x01FFu);
					RxBuffer += 2u;
				}
				else
				{
					*tmp = (uint16_t)(cuart->Instance->D & 0x00FFu);
					RxBuffer += 1u;
				}
			}
			//Word length is 8 bit
			else
			{
				if(cuart->Init.Parity == UART_Parity_None)
				{
					*RxBuffer++ = (uint8_t)(cuart->Instance->D & 0x00FFu);
				}
				else
				{
					*RxBuffer++ = (uint8_t)(cuart->Instance->D & 0x007Fu);
				}
			}
		}

		//Process unlock 
		__CSL_UNLOCK(cuart);

		cuart->RxState = CSL_UART_STATE_READY;

		return CSL_OK;
	}
	else 
	{
		return CSL_Busy;
	}
}

/**
 * @brief	UART Transmit Function in IT(no-blocking)
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_Transmit_IT(UART_HandleTypeDef* cuart, uint8_t* TxBuffer, uint16_t Size)
{
	////Parameter Check
	if((cuart == NULL) || (TxBuffer == NULL) || (Size == 0))
	{
		return CSL_Error;
	}
	
	//in Mode Transmitter IT
	if(cuart->gState == CSL_UART_STATE_READY)
	{
		cuart->gState = CSL_UART_STATE_BUSY_TX;
		cuart->ErrorCode = CSL_UART_ERROR_NONE;
		
		//Process Lock
		__CSL_LOCK(cuart);
		
		cuart->pTxBuffer = TxBuffer;
		cuart->pTxXferCount = Size;
		cuart->pTxXferSize = Size;
		
		//Process Unlock
		__CSL_UNLOCK(cuart);
		
		//Enable UART Transmit data register empty Interrupt
		SET_BIT(cuart->Instance->C2, UART_C2_TIE_MASK);
		
		return CSL_OK;
	}
	else 
	{
		return CSL_Busy;
	}
}

/**
 * @brief	UART Receive Function in IT(no-blocking)
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_Receive_IT(UART_HandleTypeDef* cuart, uint8_t* RxBuffer, uint16_t Size)
{
	//Parameter Check
	if((cuart == NULL) || (RxBuffer == NULL) || (Size == 0))
	{
		return CSL_Error;
	}
	
	//in Mode Receier IT
	if(cuart->RxState == CSL_UART_STATE_READY)
	{
		cuart->RxState = CSL_UART_STATE_BUSY_RX;
		cuart->ErrorCode = CSL_UART_ERROR_NONE;
		
		//Process Lock
		__CSL_LOCK(cuart);
		
		cuart->pRxBuffer = RxBuffer;
		cuart->pRxXferCount = Size;
		cuart->pRxXferSize = Size;
		
		//Process Unlock
		__CSL_UNLOCK(cuart);
		
		//Enable Receive Interrupt(RIE, ILIE)
		SET_BIT(cuart->Instance->C2, UART_C2_RIE_MASK | UART_C2_ILIE_MASK);
		
		/* Note: LIN Break Point and Rx Pin Edge Detection ITs need to configured by User */
		
		//Enable Error Interrupt(ORIE, NEIE, FEIE, PEIE)
		SET_BIT(cuart->Instance->C3, 0x0Fu);
		
		return CSL_OK;
	}
	else
	{
		return CSL_Busy;
	}
}

/**
 * @brief   CSL UART Abort operations(called by UARTx_IRQHandler())
 * @param   UART_HandleTypeDef* cuart
                UART Handle
 * @return  None
 * @note    None
**/
void CSL_UART_Abort(UART_HandleTypeDef* cuart)
{
	//Disable all of Interrupts
	CLEAR_BIT(cuart->Instance->BDH, 0x60u);
	CLEAR_BIT(cuart->Instance->C2, 0xF0u);
	CLEAR_BIT(cuart->Instance->C3, 0x0Fu);
	
	//Reset Tx Rx Buffer
	cuart->pTxBuffer = 0;
	cuart->pTxXferCount = 0u;
	cuart->pTxXferSize = 0u;
	cuart->pRxBuffer = 0;
	cuart->pRxXferCount = 0u;
	cuart->pRxXferSize = 0u;
	
	//Reset Error Code
	cuart->ErrorCode = CSL_UART_ERROR_NONE;
	
	//Reset Status
	cuart->gState = CSL_UART_STATE_READY;
	cuart->RxState = CSL_UART_STATE_READY;
	
	//User Callback
	CSL_UART_AbortCallback(cuart);
}

/**
 * @brief   CSL UART Abortion Callback(called by UARTx_IRQHandler())
 * @param   UART_HandleTypeDef* cuart
                UART Handle
 * @return  None
 * @note    None
**/
__weak void CSL_UART_AbortCallback(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}

/**
 * @brief   CSL UART IRQHandler(called by UARTx_IRQHandler())
 * @param   UART_HandleTypeDef* cuart
                UART Handle
 * @return  None
 * @note    None
**/
void CSL_UART_IRQHandler(UART_HandleTypeDef* cuart)
{
	uint8_t uart_s1 = 0x00u, uart_s2 = 0x00u, bdhits = 0x00u, c2its = 0x00u;
	uint8_t errorflags = 0x00u;
	
	//if no errors
	errorflags = cuart->Instance->S1 & 0x0Fu;
	if(errorflags == RESET)
	{
		//Rx Pin Edge Interrupt 
		uart_s2 = cuart->Instance->BDH & UART_BDH_RXEDGIE_MASK;
		bdhits = cuart->Instance->S2 & UART_S2_RXEDGIF_MASK;
		if((uart_s2 & bdhits) != RESET)
		{
			//Clear Flags
			cuart->Instance->S2 |= UART_S2_RXEDGIF_MASK;
			//Disable Rx Edge Interrupt
			cuart->Instance->BDH &= ~UART_BDH_RXEDGIE_MASK;
			//User Callback
			CSL_UART_RxLevelCallback(cuart);
			return;
		}
		
		//UART in mode Receiver
		uart_s2 = cuart->Instance->S2 & UART_S2_LBKDIF_MASK >> UART_S2_LBKDIF_SHIFT;
		bdhits = cuart->Instance->BDH & UART_BDH_LBKDIE_MASK >> UART_BDH_LBKDIE_SHIFT;
		uart_s1 = cuart->Instance->S1 & UART_S1_RDRF_MASK >> UART_S1_RDRF_SHIFT;
		c2its = cuart->Instance->C2 & UART_C2_RIE_MASK >> UART_C2_RIE_SHIFT;
		if(((uart_s2 & bdhits) & (uart_s1 & c2its)) != RESET)
		{
			UART_Receive_IT(cuart);
			return;
		}
		
		//UART IDLE Interrupt
		bdhits = cuart->Instance->C2 & UART_C2_ILIE_MASK;
		uart_s1 = cuart->Instance->C1 & UART_S1_IDLE_MASK;
		if((bdhits & uart_s1) != RESET)
		{
			//Clear IDLE Flag
			__CSL_UART_FLUSH_S1(cuart);
			__CSL_UART_FLUSH_DR(cuart);
			//IDLE User Callback
			CSL_UART_IDLECallback(cuart);
			/** Disable Interrupt or not depends on user **/
		}
		
	}
	
	//error occured
	if(errorflags != RESET)
	{
		//Error Overflow
		if((cuart->Instance->S1 & UART_Err_OR) != RESET)
		{
			cuart->ErrorCode = CSL_UART_ERROR_ORE;
		}
		
		//Error Noise
		if((cuart->Instance->S1 & UART_Err_NF) != RESET)
		{
			cuart->ErrorCode = CSL_UART_ERROR_NE;
		}
		
		//Error Frame-error
		if((cuart->Instance->S1 & UART_Err_FE) != RESET)
		{
			cuart->ErrorCode = CSL_UART_ERROR_FE;
		}
		
		//Error Parity
		if((cuart->Instance->S1 & UART_Err_PF) != RESET)
		{
			cuart->ErrorCode = CSL_UART_ERROR_PE;
		}
		
		//Clear the Error Flags
		__CSL_UART_FLUSH_S1(cuart);
		__CSL_UART_FLUSH_DR(cuart);
		
		//Disable Error Interrupt
		CLEAR_BIT(cuart->Instance->C3, 0x0Fu);
		
		//Reset UART Handle
		cuart->gState = CSL_UART_STATE_READY;
		cuart->RxState = CSL_UART_STATE_READY;
		
		//Error Callback Funtion
		CSL_UART_ErrorCallback(cuart);
		
		//Clear Error Code
		cuart->ErrorCode = CSL_UART_ERROR_NONE;
		
		//exit
		return;
	}
	
	//UART in mode Transmitter
	uart_s1 = cuart->Instance->S1 & UART_S1_TDRE_MASK >> UART_S1_TDRE_SHIFT;
	c2its = cuart->Instance->C2 & UART_C2_TIE_MASK>> UART_C2_TIE_SHIFT;
	if((uart_s1 & c2its) != RESET)
	{
		UART_Transmit_IT(cuart);
		return;
	}
	
	//UART in mode Transmitter End
	uart_s1 = cuart->Instance->S1 & UART_S1_TC_MASK >> UART_S1_TC_SHIFT;
	c2its = cuart->Instance->C2 & UART_C2_TCIE_MASK >> UART_C2_TCIE_SHIFT;
	if((uart_s1 & c2its) != RESET)
	{
		UART_EndTransmit_IT(cuart);
		return;
	}
}

/**
 * @brief	Trasmit Complete Callback
 * @param
 * @return
 * @note
**/
__weak void CSL_UART_TxCpltCallback(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}

/**
 * @brief	Receive Complete Callback
 * @param
 * @return
 * @note
**/
__weak void CSL_UART_RxCpltCallback(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}

/**
 * @brief 	Rx Pin voltage level Detection Callback
 * @param
 * @return 
 * @note
**/
__weak void CSL_UART_RxLevelCallback(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}

/**
 * @brief	UART Error Callback
 * @param
 * @return
 * @note
**/
__weak void CSL_UART_ErrorCallback(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}


/**
 * @brief	UART IDLE Callback
 * @param
 * @return
 * @note
**/
__weak void CSL_UART_IDLECallback(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}

/**
 * @brief	UART Error Callback
 * @param
 * @return
 * @note
**/
__weak void CSL_UART_LIN_BKPCallback(UART_HandleTypeDef* cuart)
{
	UNUSED(cuart);
}

/**
 * @brief	LIN Break Point Detection Callback
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_TransmitBKP(UART_HandleTypeDef* cuart)
{
    //Lock UART & start operations
    __CSL_LOCK(cuart);
    
    //Transmit LIN Break Point
    SET_BIT(cuart->Instance->C2, UART_C2_SBK_MASK);
    CLEAR_BIT(cuart->Instance->C2, UART_C2_SBK_MASK);
    
    //Unlock & end operations
    __CSL_UNLOCK(cuart);
    
    cuart->gState = CSL_UART_STATE_READY;
    return CSL_OK;
}

/**
 * @brief	Enable Transmitter in Half-Duplex Mode
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_HalfDuplex_TransmitterEnable(UART_HandleTypeDef* cuart)
{
	//confirm the UART status
	if(cuart->gState != CSL_UART_STATE_READY)
	{
		cuart->gState = CSL_UART_STATE_BUSY;
		return CSL_Busy;
	}
	
	//Enable the Transmitter
	__CSL_LOCK(cuart);
	
	//Tx Pin Direction is OUT
	cuart->Instance->C3 |= UART_C3_TXDIR_MASK;
	//Transmit Enable
	cuart->Instance->C2 |= UART_C2_TE_MASK;
	//Receive Disable
	cuart->Instance->C2 &= ~UART_C2_RE_MASK;
	
	__CSL_UNLOCK(cuart);
	
	cuart->gState = CSL_UART_STATE_READY;
	cuart->RxState = CSL_UART_STATE_RESET;
	cuart->ErrorCode = CSL_UART_ERROR_NONE;
	return CSL_OK;
}

/**
 * @brief	Enable Receiver in Half-Duplex Mode
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_UART_HalfDuplex_ReceiverEnable(UART_HandleTypeDef* cuart)
{
	if(cuart->RxState != CSL_UART_STATE_READY || cuart->gState != CSL_UART_STATE_READY)
	{
		cuart->gState = CSL_UART_STATE_BUSY;
		cuart->RxState = CSL_UART_STATE_BUSY_RX;
		return CSL_Busy;
	}
	
	//Enable Receiver
	__CSL_LOCK(cuart);
	
	//Tx Pin Direction is IN
	cuart->Instance->C3 &= ~UART_C3_TXDIR_MASK;
	//Receive Enable
	cuart->Instance->C2 |= UART_C2_RE_MASK;
	//Transmit Disable
	cuart->Instance->C2 &= ~UART_C2_TE_MASK;
	
	__CSL_UNLOCK(cuart);
	
	cuart->gState = CSL_UART_STATE_READY;
	cuart->RxState = CSL_UART_STATE_READY;
	cuart->ErrorCode = CSL_UART_ERROR_NONE;
	return CSL_OK;
}

/**
 * @brief	Get UART Status
 * @param
 * @return
 * @note
**/
CSL_UART_StateTypeDef CSL_UART_GetState(UART_HandleTypeDef* cuart)
{
	return cuart->gState;
}

/**
 * @brief	Get UART ErrorCode
 * @param
 * @return
 * @note
**/
uint32_t CSL_UART_GetError(UART_HandleTypeDef* cuart)
{
	return cuart->ErrorCode;
}

/** UART Private Functions(do not call these in user code) **/
/**
 * @brief	Configure UART with UART_Handle
**/
static void UART_Config(UART_HandleTypeDef* cuart)
{
	/** Paramters Check **/
	assert_param(IS_UART_STOPBIT(cuart->Init.StopBits));
	assert_param(IS_UART_WordLength(cuart->Init.WordLength));
	assert_param(IS_UART_Parity(cuart->Init.Parity));
	
	/* UART_Pins */
	if(cuart->Instance == UART0)
	{
		assert_param(IS_UART0_Pins(cuart->Init.UART_Pins));
		SIM->PINSEL &= ~SIM_PINSEL_UART0PS_MASK;
		SIM->PINSEL |= cuart->Init.UART_Pins;
	}
	else if(cuart->Instance == UART1)
	{
		assert_param(IS_UART1_Pins(cuart->Init.UART_Pins));
		SIM->PINSEL1 &= ~SIM_PINSEL1_UART1PS_MASK;
		SIM->PINSEL1 |= cuart->Init.UART_Pins;
	}
	else if(cuart->Instance == UART2)
	{
		assert_param(IS_UART2_Pins(cuart->Init.UART_Pins));
		SIM->PINSEL1 &= ~SIM_PINSEL1_UART2PS_MASK;
		SIM->PINSEL1 |= cuart->Init.UART_Pins;
	}
	else return;
	
	/* Disable Tx & Rx */
	CLEAR_BIT(cuart->Instance->C2, UART_C2_TE_MASK | UART_C2_RE_MASK);
	
	/** Write in UARTx Registers **/
	uint16_t baudrate = UART_BaudRate_Cal(cuart->Init.BuadRate);
	cuart->Instance->BDH &= ~UART_BDH_SBR_MASK;
	cuart->Instance->BDH = cuart->Init.StopBits | (baudrate >> 8);
	cuart->Instance->BDL = (uint8_t)(baudrate & 0xFFu);
	cuart->Instance->C1 = cuart->Init.UART_Line | cuart->Init.WordLength | cuart->Init.Parity;
	cuart->Instance->C2 = cuart->Init.UART_Mode;
}

/** 
 * @brief	Calculate the BaudRate Pre-Scaler
**/
static uint16_t UART_BaudRate_Cal(uint32_t BaudRate)
{
	uint16_t SBR = ((SystemBusClock >> 4) * 10 / BaudRate + 5) / 10;
	return ((uint16_t)SBR & 0x1FFFu);
}
	
/**
 * @brief	Wait UART Flags Until Timeout(only for UARTx->S1)
**/
static CSL_StatusTypeDef UART_WaitFlag(UART_HandleTypeDef* cuart, uint8_t Flag, Flag_Status Status, uint32_t TickStart, uint32_t Timeout)
{
	while(__CSL_UART_GET_FLAG(cuart->Instance->S1, Flag) != Status)
	{
		//Check for Timeout
		if(Timeout != CSL_MAX_DELAY)
		{
			if((Timeout == 0u) || (CSL_GetTick() - TickStart > Timeout))
			{
				//Disable Interrupt(TDRE, TCIE, RIE, ILIE)
				CLEAR_BIT(cuart->Instance->C2, 0xF0u);
				
				//Disbale Interrupt(ORIE, NEIE, FEIE, PEIE)
				CLEAR_BIT(cuart->Instance->C3, 0x0Fu);
				
				cuart->gState = CSL_UART_STATE_READY;
				cuart->RxState = CSL_UART_STATE_READY;
				
				return CSL_Timeout;
			}
		}
	}
	return CSL_OK;
}

/**
 * @brief	Transmit in IT
**/
static CSL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef* cuart)
{
	uint16_t* tmp = 0;
	
	/** Clear TDRE Flag --> Read UARTx->S1, Write UARTx->D */
	
	//Check Transmit Process is ongoing or not
	if(cuart->gState == CSL_UART_STATE_BUSY_TX)
	{
		//Read UARTx->S1
		__CSL_UART_FLUSH_S1(cuart);
		
		//Word length is 9 bit
		if(cuart->Init.WordLength == UART_Word_9B)
		{
			tmp = (uint16_t*)cuart->pTxBuffer;
			//Write Data to UARTx->C3[T8] & UARTx->D
			CLEAR_BIT(cuart->Instance->C3, UART_C3_T8_MASK);
			cuart->Instance->C3 = ((*tmp >> 1) & 0x80u) | cuart->Instance->C3;
			cuart->Instance->D = (uint8_t)(*tmp & (uint8_t)0xFF);
			if(cuart->Init.Parity == UART_Parity_None)
			{
				cuart->pTxBuffer += 2u;
			}
			else
			{
				cuart->pTxBuffer += 1u;
			}
		}
		
		//Word length is 8 bit
		else
		{
			cuart->Instance->D = (uint8_t)(*cuart->pTxBuffer++ & (uint8_t)0xFF);
		}
		
		//Check Transmit is Completed or not
		if(--cuart->pRxXferCount == 0u)
		{
			//Disbale the Transmit Cache Empty Interrupt
			CLEAR_BIT(cuart->Instance->C2, UART_C2_TIE_MASK);
			
			//Enable the Transmit Complete Interrupt
			SET_BIT(cuart->Instance->C2, UART_C2_TCIE_MASK);
		}
		
		return CSL_OK;
	}
	else return CSL_Busy;
}

/**
 * @brief 	Transmit End in IT
**/
CSL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef* cuart)
{
	//Disable Transmit Complete Interrupt
	CLEAR_BIT(cuart->Instance->C2, UART_C2_TCIE_MASK);
	
	//Transmit Process is ended
	cuart->gState = CSL_UART_STATE_READY;
	
	//Transmit Complete Callback
	CSL_UART_TxCpltCallback(cuart);
	
	return CSL_OK;
}

/**
 * @brief	Receive in IT
**/
static CSL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef* cuart)
{
	//LIN Break Point Detection Enabled
	if((cuart->Instance->S2 & 0x02) != 0)
	{
		//clear LINBKP Flag
		CLEAR_BIT(cuart->Instance->S2, UART_S2_LBKDIF_MASK);
		
		//Disable LINBKP Interrupt
		CLEAR_BIT(cuart->Instance->BDH, UART_BDH_LBKDIE_MASK);
		
		//Break Point Detection Callback
		CSL_UART_LIN_BKPCallback(cuart);
		
		return CSL_OK;
	}
	
	//Receiver
	if(cuart->RxState == CSL_UART_STATE_BUSY_RX)
	{
		uint16_t* tmp = 0;
		uint8_t data = 0;
		
		//Clear RDRF Flag
		__CSL_UART_FLUSH_S1(cuart);
		
		//Word length is 9 bit
		if(cuart->Init.WordLength == UART_Word_9B)
		{
			tmp = (uint16_t*)cuart->pRxBuffer;
			data = cuart->Instance->D;
			if(cuart->Init.Parity == UART_Parity_None)
			{
				*tmp = (uint16_t)(((cuart->Instance->C3 & 0x80u << 1u) | data) & 0x01FFu);
				cuart->pRxBuffer += 2u;
			}
			else
			{
				*tmp = (uint16_t)(cuart->Instance->D & 0x00FFu);
				cuart->pRxBuffer += 1u;
			}
		}
		//Word length is 8 bit
		else
		{
			if(cuart->Init.Parity == UART_Parity_None)
			{
				*cuart->pRxBuffer++ = (uint8_t)(cuart->Instance->D & 0x00FFu);
			}
			else
			{
				*cuart->pRxBuffer++ = (uint8_t)(cuart->Instance->D & 0x007Fu);
			}
		}
		
		if(--cuart->pRxXferCount == 0u)
		{
			//Disable Receive Interrupt(RIE, ILIE)
			CLEAR_BIT(cuart->Instance->C2, UART_C2_RIE_MASK | UART_C2_ILIE_MASK);
			
			//Disable Error Interrupt(ORIE, NEIE, FEIE, PEIE)
			CLEAR_BIT(cuart->Instance->C3, 0x0Fu);
			
			//User Callback
			CSL_UART_RxCpltCallback(cuart);
			
			cuart->RxState = CSL_UART_STATE_READY;
			
			return CSL_OK;
		}
	}
	
	return CSL_OK;
}

//EOF
