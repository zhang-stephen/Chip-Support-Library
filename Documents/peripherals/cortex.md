# Cortex in Chip Support Library for NXP KinetisKEA series MCUs

#### 概述

此部分详述ARM Cortex-M0+内核部分的相关配置，包括但不限于SysTick、NVIC、SRAM等

#### SysTick定时器

在ARM Cortex-M系列微处理器中，都提供一个24位的减计数器SysTick，用作系统节拍，关于此定时器的详细信息，请参阅ARM官方文档，此处只介绍CSL中使用SysTick做ms级延时的相关部分

