/**
 * Title 	UART module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_UART_H
#define __KinetisKE_CSL_UART_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl.h"

/** 
  * @brief CSL UART State structures definition  
  * @note  CSL UART State value is a combination of 2 different substates: gState and RxState.
  *        - gState contains UART state information related to global Handle management 
  *          and also information related to Tx operations.
  *          gState value coding follow below described bitmap :
  *          b7-b6  Error information 
  *             00 : No Error
  *             01 : (Not Used)
  *             10 : Timeout
  *             11 : Error
  *          b5     IP initilisation status
  *             0  : Reset (IP not initialized)
  *             1  : Init done (IP not initialized. HAL UART Init function already called)
  *          b4-b3  (not used)
  *             xx : Should be set to 00
  *          b2     Intrinsic process state
  *             0  : Ready
  *             1  : Busy (IP busy with some configuration or internal operations)
  *          b1     (not used)
  *             x  : Should be set to 0
  *          b0     Tx state
  *             0  : Ready (no Tx operation ongoing)
  *             1  : Busy (Tx operation ongoing)
  *        - RxState contains information related to Rx operations.
  *          RxState value coding follow below described bitmap :
  *          b7-b6  (not used)
  *             xx : Should be set to 00
  *          b5     IP initilisation status
  *             0  : Reset (IP not initialized)
  *             1  : Init done (IP not initialized)
  *          b4-b2  (not used)
  *            xxx : Should be set to 000
  *          b1     Rx state
  *             0  : Ready (no Rx operation ongoing)
  *             1  : Busy (Rx operation ongoing)
  *          b0     (not used)
  *             x  : Should be set to 0.
***/ 
typedef enum
{
    CSL_UART_STATE_RESET             = 0x00U,    /*!< Peripheral is not yet Initialized 
                                                   Value is allowed for gState and RxState */
    CSL_UART_STATE_READY             = 0x20U,    /*!< Peripheral Initialized and ready for use
                                                   Value is allowed for gState and RxState */
    CSL_UART_STATE_BUSY              = 0x24U,    /*!< an internal process is ongoing
                                                   Value is allowed for gState only */
    CSL_UART_STATE_BUSY_TX           = 0x21U,    /*!< Data Transmission process is ongoing 
                                                   Value is allowed for gState only */
    CSL_UART_STATE_BUSY_RX           = 0x22U,    /*!< Data Reception process is ongoing
                                                   Value is allowed for RxState only */
    CSL_UART_STATE_BUSY_TX_RX        = 0x23U,    /*!< Data Transmission and Reception process is ongoing 
                                                   Not to be used for neither gState nor RxState.
                                                   Value is result of combination (Or) between gState and RxState values */
    CSL_UART_STATE_TIMEOUT           = 0xA0U,    /*!< Timeout state    
                                                   Value is allowed for gState only */
    CSL_UART_STATE_ERROR             = 0xE0U     /*!< Error   
                                                   Value is allowed for gState only */
}CSL_UART_StateTypeDef;

/**
 * UART Initialization Structure
**/
typedef struct
{
	uint32_t			UART_Pins;					//UART Pins Alternative
	uint8_t				StopBits;					//UART Stop Bits
	uint32_t 			BuadRate;					//UART BaudRate
	uint8_t 			UART_Line;					//UART Lines(1/2/loop)
	uint8_t 			WordLength;					//Word Length(8bit or 9bit), Include Pariting Bits
	uint8_t 			UART_Mode;					//Work Mode
	uint8_t 			Parity;						//Parity Check
}UART_InitTypeDef;

/**
 * UART Handle Structure
**/
typedef struct
{
	UART_Type*						Instance;		//Instance of UART
	UART_InitTypeDef				Init;			//Initialize Structure
	uint8_t* 						pTxBuffer;		//Transmit Buffer
	uint16_t						pTxXferSize;	//Size of Tx Buffer
	__IO uint16_t 					pTxXferCount;	//Counter of Tx Buffer
	uint8_t* 						pRxBuffer;		//Receive Buffer
	uint16_t						pRxXferSize;	//Size of Rx Buffer
	__IO uint16_t 					pRxXferCount;	//Counter of Rx Buffer
	__IO CSL_LockTypeDef			Lock;			//Process Lock
	__IO CSL_UART_StateTypeDef 		gState;			//Global State
	__IO CSL_UART_StateTypeDef		RxState;		//Receiver State
	__IO uint32_t					ErrorCode;		//Error Code 
}UART_HandleTypeDef;

/**
 * UART Pins(SIM->PINSELx)
**/																	//Rx   & Tx
#define UART0_Pin_Group0			((uint32_t)(0x00000000u))		//PTB0 & PTB1
#define UART0_Pin_Group1			SIM_PINSEL_UART0PS_MASK			//PTA2 & PTA3
#define UART1_Pin_Group0			((uint32_t)(0x00000000u))		//PTC6 & PTC7
#define UART1_Pin_Group1			SIM_PINSEL1_UART1PS_MASK		//PTF2 & PTF3
#define UART2_Pin_Group0			((uint32_t)(0x00000000u))		//PTD6 & PTD7
#define UART2_Pin_Group1			SIM_PINSEL1_UART2PS_MASK		//PTI0 & PTI1	

/**
 * UART Stop Bits(UARTx->BDH)
**/
#define UART_STOP_1BIT				0x00u
#define UART_STOP_2BIT				0x20u

/**
 * UART Line Mode(UARTx->C1)
**/
#define UART_Line_2Line				0x00u
#define UART_Line_1Line				0xA0u
#define UART_Line_Int_Loop			0x80u

/** 
 * UART Word Length(UARTx->C1)
**/
#define UART_Word_8B				0x00u
#define UART_Word_9B				0x10u

/**
 * UART Work Mode(UARTx->C2)
**/
#define UART_Mode_Tx_Only			0x08u
#define UART_Mode_Rx_Only			0x04u
#define UART_Mode_TxRx				0x0Cu

/**
 * Parity Check(UARTx->C1)
**/
#define UART_Parity_None			0x00u
#define UART_Parity_Even			0x02u
#define UART_Parity_Odd				0x03u

/**
 * UART Wake-up Methods(UARTx->C1)
**/
#define UART_Wakeup_Wire			0x00u
#define UART_Wakeup_Address			0x08u

/**
 * UART Interrupt Flags Bits
**/
//UARTx->S1
#define UART_Tx_TDRE				UART_S1_TDRE_MASK			    //Transmit Data Register is Empty
#define UART_Tx_TC					UART_S1_TC_MASK     			//Transmit Complete
#define UART_Rx_RDRF				UART_S1_RDRF_MASK   			//Receive Data Register
#define UART_Rx_IDLE				UART_S1_IDLE_MASK			    //Receive IDLE
#define UART_Err_OR					UART_S1_OR_MASK			        //Error --> Overflow
#define UART_Err_NF					UART_S1_NF_MASK			        //Error --> Noise
#define UART_Err_FE					UART_S1_FE_MASK			        //Error --> Frame Error
#define UART_Err_PF					UART_S1_PF_MASK			        //Error --> Parity Error
//UARTx->S2
#define UART_Rx_LBK					UART_S2_LBKDIF_MASK 			//Receive LIN Break Point Checking
#define UART_Rx_Edge				UART_S2_RXEDGIF_MASK			//Rx Pin Edge Detection

/**
 * Error Codes of UART
**/
#define CSL_UART_ERROR_NONE         0x00000000U   /*!< No error            */
#define CSL_UART_ERROR_PE           0x00000001U   /*!< Parity error        */
#define CSL_UART_ERROR_NE           0x00000002U   /*!< Noise error         */
#define CSL_UART_ERROR_FE           0x00000004U   /*!< Frame error         */
#define CSL_UART_ERROR_ORE          0x00000008U   /*!< Overrun error       */
#define CSL_UART_ERROR_CONFIG		0x00000010U   /*!< Configure Failed	   */

/** Macros Functions **/
/**
 * @brief 	Enable/Disable LIN Break Point Detection ENable
**/
#define __CSL_UART_LIN_BKDE_ENABLE(__HANDLE__)			(SET_BIT((__HANDLE__)->Instance->S2, UART_S2_LBKDE_MASK))
#define __CSL_UART_LIN_BKDE_DISABLE(__HANDLE__)			(CLEAR_BIT((__HANDLE__)->Instance->S2, UART_S2_LBKDE_MASK))

/**
 * @brief	Enable/Disable Idle Detection
**/
#define __CSL_UART_ENABLE_IDLE(__HANDLE__)				(SET_BIT((__HANDLE__)->Instance->S2, UART_S2_RWUID_MASK))
#define __CSL_UART_DISABLE_IDLE(__HANDLE__)				(CLEAR_BIT((__HANDLE__)->Instance->S2, UART_S2_PWUID_MASK))

/**
 * @brief	    Clear Interrupt Flags(only for UARTx->S2)
**/
#define __CSL_UART_CLEAR_LBKDIF(__HANDLE__)				(CLEAR_BIT((__HANDLE__)->Instance->S2, UART_Rx_LBK))
#define __CSL_UART_CLEAR_RXEDGIF(__HANDLE__)			(CLEAR_BIT((__HANDLE__)->Instance->S2, UART_Rx_Edge))

/**
 * @brief 	Flush the DR
**/
#define __CSL_UART_FLUSH_DR(__HANDLE__)					(UNUSED((__HANDLE__)->Instance->D))

/**
 * @brief 	Flush the UARTx->S1
**/
#define __CSL_UART_FLUSH_S1(__HANDLE__)					(UNUSED((__HANDLE__)->Instance->S1))

/**
 * @brief	    Get UART Interrupt Bits
**/
#define __CSL_UART_GET_IT(__REG__, __IT__)			((__REG__ & (__IT__)) ? SET : RESET)

/**
 * @brief	    Get UART Status Flags
**/
#define __CSL_UART_GET_FLAG(__REG__, __FLAG__)			((__REG__ & (__FLAG__)) ? SET : RESET)


/** UART Public Fuctions **/
CSL_StatusTypeDef CSL_UART_Init(UART_HandleTypeDef* cuart);
CSL_StatusTypeDef CSL_UART_DeInit(UART_HandleTypeDef* cuart);
CSL_StatusTypeDef CSL_UART_HalfDuplex_Init(UART_HandleTypeDef* cuart);
CSL_StatusTypeDef CSL_UART_MultiProcessorInit(UART_HandleTypeDef* cuart, uint8_t WakeUp);
void CSL_UART_MspInit(UART_HandleTypeDef* cuart);
void CSL_UART_MspDeInit(UART_HandleTypeDef* cuart);

//Functions in Polling
CSL_StatusTypeDef CSL_UART_Transmit(UART_HandleTypeDef* cuart, uint8_t* TxBuffer, uint16_t Size, uint32_t Timeout);
CSL_StatusTypeDef CSL_UART_Receive(UART_HandleTypeDef* cuart, uint8_t* RxBuffer, uint16_t Size, uint32_t Timeout);

//Fucntions in IT
CSL_StatusTypeDef CSL_UART_Transmit_IT(UART_HandleTypeDef* cuart, uint8_t* TxBuffer, uint16_t Size);
CSL_StatusTypeDef CSL_UART_Receive_IT(UART_HandleTypeDef* cuart, uint8_t* RxBuffer, uint16_t Size);

//UART Abortion
void CSL_UART_Abort(UART_HandleTypeDef* cuart);
void CSL_UART_AbortCallback(UART_HandleTypeDef* cuart);

//Interrupt Service Requestion Callbacks
void CSL_UART_IRQHandler(UART_HandleTypeDef* cuart);
void CSL_UART_TxCpltCallback(UART_HandleTypeDef* cuart);
void CSL_UART_RxCpltCallback(UART_HandleTypeDef* cuart);
void CSL_UART_RxLevelCallback(UART_HandleTypeDef* cuart);
void CSL_UART_ErrorCallback(UART_HandleTypeDef* cuart);
void CSL_UART_IDLECallback(UART_HandleTypeDef* cuart);
void CSL_UART_LIN_BKPCallback(UART_HandleTypeDef* cuart);

//Device Control Functions
CSL_StatusTypeDef CSL_UART_TransmitBKP(UART_HandleTypeDef* cuart);
CSL_StatusTypeDef CSL_UART_HalfDuplex_TransmitterEnable(UART_HandleTypeDef* cuart);
CSL_StatusTypeDef CSL_UART_HalfDuplex_ReceiverEnable(UART_HandleTypeDef* cuart);
CSL_UART_StateTypeDef CSL_UART_GetState(UART_HandleTypeDef* cuart);
uint32_t CSL_UART_GetError(UART_HandleTypeDef* cuart);

/* Defgroup UART_Private_Macros CORTEX Private Macros */
#define IS_UART_UARTx(Instance)		((Instance == UART0) || (Instance == UART1) || (Instance == UART2))
#define IS_UART0_Pins(UART_Pin)		((UART_Pin == UART0_Pin_Group0) || (UART_Pin == UART0_Pin_Group1))
#define IS_UART1_Pins(UART_Pin)		((UART_Pin == UART1_Pin_Group0) || (UART_Pin == UART1_Pin_Group1))
#define IS_UART2_Pins(UART_Pin)		((UART_Pin == UART2_Pin_Group0) || (UART_Pin == UART2_Pin_Group1))
#define IS_UART_STOPBIT(bits)		((bits == UART_STOP_1BIT) || (bits == UART_STOP_2BIT))
#define IS_UART_WordLength(len)		((len == UART_Word_8B) || (len == UART_Word_9B))
#define IS_UART_Parity(parity)		((parity == 0x00u) || (parity == 0x02u) || (parity == 0x03u))
#define IS_UART_Wakeup(x)			((x == 0x00u) || (x == 0x08u))

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_UART_H*/

//EOF
