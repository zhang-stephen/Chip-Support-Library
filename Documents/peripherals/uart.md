# UART in Chip Support Library for NXP KinetisKEA series MCUs

#### 概述

此手册叙述如何在CSL软件库中使用UART以及关于UART的注意事项

1. 硬件资源

   KEA系列微控制器最多提供三个互相独立的UART模块，它们拥有完全独立的硬件资源（时钟除外），双数据缓冲区等等特点，支持8/9位数据模式，奇偶校验和可编程的停止位，其余部分详见参考手册

2. UART波特率

   该软件库可以支持不大于1382400bps的波特率，实际上更高的波特率我并没有测试，有兴趣的同学可以尝试一下逼近极限

   UART波特率的产生由BDH和BDL寄存器负责，其中13位SBR是总线时钟分频器，波特率计算公式为

   > BaudRate = SystemBusClock / SBR

   __在8位数据模式下允许的波特率最大误差为4.5%，9位数据模式下为4%__

   __如果发生了更改总线时钟的操作，在UART未初始化时，应该调用`SystemClock_Update()`函数更新总线时钟后再初始化UART；如果UART已经初始化，则应该立即禁用UART的发送与接收，更新总线时钟频率后重新写入UART的BDH/BDL寄存器的SBR位以重设波特率__

   __总线时钟的重要性不言而喻，如不使用CSL的Clock模块进行时钟初始化，请最大限度的保证总线时钟的预设频率和由上述函数计算得出结果吻合，以保证波特率的正确性__

   #### 初始化

   该模块使用句柄结构体控制对每一个UART模块的初始化，此结构体中包含初始化结构体和程序定义在堆栈上的数据缓冲区以及全局状态等信息，可以认为这是对UART封装的对象(C with Class)

   ```c++
   /**
    * UART Initialization Structure
   **/
   typedef struct
   {
   	uint32_t			UART_Pins;					//UART Pins Alternative
   	uint8_t				StopBits;					//UART Stop Bits
   	uint32_t 			BuadRate;					//UART BaudRate
   	uint8_t 			UART_Line;					//UART Lines(1/2/loop)
   	uint8_t 			WordLength;					//Word Length(8bit or 9bit), Include Pariting Bits
   	uint8_t 			UART_Mode;					//Work Mode
   	uint8_t 			Parity;						//Parity Check
   }UART_InitTypeDef;

   /**
    * UART Handle Structure
   **/
   typedef struct
   {
   	UART_Type*						Instance;		//Instance of UART
   	UART_InitTypeDef				Init;			//Initialize Structure
   	uint8_t* 						pTxBuffer;		//Transmit Buffer
   	uint16_t						pTxXferSize;	//Size of Tx Buffer
   	__IO uint16_t 					pTxXferCount;	//Counter of Tx Buffer
   	uint8_t* 						pRxBuffer;		//Receive Buffer
   	uint16_t						pRxXferSize;	//Size of Rx Buffer
   	__IO uint16_t 					pRxXferCount;	//Counter of Rx Buffer
   	__IO CSL_LockTypeDef			Lock;			//Process Lock
   	__IO CSL_UART_StateTypeDef 		gState;			//Global State
   	__IO CSL_UART_StateTypeDef		RxState;		//Receiver State
   	__IO uint32_t					ErrorCode;		//Error Code 
   }UART_HandleTypeDef;
   ```

   给此结构体赋值后调用初始化函数即可完成对UART的初始化，提供用户自定义初始化接口，即`CSL_UART_MspInit()`，您可以在自己的代码中重定义此函数，但是不要与上述结构体的配置冲突

   对于UART，CSL中提供不同的初始化函数以供不同的通信模式使用

   + `CSL_UART_Init()` 	通用初始化函数，初始化后的UART支持常用的polling/IT工作模式
   + `CSL_UART_HalfDuplex_Init()` 半双工/单工/内环模式初始化
   + `CSL_UART_MultiProcessorInit()` 多处理器通信，此模式需要硬件电路支持
   + 暂不支持LIN模式

   #### UART工作模式

   该模块支持轮询和中断工作模式，其中需要用户调用的、中断模式的函数均带有IT后缀

   1. 外设操作函数

      外设操作函数不是很多，功能分别是发送LIN断点，使能半双工/单工模式下的接收器或者发送器，获取当前UART模块的状态和错误代码，具体详见注释和源码

   2. 发送/接收函数

      发送和接收函数在轮询模式下，每读出/写入一个字节的数据，都会去轮询状态寄存器，如果相应标志位置位，则在此函数内执行其他操作

      在中断模式下，所谓的发送/接收函数仅仅是一个配置函数而已，具体的工作由中断和在中断里运行的静态函数完成，在操作执行完成后会调用各个回调函数，用户此时可以自定义自己的中断操作

   3. 中断函数及其回调

      此部分在上文中已有阐述，您不需了解中断的具体操作，只要可以正常使用中断回调函数即可

   4. 其他

      CSL库支持中途取消操作，以及提供回调接口供用户自定义取消操作的善后工作，请见源码部分

   #### CSL例程

   例程中提供对UART的各种常用初始化/去初始化的样例，在对UART寄存器操作之前，请先调用位于`KinetisKE_csl_config.h`中的系统时钟配置宏定义，开启对应UART时钟，否则将会触发HardFault中断

   由于Cortex-M0+的限制（ARMv6），进入HardFault被认为是发生了无法挽回的操作，应当立刻重置系统