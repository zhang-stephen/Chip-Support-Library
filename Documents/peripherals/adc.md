# ADC in Chip Support Library for NXP KinetisKEA series MCUs

#### 概述

​	此手册讲述如何使用CSL中ADC驱动，其中ADC驱动分为四个文件分别是`KinetisKE_csl_adc.h/.c`和`KinetisKE_csl_adc_ex.h/.c`，前者对应ADC的基本驱动，后者对应ADC的特殊应用模式，即温度传感器模式，该驱动提供了详细的ADC的初始化和应用函数

#### 硬件参数描述

1. 时钟频率

   ADC可配置为高速或低功耗模式

   在高速模式下，时钟频率范围为0.4~8.0MHz；低功耗模式下，时钟频率为0.4~4.0MHz，请小心配置此时钟，避免超过其最大值

   ADC异步时钟(ADACK)的频率范围是3.3MHz（2~5MHz，高速模式），或者2MHz（1.25~3.3MHz，低功耗模式）

2. 电压

   供电电压同VDDA和VDD，2.7~5.5V，此外VDDA和VDD的差值不得大于100mV

   外部参考电压引脚对的电压要求详见数据手册，其中VREFL的供电范围为0~VDDA/2，VREFH的供电范围为VDDA/2~VDDA

3. 温度传感器模式

   在温度传感器模式下，时钟频率仅能配置为低于1MHz，若使用温度传感器，请使用`KinetisKE_csl_adc_ex.h/.c`的相关函数，或者用户需要小心配置ADC使之满足要求

   此外，由于数据手册只提供了5V供电时的温度传感器参数，故驱动库也仅支持5V条件下的温度传感器，其它供电电压下不要使用

#### 基本驱动部分

​	用户初始化ADC的句柄结构体`ADC_HandleTypeDef`并调用初始化函数完成初始化，其中`CSL_ADC_MspInit()`函数为用户提供了自定义配置的入口，用户应在自己的代码里重定义此函数而不是改写它。下面是ADC句柄的定义：

```C++
/**
 * ADC Handle TypeDef
**/
typedef struct
{
	ADC_StateTypeDef 		gState;				//Global State of ADC
	ADC_InitTypeDef 		Init;				//Initialized Structure
	ADC_FIFOTypeDef 		FIFO;				//FIFO Configuration Structure
	ADC_CompTypeDef 		Compare;			//Compare Configuration Structure
	ADC_TrigTypeDef 		Trigger;			//Trigger Configuration Structure
}ADC_HandleTypeDef;
```

​	如果需要使用FIFO，Hardware Compare或者设置触发模式，请分别给其中的结构体按照头文件的各种宏定义赋值，之后调用初始化函数即可

​	除基本初始化和去初始化函数外，基本驱动中也提供相应的获取ADC转换结果的函数，具体请见驱动库内的函数

#### 中断

​	ADC支持转换完成中断，若SC1寄存器中的AIEN位置1，当此寄存器内COCO位置1时，生成硬件中断；若要清除该中断标志位，可以写入SC1寄存器或者读取结果寄存器R

​	ADC仅支持此中断，有关COCO位置位的信息，请参见参考手册

​	CSL也提供了`CSL_ADC_IRQHandler()`函数和对应的`CSL_ADC_ConvCpltCallback()`函数，方便用户在ADC产生中断时进行操作，关于此函数的详细信息，请参见函数注释

#### 温度传感器

​	上文中已经提到，函数库中提供了一个独立的温度传感器配置函数，若要去初始化，仍可使用基本驱动中的`CSL_ADC_DeInit()`函数；温度传感器仅支持5V条件下的ADC

​	温度传感器的默认配置为，低功耗模式，至多1MHz时钟，12位数据模式，连续转换，无FIFO，无Compare，软件触发，长采样时间

​	此部分还提供一个温度计算函数，用于直接获取当前温度，传入参数为ADC的转换数据（第一手数据，直接读R寄存器所得），详细内容见函数注释

#### 基本初始化

​	工程模板中提供了对于ADC进行基本初始化的例程，通过更改`__ADC_MODE`宏定义，可以实现单通道连续转换，4级FIFO连续转换，4级FIFO扫描模式等，具体请见例程



Copyright &copy; Yangtze University EE Stark Zhang, All Rights Reserved 12.23.2017

Copyright &copy; 长江大学 电子信息学院 张璞 保留所有权利  2017.12.23