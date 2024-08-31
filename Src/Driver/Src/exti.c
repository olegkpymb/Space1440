/*!
 * \file exti.c
 * \brief Драйвер внешних прерываний
 * \author Крутяков Олег
 */

#include "exti.h"

Result_t extiInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**EXTI0 GPIO Configuration
    PA0    ------> EXTI0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* EXTI0 interrupt priority */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);

    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    return SUCCESS;
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_0)
    {
        exti0Callback();
    }
}
