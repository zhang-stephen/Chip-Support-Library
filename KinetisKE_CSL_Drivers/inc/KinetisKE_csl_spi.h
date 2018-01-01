/**
 * Title 	SPI module in CSL for KEAZxxx(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_SPI_H
#define __KinetisKE_CSL_SPI_H

#include "KinetisKE_csl.h"

/**
 * SPI Initialize Structure
**/
typedef struct
{
	uint8_t Mode;				//Work Mode
	uint8_t Direction;			//SPI Data Wire Directions
	uint8_t CLKPolarity;		//Clock Polarity
	uint8_t CLKPhase;			//Clock Phase
	uint8_t NSS;				//SS Pin
	uint8_t FirstBit;			//MSB or LSB
	uint8_t PreScaler;			//BaudRate PreScaler
	uint8_t Scaler;				//BaudRate Scaler
	uint8_t Match;				//Enable Match or not
	uint8_t MatchValue;			//Match Value
	uint16_t Pins;				//Pin Group
}SPI_InitTypeDef;

/**
 * SPI State enum
**/
typedef enum
{
	CSL_SPI_STATE_RESET			= 0x00u,
	CSL_SPI_STATE_READY			= 0x01u,
	CSL_SPI_STATE_BUSY			= 0x02u,
	CSL_SPI_STATE_BUSY_TX		= 0x03u,
	CSL_SPI_STATE_BUSY_RX		= 0x04u,
	CSL_SPI_STATE_BUSY_TX_RX	= 0x05u,
	CSL_SPI_STATE_ERROR			= 0x06u,
}SPI_StateTypeDef;

/**
 * SPI Handler Structure
**/
typedef struct __SPI_HandleTypeDef
{
	SPI_Type* 				Instance;
	SPI_InitTypeDef 		Init;
	uint8_t* 				pTxBuffer;
	uint16_t 				pTxXferCount;
	uint16_t 				pTxXferSize;
	uint8_t* 				pRxBuffer;
	uint16_t 				pRxXferCount;
	uint16_t 				pRxXferSize;
	void 					(*TxISR)(struct __SPI_HandleTypeDef*);
	void					(*RxISR)(struct __SPI_HandleTypeDef*);
	__IO SPI_StateTypeDef	gState;
	__IO uint8_t			ErrorCode;
	__IO CSL_LockTypeDef 	Lock;
}SPI_HandleTypeDef;

/**
 * SPI Mode(Master or Slave)
**/
#define SPI_MODE_MASTER		SPI_C1_MSTR_MASK
#define SPI_MODE_SLAVE		0x00u

/**
 * SPI Directions(Duplex or Half-Duplex)
**/
#define SPI_DIRECTION_2LINES		0x00u			//2 lines Duplex
#define SPI_DIRECTION_1LINES		0x01u			//1 line Half-Duplex
#define SPI_DIRECTION_1LINE_RX		0x02u			//1 line Rx Only

/**
 * SPI Clock Pority
**/
#define SPI_POLARITY_LOW			SPI_C1_CPOL_MASK
#define SPI_POLARITY_HIGH			0x00u

/**
 * SPI Clock Phase
**/
#define SPI_PHASE_1EDGE				SPI_C1_CPHA_MASK
#define SPI_PHASE_2EDGE				0x00u

/**
 * SPI SS pin 
**/
#define SPI_NSS_NONE				0x00u				//SS is configured as GPIO(no SPI)
#define SPI_NSS_MFIN				0x01u				//SS is configured as Mode Fault Input
#define SPI_NSS_CSOUT				0x02u				//SS is configured as Chip Select OUTPUT

/**
 * SPI First Bit
**/
#define SPI_FIRSTBIT_LSB			SPI_C1_LSBFE_MASK	
#define SPI_FIRSTBIT_MSB			0x00u

/**
 * SPI BaudRate PreScaler(Bus clock input)
**/
#define SPI_BUADRATEPRESCALER_DIV1			0x00u
#define SPI_BUADRATEPRESCALER_DIV2			0x10u
#define SPI_BUADRATEPRESCALER_DIV3			0x20u
#define SPI_BUADRATEPRESCALER_DIV4			0x30u
#define SPI_BUADRATEPRESCALER_DIV5			0x40u
#define SPI_BUADRATEPRESCALER_DIV6			0x50u
#define SPI_BUADRATEPRESCALER_DIV7			0x60u
#define SPI_BUADRATEPRESCALER_DIV8			0x70u

/**
 * SPI BaudRate Scaler(Prescaler input)
**/
#define SPI_BUADRATESCALER_DIV2				0x00u
#define SPI_BUADRATESCALER_DIV4				0x01u
#define SPI_BUADRATESCALER_DIV8				0x02u
#define SPI_BUADRATESCALER_DIV16			0x03u
#define SPI_BUADRATESCALER_DIV32			0x04u
#define SPI_BUADRATESCALER_DIV64			0x05u
#define SPI_BUADRATESCALER_DIV128			0x06u
#define SPI_BUADRATESCALER_DIV256			0x07u
#define SPI_BUADRATESCALER_DIV512			0x08u

/**
 * SPI Match Enable or not
**/
#define SPI_MATCH_ENABLE					0x01u
#define SPI_MATCH_DISABLE					0x00u

/**
 * SPI Pins
**/													//	SCK			MOSI		MISO		PCS	
#define SPI0_PIN_GROUP0						0x0000u	//	PTB2		PTB3		PTB4		PTB5
#define SPI0_PIN_GROUP1						0x0040u	// 	PTE0		PTE1		PTE2		PTE3
#define SPI1_PIN_GROUP0						0x0000u	// 	PTD0		PTD1		PTD2		PTD3
#define SPI1_PIN_GROUP1						0x0800u	//	PTG4		PTG5		PTG6		PTG7

/** 
 * SPI Error Code
**/
#define CSL_SPI_ERROR_NONE					0x00u	//No Error
#define CSL_SPI_ERROR_MODF					0x01u	//Mode Fault Error
#define CSL_SPI_ERROR_SPMF					0x02u	//Match Flags
#define CSL_SPI_ERROR_FLAG					0x03u	//Other Flags

/* Macros Functions */
/**
 * @brief	Enable/Disable the SPI
**/
#define __CSL_SPI_ENABLE(__HANDLE__)		SET_BIT(__HANDLE__->Instance->C1, SPI_C1_SPE_MASK)
#define __CSL_SPI_DISABLE(__HANDLE__)		CLEAR_BIT(__HANDLE__->Instance->C1, SPI_C1_SPE_MASK)

/**
 * @brief	Configure Communicate Direcction in Single-wire Mode
**/
#define __CSL_SPI_1Line_Tx(__HANDLE__)		SET_BIT(__HANDLE__->Instance->C2, SPI_C2_BIDIROE_MASK)
#define __CSL_SPI_1Line_Rx(__HANDLE__)		CLEAR_BIT(__HANDLE__->Instance->C2, SPI_C2_BIDIROE_MASK)

/**
 * @brief	Enable/Disable SPI Interrupts
**/
//SPIE --> Rx Buffer full & Mode Fault
#define __CSL_SPI_SPIE_ENABLE(__HANDLE__)	SET_BIT(__HANDLE__->Instance->C1, SPI_C1_SPIE_MASK)
#define __CSL_SPI_SPIE_DISBALE(__HANDLE__)	CLEAR_BIT(__HANDLE__->Instance->C1, SPI_C1_SPIE_MASK)
//SPTIE --> Tx Buffer empty
#define __CSL_SPI_SPTIE_ENABLE(__HANDLE__)	SET_BIT(__HANDLE__->Instance->C1, SPI_C1_SPTIE_MASK)
#define __CSL_SPI_SPTIE_DISBALE(__HANDLE__)	CLEAR_BIT(__HANDLE__->Instance->C1, SPI_C1_SPTIE_MASK)
//SPMIE --> Match Interrupt
#define __CSL_SPI_SPMIE_ENABLE(__HANDLE__)	SET_BIT(__HANDLE__->Instance->C2, SPI_C2_SPMIE_MASK)
#define __CSL_SPI_SPMIE_DISBALE(__HANDLE__)	CLEAR_BIT(__HANDLE__->Instance->C2, SPI_C2_SPMIE_MASK)

/**
 * @brief	Flush SPI Rx DR
**/
#define __CSL_SPI_FLUSH_DR(__HANDLE__)		(UNUSED(__HANDLE__->Instance->D))

/**
 * @brief 	Get Flags of SPI
**/
#define __CSL_SPI_GET_FLAG(__HANDLE__, __FLAG__)	((__HANDLE__->Instance->S & __FLAG__) ? SET : RESET)

/* SPI Init/Deinit Functions */
CSL_StatusTypeDef CSL_SPI_Init(SPI_HandleTypeDef* cspi);
CSL_StatusTypeDef CSL_SPI_DeInit(SPI_HandleTypeDef* cspi);
void CSL_SPI_MspInit(SPI_HandleTypeDef* cspi);
void CSL_SPI_MspDeInit(SPI_HandleTypeDef* cspi);

/* SPI Communication Functions */
CSL_StatusTypeDef CSL_SPI_Transmit(SPI_HandleTypeDef* cspi, uint8_t* TxBuffer, uint16_t Size, uint32_t Timeout);
CSL_StatusTypeDef CSL_SPI_Receive(SPI_HandleTypeDef* cspi, uint8_t* RxBuffer, uint16_t Size, uint32_t Timeout);
CSL_StatusTypeDef CSL_SPI_TransmitReceive(SPI_HandleTypeDef* cspi, uint8_t* TxBuffer, uint8_t* RxBuffer, uint16_t Size, uint32_t Timeout);

CSL_StatusTypeDef CSL_SPI_Transmit_IT(SPI_HandleTypeDef* cspi, uint8_t* TxBuffer, uint16_t Size);
CSL_StatusTypeDef CSL_SPI_Receive_IT(SPI_HandleTypeDef* cspi, uint8_t* RxBuffer, uint16_t Size);
CSL_StatusTypeDef CSL_SPI_TransmitReceive_IT(SPI_HandleTypeDef* cspi, uint8_t* TxBuffer, uint8_t* RxBuffer, uint16_t Size);

/* SPI Interrupt Functions */
void CSL_SPI_IRQHandler(SPI_HandleTypeDef* cspi);
void CSL_SPI_TxCpltCallback(SPI_HandleTypeDef* cspi);
void CSL_SPI_RxCpltCallback(SPI_HandleTypeDef* cspi);
void CSL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* cspi);
void CSL_SPI_MODFCallback(SPI_HandleTypeDef* cspi);
void CSL_SPI_MatchCallback(SPI_HandleTypeDef* cspi);

/* SPI Peripherals Function */
SPI_StateTypeDef CSL_SPI_GetState(SPI_HandleTypeDef* cspi);
uint8_t CSL_SPI_GetError(SPI_HandleTypeDef* cspi);
uint32_t CSL_SPI_GetBaudRate(SPI_HandleTypeDef* cspi);
uint8_t CSL_SPI_GetMatchValue(SPI_HandleTypeDef* cspi);
void CSL_SPI_SetMatchValue(SPI_HandleTypeDef* cspi, uint8_t match);

/* Private Macros for SPI parameters check*/
#define IS_SPI_MODE(mode)					((mode == SPI_MODE_MASTER) || (mode == SPI_MODE_SLAVE))
#define IS_SPI_DIRECTIONS(dir)				((dir == SPI_DIRECTION_2LINES) || \
											 (dir == SPI_DIRECTION_1LINES) || \
											 (dir == SPI_DIRECTION_1LINE_RX))
#define IS_SPI_POLARITY(polarity)			((polarity == SPI_POLARITY_HIGH) || (polarity == SPI_POLARITY_LOW))
#define IS_SPI_PHASE(phase)					((phase == SPI_PHASE_1EDGE) || (phase == SPI_PHASE_2EDGE))
#define IS_SPI_NSS(ss)						((ss == SPI_NSS_NONE) || (ss == SPI_NSS_MFIN) || (ss == SPI_NSS_CSOUT))
#define IS_SPI_BAUDRATEPSC(psc)				(((psc >> 4) +1 >= 0x01u ) && ((psc >> 4) <= 0x07u))
#define IS_SPI_BAUDRATESC(sc)				(((sc+1) >= 0x01u) && (sc <= 0x08u))
#define IS_SPI0_PINS(pin)					((pin == SPI0_PIN_GROUP0) ||(pin == SPI0_PIN_GROUP1))
#define IS_SPI1_PINS(pin)					((pin == SPI1_PIN_GROUP0) ||(pin == SPI1_PIN_GROUP1))

#endif /*__KinetisKE_CSL_SPI_H*/

//EOF
