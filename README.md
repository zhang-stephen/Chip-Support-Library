# Chip Support Library for NXP/FreeScale KinetisKEA MCUs
This is a Low-layer Support Library for KinetisKE MCUs of NXP/Freescale, I called it as CSL


### 概述  
 CSL是一个以STM32-HAL为蓝本设计的针对KinetisKEA系列MCU的软件开发库，我将尽量为这个库提供详尽的使用说明

    1. 该软件库写于KinetisKEA128处理器平台(S9KEAZ128AMLK)，理论上支持KEAZ128和KEAZ64两种微处理器
    2. 我致力于降低该软件库内部的耦合度，目前仅有几个外设驱动是必须的，它们负责驱动该处理器的内核，时钟，内部flash，以及包含了一些必须的定义
    3. 在编程时，我修改了startup(asm)文件和system_keaz128xxx4.h/.c，这让它们看起来和官方提供的文件有些不同，因此在使用CSL时，务必使用我提供的文件

 ### 文件说明  
 上传到github时，我只提供必须文件，而工程模板及例程将在release中发布，包括IAR for ARM(v8.10.0)和MDK-ARM(v5.24a)，这是我目前正在使用的两个开发环境

 1. 必需文件  
    前文中已经说过哪些文件是必不可少的，这里给出CSL的组织结构  

    ```asm
    KinetisKEA_CSL  
        +-----inc\
        |-----------\ KinetisKE_csl.h  
        |-----------\ KinetisKE_csl_assert.h  
        |-----------\ KinetisKE_csl_clk.h  
        |-----------\ KinetisKE_csl_config.h    
        |-----------\ KinetisKE_csl_cortex.h    
        |-----------\ KinetisKE_csl_def.h  
        |-----------\ KinetisKE_csl_flash.h  
        |-----------\ KinetisKE_csl_modulexx.h   
        +-----src\  
        |-----------\ KinetisKE_csl.c  
        |-----------\ KinetisKE_csl_clk.c  
        |-----------\ KinetisKE_csl_cortex.c  
        |-----------\ KinetisKE_csl_flash.c  
        |-----------\ KinetisKE_csl_modulexx.c  
        +-----\ KinetisKE_csl_inc.h
    ```

      其中`KinetisKE_csl_inc.h`包含了所有的CSL头文件，可在main函数之前调用，此外其他的所有文件基本上均可删除而不影响使用

### 使用说明

1. 该软件包依靠宏定义确定使用芯片型号

   ```c++
   #ifdef	CSL_KEAZ128xxx4
    #include "kinetis_keaz128xxx4.h"
    #define FLASH_SIZE				((uint32_t)(128U*1024U))		//Flash: 128KiB
   #elif defined(CSL_KEAZ64xxx4)
    #include "kinetis_keaz64xxx4.h"
    #define FLASH_SIZE				((uint32_t)(64U*1024U))		    //Flash: 64KiB
   #else
    #error "Please specify your MCU model!"
   #endif /* Model Selection */
   ```

2. 该系列芯片基于ARM Cortex-M0+架构，若要使用数学库请在预处理器中定义`ARM_MATH_CM0PLUS`，不过该芯片无FPU，__故不要对其浮点性能抱太大希望__  

3. KinetisKEA的上电复位(POR)

   这是一个比较复杂的话题，我尽量用简洁的语言将这一过程描述的简明易懂

   1) 芯片上电后，首先执行`Reset_Handler()` 函数，此函数在startup文件中定义

   ```asm
   ; Reset Handler
   ; MDK-ARM
   Reset_Handler   PROC
                   EXPORT  Reset_Handler             [WEAK]
                   IMPORT  SystemInit
                   IMPORT  __main
                   LDR     R0, =SystemInit
                   BLX     R0
                   LDR     R0, =init_data_bss
                   BLX     R0
                   LDR     R0, =__main
                   BX      R0
                   ENDP
   ```

     基本上可以确定，首先执行`SystemInit()`函数，然后执行主函数，在`SystemInit()`函数中，主要进行基本的时钟配置，使芯片可以正常运行以进行其他外设的初始化，进行时钟配置后会执行`SystemClock_Update()`函数，以便将当前时钟的频率保存下来作为内核与Flash时钟初始化之用

   2) 进入主函数后，首先执行`SystemClock_Init()`函数，根据用户配置更新时钟并更新时钟频率，作为新的内核与Flash初始化的依据 

   3) 其次执行`CSL_Init()`函数，该函数主要进行这么几件事：

   + 开启Flash时钟，根据配置决定是否开启Flash高速缓存、推断以及Flash中断并对内部Flash进行初始化
   + 根据配置决定是否开启ICS（内部时钟源）失锁中断（仅对FLL有效）
   + 初始化SysTick定时器为每1ms进入一次中断（可以自定义）

   4) 以上都无错误时，进行用户的外设初始化，执行用户代码，直到断电或者再次复位

4. CSL中的用户配置

   CSL中的用户配置见`KinetisKE_csl_config.h`，其中的大致内容已经在上文中提过，该文件主要规定了一些重要中断的开启与否，外部时钟源、内部振荡器和内部低功耗振荡器的频率，以及其他用户配置，鉴于该文件注释较详细，此处不多赘述，但是使用前记得按需修改配置

   除此之外，该文件还有各个外设模块的时钟选通配置，均为宏定义。在调用外设初始化及使能函数时，这些宏定义需要用户在自己的外设初始化函数中调用，具体请参阅该文件

   __注意：用户在执行自己的初始化代码时，必须首先初始化GPIO/FGPIO，以免和其他外设引脚相冲突！__

   __注意：初始化任何引脚时，尽量不要占用PTA4/PTA5/PTB4/PTB6/PTB7/PTC4！__

5. 其他详细配置请见工程模板

6. 具体使用请见例程



#### Documents

?	[使用帮助](./Documents/Overview.md) 请点击此链接

#### 更新日志

 version 0.0.1 (2017.12.4)

1. 仅提供工程模板

version 0.2.5 (2017.12.22)

1. 修复了大量bug
2. 提供对UART，PWT，PMC，FLASH的支持
3. 其他模块正在完善中

Copyright &copy; Yangtze University, Stark-Zhang, All rights reserved  
Copyright &copy; 长江大学 电子信息学院 张璞 保留所有权利