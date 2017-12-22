/**
 * Title 	Chip Support Library for KinetisKE MCUs Header File
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_H
#define __KinetisKE_CSL_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_Common.h"
#include "KinetisKE_csl_assert.h"
#include "KinetisKE_csl_config.h"
#include "KinetisKE_csl_def.h"
#include "system_keaz128xxx4.h"

/**
 * Enable/Disable Global Interrupts 
**/
#define __CSL_GIRQ_ENABLE()             __set_PRIMASK(0)
#define __CSL_GIRQ_DISABLE()            __set_PRIMASK(1)

/* Initialization and de-initialization functions */
CSL_StatusTypeDef CSL_Init(void);
CSL_StatusTypeDef CSL_Deinit(void);
void CSL_MspInit(void);
void CSL_MspDeinit(void);
CSL_StatusTypeDef CSL_InitTick(uint32_t TickPriority);

/* Functions of Systick */
void CSL_IncTick(void);
uint32_t CSL_GetTick(void);
void CSL_SuspendTick(void);
void CSL_ResumeTick(void);
void CSL_Delay(__IO uint32_t Delay);

/* Device functions */
uint16_t CSL_GetDevID(void);
void CSL_GetUID(uint32_t *uid);

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_H*/

//EOF
