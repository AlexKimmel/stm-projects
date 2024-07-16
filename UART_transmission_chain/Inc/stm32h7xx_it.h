#ifndef __STM32H7XX_IT_H
#define __STM32H7XX_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

void SysTick_Handler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32H7XX_IT_H */