# GPIO/FGPIO & Port in Chip Support Library for NXP KinetisKEA series MCUs

#### 概述

ARM Cortex-M0+支持快速的，可在一个周期内完成操作的GPIO，称为单周期IO(FGPIO)，此IO模块亦被KinetisKEA系列MCU支持，使用和GPIO一样的寄存器别名，但硬件映射地址不同，关于GPIO和FGPIO的映射地址，请参考数据手册

除此之外，KinetisKEA系列亦支持部分引脚的大电流驱动，在CSL中也提供相应的库函数以支持开启和禁用大电流驱动功能

KinetisKEA系列的内部上拉电阻仅对数字I/O输入有效，如果需要输出上拉，请外接上拉电阻，上拉电压不得超过VDD/VDDA

值得注意的是，在此系列中，引脚PTA2和PTA3为真开漏引脚，即无外部上拉时无法输出高电平，仅能输出Hi-Z（高阻态）和低电平，此引脚对亦是I2C0的默认IO接口

对于某些关键的系统引脚，如SWD接口，RESET和NMI等，请尽量不要占用

#### 引脚复用

KinetisKEA系列的引脚复用管理较为复杂，它并不是通过GPIO或者PORT模块实现的，而是由分散的几个寄存器一起实现的

引脚复用优先级请参考参考手册第十章——信号多用复用和信号说明

1. `SIM_PINSELx`

   此寄存器位于系统集成模块(SIM)中，共有两个，主要控制IO的数字功能复用，例如IRQ，UART，FTM等外部引脚的选择

   此外在`SIM_SOPT0[CLKOE]`位控制总线时钟是否在PTH2上输出

2. `ADC_APCTL1`

   此寄存器控制ADC_SEx引脚是否禁用数字功能

   ADC模拟输入引脚是固定不变的，模拟输入功能的优先级为最高

3. `ACMPx_C2`

   此寄存器控制ACMP的模拟输入

   另有`ACMPx_CS[ACOPE]`位使能ACMP比较结果在引脚上输出

4. `KBIx_PE`

   该位控制KBI引脚选择，详见[KBI](./kbi.md)帮助文档

#### 中断

GPIO/FGPIO中断分别由[IRQ](./irq.md)和[KBI](./kbi.md)实现，具体参见相应的手册

#### 端口滤波

KinetisKEA系列微控制器支持硬件滤波，通过`PORT_IOFLTx`寄存器控制，此部分未在CSL中支持，如有需要可以自行配置PORT寄存器以实现

#### 编程模型

此模块提供GPIO和FGPIO的初始化结构体，用户实例化之后调用初始化函数即可完成初始化

特别的，CSL支持一次性对同一个GPIO的多个引脚操作，在对引脚进行赋值时使用按位或运算按照如下格式即可

```C++
GPIO_Init.Pin = GPIO_Pin_1 | GPIO_Pin_2;
```

至于其他细节，请参见工程模板中的GPIO模块及其函数应用





Copyright &copy; Yangtze University EE Stark Zhang, All Rights Reserved 12.28.2017

Copyright &copy; 长江大学 电子信息学院 张璞 保留所有权利  2017.12.28

