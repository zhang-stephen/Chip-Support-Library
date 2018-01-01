# CSL库中 已知Bug列表

#### @2017.12.22 (v0.2.5) 

1. UART中断函数`CSL_UART_IRQHandler(UART_HandleTypeDef* cuart)`无法正常进入RDRF中断，以致无法使用中断函数
2. PWT脉宽测不准，正在调试中
3. GPIO/FGPIO在时钟处于FBI/FBILP模式时可能失效

#### @2018.01.02

1. SPI用于主机收发的函数`CSL_SPI_TransmitReceive()`不可用，其他函数正常，其中关于SPI中断部分正在调试
2. FTM支持尚不完全
3. PWT和UART中断的Bug均已修复
4. GPIO/FGPIO未解决，请尽量避免FBI/FBILP的使用

---




若有其他Bug，欢迎提出issues！！！