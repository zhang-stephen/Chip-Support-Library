/**
 * Title 	Some defination in CSL
 * Lisence	General Public Lisence v2.0(GPLv2.0)
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_DEF_H
#define __KinetisKE_CSL_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

/**
 * @brief	Includes for KinetisKE MCUs
**/
#include <stdio.h>
#include "KinetisKE_common.h"

/**
 * @brief	CSL Status defination
**/
typedef enum
{
	CSL_OK 		= 0x00U,
	CSL_Error	= 0x01U,
	CSL_Busy	= 0x02U,
	CSL_Timeout	= 0x03U,
}CSL_StatusTypeDef;

/**
 * @brief	Flag/IT Status
**/
typedef enum
{
	RESET 	= 0,
	SET		= !RESET,
}FunctionalState, Flag_Status, IT_Status;

/**
 * @brief	CSL Lock Status defination
**/
typedef enum
{
	CSL_LOCKED 		= 0x00U,
	CSL_UNLOCKED 	= 0x01U,
}CSL_LockTypeDef;

/**
 * @brief	Exported Macros
**/
#define CSL_MAX_DELAY						0x00FFFFFFU			//Max Delay Core period
#define CSL_IS_BIT_SET(REG, BIT)         	(((REG) & (BIT)) != RESET)
#define CSL_IS_BIT_CLR(REG, BIT)         	(((REG) & (BIT)) == RESET)
#define UNUSED(x)							((void)(x))

/**
 * @brief	Support to RTOS
**/
#if (USE_RTOS == 1)
 #error "current CSL verison is not supported to RTOS"
#else
 #define __CSL_LOCK(__HANDLE__) do{                                        \
                                    if((__HANDLE__)->Lock == CSL_LOCKED)   \
                                    {                                      \
                                       return CSL_Busy;                    \
                                    }                                      \
                                    else                                   \
                                    {                                      \
                                       (__HANDLE__)->Lock = CSL_LOCKED;    \
                                    }                                      \
                                  }while (0U)
  #define __CSL_UNLOCK(__HANDLE__)                                          \
                                  do{                                       \
                                      (__HANDLE__)->Lock = CSL_UNLOCKED;    \
                                    }while (0U)
#endif /*USE_RTOS == 1 or 0*/
										
/** 
 * @brief 	Reset the Handle's State field.
 * @param 	__HANDLE__ specifies the Peripheral Handle
 * @return 	None
**/
#define __CSL_RESET_HANDLE_STATUS__(__HANDLE__)		((__HANDLE__)->gState = 0x00U)

/* Some Keyword Common interface */
#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
  #ifndef __packed
    #define __packed __attribute__((__packed__))
  #endif /* __packed */
#endif /* __GNUC__ */							

/* Macro to get variable aligned on 4-bytes, for __ICCARM__ the directive "#pragma data_alignment=4" must be used instead */
#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __ALIGN_END
    #define __ALIGN_END    __attribute__ ((aligned (4)))
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN  
    #define __ALIGN_BEGIN
  #endif /* __ALIGN_BEGIN */
#else
  #ifndef __ALIGN_END
    #define __ALIGN_END
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN      
    #if defined   (__CC_ARM)      /* ARM Compiler */
      #define __ALIGN_BEGIN    __align(4)
    #elif defined (__ICCARM__)    /* IAR Compiler */
      #define __ALIGN_BEGIN 
    #endif /* __CC_ARM */
  #endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */								
										
/** 
  * @brief  __RAM_FUNC definition
  */ 
#if defined ( __CC_ARM   )
/* ARM Compiler
   ------------
   RAM functions are defined using the toolchain options. 
   Functions that are executed in RAM should reside in a separate source module.
   Using the 'Options for File' dialog you can simply change the 'Code / Const' 
   area of a module to a memory space in physical RAM.
   Available memory areas are declared in the 'Target' tab of the 'Options for Target'
   dialog. 
*/
#define __RAM_FUNC HAL_StatusTypeDef 

#elif defined ( __ICCARM__ )
/* ICCARM Compiler
   ---------------
   RAM functions are defined using a specific toolchain keyword "__ramfunc". 
*/
#define __RAM_FUNC __ramfunc CSL_StatusTypeDef

#elif defined   (  __GNUC__  )
/* GNU Compiler
   ------------
  RAM functions are defined using a specific toolchain attribute 
   "__attribute__((section(".RamFunc")))".
*/
#define __RAM_FUNC HAL_StatusTypeDef  __attribute__((section(".RamFunc")))

#endif /*__CC_ARM || __GUNC__ || __ICCARM*/

/** 
  * @brief  __NOINLINE definition
  */ 
#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
/* ARM & GNUCompiler 
   ---------------- 
*/
#define __NOINLINE __attribute__ ( (noinline) )

#elif defined ( __ICCARM__ )
/* ICCARM Compiler
   ---------------
*/
#define __NOINLINE _Pragma("optimize = no_inline")

#endif /*__CC_ARM || __GUNC__ || __ICCARM*/
										
/** @addtogroup Exported_macro
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL))) 
										
#ifdef __cplusplus
 }
#endif /*__cplusplus*/
	 
#endif /*__KinetisKE_CSL_DEF_H*/

//EOF
