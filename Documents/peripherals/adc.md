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

#### FIFO

此处提供一个保证FIFO次序的思路，针对各位参加电磁/电磁直立/无线节能的小伙伴们，当然如果使用外部ADC可忽略此步，此处亦提供使用硬件触发的初始化流程

```C++
//ADC Handle
ADC_HandleTypeDef cadc = {0};

//Global Array to store ADC Data
uint16_t adc_data[0x20u] = {0};

/**
 * @brief	User ADC Template Code
 * @basic configuration:
 * 			ADC CLK = SystemBusClock / 2 / 2 = 6MHz
 * 			ADC word length is 12bits
 * 			ADC works in continuous mode
 * 			Voltage Reference is VDDA pin
 * @advanced configuration:
 *			FIFO Mode is SET and its depth is 4-level
 * 			Channel 0~3
 * 			Trigged by PIT Channel 0
**/
void User_ADC_FIFO_Init(void)
{
	//enable ADC Clock
  	__CSL_ADC_CLK_ENABLE();
	
  	//ADC Configuration
  	cadc.Init.ConvMode = Continuous;
	cadc.Init.Reference = ADC_REF_VDDA;
	cadc.Init.Clock = ADC_CLK_BusClock_DIV2;
	cadc.Init.PreScaler = ADC_CLK_DIV2;
	cadc.Init.Length = ADC_DATA_LEN12;
  	
  	//Trigger
  	cadc.Trigger.state = ADC_TRIG_HARDWARE;
  	cadc.Trigger.HardTrigger = ADC_HTRIG_PITCH0;
  
  	//Compare is RESET
	cadc.Compare.state = RESET;
	
  	//FIFO
	cadc.FIFO.state = SET;			
	cadc.FIFO.Depth = ADC_FIFO_D4;
	cadc.FIFO.TrigMode = ADC_FTRIG_CONTINUOUS;
	cadc.FIFO.Channel[0] = ADC_Channel_0;
	cadc.FIFO.Channel[1] = ADC_Channel_1;
	cadc.FIFO.Channel[2] = ADC_Channel_2;
	cadc.FIFO.Channel[3] = ADC_Channel_3;
  
  	if(CSL_ADC_Init(&cadc) != CSL_OK)
	{
		__Error_Handler(__FILE__, __LINE__);
	}
}

void CSL_ADC_MspInit(ADC_HandleTypeDef* cadc)
{
  	//Enable ADC Interrupt
  	__CSL_ADC_IT_ENABLE();
  	
  	//NVIC SETTING
  	CSL_NVIC_EnableIRQ(ADC_IRQn);
  	CSL_NVIC_SetPriority(ADC_IRQn, 1);	//PIT Channel 0 is 1, too
}

//ADC Interrupt Callback
void CSL_ADC_ConvCpltCallback(ADC_HandleTypeDef* cadc)
{
  	for(uint8_t i = 0; i < cadc.FIFO.Depth; i++)
    {
      	adc_data[ADC->SC1 & ADC_SC1_ADCH_MASK] = ADC->R;
    }
}

//ADC Interrupt Function
void ADC0_IRQHandler(void)
{
	CSL_ADC_IRQHandler(&cadc);
}
```



Copyright &copy; Yangtze University EE Stark Zhang, All Rights Reserved 12.23.2017

Copyright &copy; 长江大学 电子信息学院 张璞 保留所有权利  2017.12.23