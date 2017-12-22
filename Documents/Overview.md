# Chip Support Library for NXP KEAZ1284/KEAN64/KEAN8 用户手册

这是一个私人开发的，仅对NXP KinetisKEA系列芯片提供支持的底层驱动库，简称CSL，作者是中国某大学在校本科学生，为参加第十三届NXP杯智能汽车竞赛而开发

这个手册文件会提供对该驱动库的基本技术支持，包括这个库的一般思路，使用方法以及其他信息

#### Overview  
1. 该驱动库的支持范围 
   该驱动库支持KEA系列芯片的绝大部分外设及其功能，它提供比较完整的low-layer APIs，同时预留各种接口供用户自定义外设配置  

2. 中断  

   除了启动文件中已注册的中断函数外，该驱动库中提供由默认中断函数调用的用户中断，如下例

   ```C++
   //Prototype
   void CSL_UART_IRQHandler(UART_HandleTypeDef* cuart);

   //Call
   void UART0_IRQHandler(void)
   {
       CSL_UART_IRQHandler(&cuart0);
   }
   ```

   若使用此种方式，则用户只需重定义各种中断回调函数即可，若不使用，请自定义中断函数

3. 用户配置  

   驱动库中提供`KinetisKE_csl_config.h` 头文件，用户可根据需要自定义某些配置，具体内容请见文件

4. 外设初始化

   针对每一个外设均提供控制句柄，用户只需定义句柄然后调用初始化函数即可

#### 使用说明

1. 工程文件

   该库目前支持两种IDE，分别为Keil MDK和IAR Workbench for ARM，不过在Github Release中，只提供基于IAR的工程模板

   使用IAR时，需要添加以下宏定义到Processer中（必须）

   ```C++
   //标准C语言库支持
   _DLIB_FILE_DESCRIPTOR
   //MCU型号指定，该宏定义见于KinetisKE_Common.h
   CSL_KEAZ128xxx4
   //若使用数学库，请添加此定义
   ARM_MATH_CM0PLUS
   //外部时钟频率，大小自定，默认为16MHz，若无此定义则默认8MHz，见system_keaz1284.h
   CPU_XTAL_CLK_HZ=16000000
   ```

   若使用MDK，则除第一个宏以外，其他的都要添加

2. 裁剪

   该驱动库是可裁剪的，如不需要某些部分，则可不添加此驱动到工程中，但是此时需对`KinetisKE_csl_inc.h`做出修改，注释掉相应头文件以免报错

   上述头文件可以在main文件中包含，但是不建议在用户外设的初始化文件中包含，以免代码体积过大

3. 参数检查

   若要使用参数检查，请在Processer中添加`USE_FULL_ASSERT`，此时参数检查会启用，如果用户库函数中输入了非法参数则会生成相应警告

4. 其他便捷操作

   驱动库中提供一些对寄存器的便捷操作，位于`KinetisKE_csl_def.h`中，请有需要的用户自行查阅

#### 关于通信接口

1. 轮询模式

   通信接口的控制句柄中会提供指向收发缓冲区的指针，用户不需要按值传递参数即可实现收发，值得注意的是，轮询模式下的收发函数，是真收发函数，发送和接收的操作均在此函数中完成

2. 中断模式(IT)

   原理类似轮询模式，但是收发函数（后缀为IT）是中断配置函数，用户不必手动开启中断控制位，但需要手动配置中断优先级，在此类函数中，函数完成配置后，即等待中断生成，中断生成后自动收发，若使用驱动库提供的中断方式，则在发生不可预知行为时，会调用相应中断回到完成中断，此后回到主函数中继续循环直至下次中断被配置

#### 支持的外设及其帮助

+ [Clock](./peripherals/clock.md)
+ [Flash](./peripherals/ftmre.md)
+ [FlexTimer](./peripherals/ftm.md)
+ [GPIO/FGPIO](./peripherals/gpio.md)
+ [IRQ](./peripherals/irq.md)
+ [PMC](./peripherals/pmc.md)
+ [PWT](./peripherals/pwt.md)
+ [UART](./peripherals/uart.md)
+ [WDOG](./peripherals/wdog.md)

#### 其他说明

+ [错误列表BugList](./others/buglist.md)





Copyright &copy; Yangtze University EE Stark Zhang, All Rights Reserved 12.2017

Copyright &copy; 长江大学 电子信息学院 张璞 保留所有权利  2017.12


