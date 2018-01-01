# PIT in Chip Support Library for NXP KinetisKEA series MCUs

#### 概述

周期性中断定时器(Periodic interrupt timer, PIT)是KinetisKEA系列中的具有的一种基本定时器，它不具有外部信号，只能在计数结束时根据用户配置选择是否生成中断，可供周期性操作之用

PIT定时器的时钟源为总线时钟，不可更改和分频，其计数模式为向下计数，最大计数值为32位，即2^32-1，在KinetisKEA系列中，所有的PIT定时器都提供两个通道，可以级联计数，但是只能第n通道向下级联第n-1通道，第0通道无法进行级联

单通道计数周期为`T = (Counter+1) / SystemBusClock`，记双通道的计数周期分别为T0和T1，装载值为C0和C1，则通道1计数周期为`T1 = T0 * C1`，式中`T0 = (C0+1) / SystemBusClock`

#### 编程模型

和其他模块一样，实例化句柄结构体并调用初始化函数即可完成初始化

具体操作请参见例程，注意计数装载值不要超过寄存器限制即可



Copyright &copy; Yangtze University EE Stark Zhang, All Rights Reserved 12.28.2017

Copyright &copy; 长江大学 电子信息学院 张璞 保留所有权利  2017.12.28