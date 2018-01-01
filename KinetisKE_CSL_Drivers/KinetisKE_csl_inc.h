/**
 * Title 	CSL Commen Include File
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
 * version	0.2.5(2017.12.22)
**/

#ifndef __KinetisKE_CSL_INC_H
#define __KinetisKE_CSL_INC_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

/* Low-layer Header File */	 
#include "KinetisKE_common.h"
#include "system_keaz128xxx4.h"

/* Common Partitions in CSL for KinetisKE MCUs */
#include "./inc/KinetisKE_csl_assert.h"
#include "./inc/KinetisKE_csl_def.h"

/* Modules in CSL for KinetsKE MCUs */
#include "./inc/KinetisKE_csl.h"
#include "./inc/KinetisKE_csl_acmp.h" 
#include "./inc/KinetisKE_csl_adc.h"
#include "./inc/KinetisKE_csl_adc_ex.h"
#include "./inc/KinetisKE_csl_clk.h"
#include "./inc/KinetisKE_csl_cortex.h"
#include "./inc/KinetisKE_csl_flash.h"
//#include "./inc/KinetisKE_csl_ftm.h"
#include "./inc/KinetisKE_csl_gpio.h"
#include "./inc/KinetisKE_csl_gpio_ex.h"
#include "./inc/KinetisKE_csl_irq.h"
#include "./inc/KinetisKE_csl_pit.h"
#include "./inc/KinetisKE_csl_pmc.h"
#include "./inc/KinetisKE_csl_pwt.h"
//#include "./inc/KinetisKE_csl_spi.h"
#include "./inc/KinetisKE_csl_uart.h"
#include "./inc/KinetisKE_csl_wdog.h"

#ifdef __cplusplus
 }
#endif /*__cplusplus*/
	 
#endif /*__KinetisKE_INC_H*/

//EOF
