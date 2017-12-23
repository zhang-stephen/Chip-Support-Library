 # Clock in Chip Support Library for NXP KinetisKEA series MCUs

#### Clock驱动模块

1. Clock驱动涉及两个硬件外设，分别为ICS和OSC，此二者的配置综合在这一个文件里
2. 该模块负责配置Core/Bus/TIM时钟，总线时钟输出以及其他功能
3. 若更改了Core/Bus时钟，务必调用`SystemClock_Update()`函数以更新CSL库中存储的时钟配置，此函数和时钟存储见于`system_keaz1284.h`文件

#### 硬件资源

1. ICS

   内部时钟源，含有两个振荡器和一个锁频环(FLL)

   + 振荡器IRC：内部振荡器，频率为37.5kHz，可通过CS3寄存器在合适范围内微调
   + 振荡器LPO：低功耗振荡器，频率为1kHz，为RTC和Watch Dog提供时钟源
   + 锁频环FLL：时钟倍频器，频率增益为1280，输入范围为31.25~39.0625kHz，最大输出为48MHz

2. OSC

   外部振荡器控制，具体见参考手册

   使用外部时钟时，请小心配置RDIV分频器，使输入FLL的频率不超出既定范围，若FLL禁用（低功耗模式），可以不配置RDIV分频器

3. 系统时钟

   由ICS和OSC生成，共有六种模式，分别为FEE/FBE/FBELP和FEI/FBI/FBILP模式，详见工程模板，也给出了每种模式的计算公式

4. 最大值

   KEA处理器支持的Core clock最大频率为48MHz，Bus clock为24MHz，TIM clock为48MHz

#### 基本驱动

1. 时钟配置句柄

   ```C++
   /**
    * defination of CLK structure
   **/
   typedef struct
   {
   	//ICS & FLL settings
   	uint8_t SystemClkSource;				 //Core Clock Source
   	uint8_t FLLClkSource;					 //FLL Clock Source 
   	uint8_t RDIV;							//Reference Clock Frequency Scaler
   	uint8_t BDIV;							//Bus Clock Scaler
   	
   	//OSC Settings
   	OSC_SourceTypeDef Type;					 //External clock signals
   	uint8_t Range;							//Ext-CLKsource Frequency
   	uint8_t PowerMode;						//LP or HF
   	
   	//SIM->CLKDIV
   	uint32_t Core_PreScaler;				 //Core ClkFrequency after FLL DIV
   	uint32_t Bus_PreScaler;					 //Bus/Flash ClkFrequency after FLL DIV
   }CLK_ConfigTypeDef;
   ```

   通过配置此句柄并调用初始化函数完成时钟初始化

   若配置中Core clock大于24MHz，则一定要对Bus Clock进行二分频，否则可能会直接进入HardFault中断

2. TIM Clock

   可通过函数`void CSL_CLK_TIMClockSouceConfig(uint8_t TimerDivider)`配置定时器时钟分频，一般定时器都有多个时钟供选择，若使用TIM clock作为定时器时钟，则请酌情配置此函数

3. 获取时钟频率

   提供函数直接获取内核/总线/定时器时钟

4. 配置总线时钟输出

   通过函数`void CSL_CLK_BusClockOutputEnable(uint32_t busref)`配置总线时钟是否在PTH2上输出，传入参数为输出分频系数，尽量将输出时钟频率控制在5MHz以下，否则吉布斯现象和信号失真非常严重

#### ICS中断

​	内部时钟源支持一个中断，即FLL失锁中断，若SC4寄存器中LOLIE位置位，则FLL失锁时会生成硬件中断

​	该中断生成时即表明FLL运行不正常，可在此中断内重新配置时钟，使系统恢复正常运行，可视为时钟系统的一种安全机制

​	该中断的开启与否在`KinetiskE_csl_config.h`中进行配置，通过修改宏定义`ICS_EXTI_STAT`确定是否开启FLL失锁中断，与此相关的语句位于`CSL_Init()`函数中

​	用户若使用此中断，仅需重定义`CSL_ICS_Callback()`即可

#### 小技巧

如何测量总线时钟？

​	使总线时钟以一定分频系数输出，接入示波器测量后乘以分频系数即为总线时钟





Copyright &copy; Yangtze University EE Stark Zhang, All Rights Reserved 12.23.2017

Copyright &copy; 长江大学 电子信息学院 张璞 保留所有权利  2017.12.23