/**
 * Title	KinetisKE MCUs Public Header File
 * Lisence	General Public Lisence v2.0(GPLv2.0)
 * Author	Stark Zhang
 * Debug 	None
**/

#ifndef __KinetisKE_Common_H
#define __KinetisKE_Common_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "stdio.h"
#include "stdint.h"
     
/* if in ARMCC */
#if defined(__CC_ARM) || defined(__GNUC__)
 #define __no_operation()       __NOP()
#endif /*__CC_ARM || __GNUC__*/

#ifdef	CSL_KEAZ128xxx4
 #include "kinetis_keaz128xxx4.h"
 #define FLASH_SIZE				((uint32_t)(128U*1024U))		//Flash: 128KiB
#elif defined(CSL_KEAZ64xxx4)
 #include "kinetis_keaz64xxx4.h"
 #define FLASH_SIZE				((uint32_t)(64U*1024U))		    //Flash: 64KiB
#elif defined(CSL_KEAZ8xxx4)
 #include "kinetis_keaz8xxx4.h"
 #define FLASH_SIZE 			((uint32_t)(8U*1024U))			//Flash: 8KiB
#else
 #error "Please specify your MCU model!"
#endif /* Model Selection */
     
/**
 * Error Handler Function 
*/
/**
 * @brief 	Error Handler
 * @param	uint8_t* file
	 			File that error occured
 * @param 	uint32_t line
				Line taht error occured
 * @return	None
 * @note 	Don't Call it in CSL Drivers
**/
__STATIC_INLINE void __Error_Handler(char* file, uint32_t line)
{
	while(1)
	{
		
	}
}

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_Common_H*/

//EOF
