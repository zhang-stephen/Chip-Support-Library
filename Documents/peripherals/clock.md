# Clock Module in CSL

#### KEA中的时钟系统

1. KEA的时钟系统最大值为48MHz，关于时钟的生成和来源请参见[参考手册](https://www.nxp.com/docs/en/reference-manual/KEA128RM.pdf)，驱动库中的时钟驱动关联KEA系列MCU的两个外设模块，分别是ICS和OSC，完全支持六种时钟工作模式
2. 除对时钟系统的基本配置外，提供了TIM Clock Source的配置，以便定义定时器的时钟源大小，还有对总线时钟输出的配置，获取各关键时钟值的函数等等
3. 使用方法见例程