# CSL库中 已知Bug列表

#### @2017.12.22 (v0.2.5) 

1. UART中断函数`CSL_UART_IRQHandler(UART_HandleTypeDef* cuart)`无法正常进入RDRF中断，以致无法使用中断函数
2. PWT脉宽测不准，正在调试中
3. GPIO/FGPIO在时钟处于FBI/FBILP模式时可能失效



若有其他Bug，欢迎提出issues！！！