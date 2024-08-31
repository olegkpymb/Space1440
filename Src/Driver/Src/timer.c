/*!
 * \file timer.c
 * \brief Драйвер таймеров
 * \author Крутяков Олег
 */

#include "timer.h"

// Дескриптор таймерa
TIM_HandleTypeDef htim2;

Result_t timerInit(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = (SystemCoreClock / 1000000) - 1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFFFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if(HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        return ERROR;
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if(HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        return ERROR;
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if(HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        return ERROR;
    }

    if(HAL_TIM_Base_Start(&htim2) != HAL_OK)
    {
        return ERROR;
    }

    return SUCCESS;
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef * tim_baseHandle)
{
    if(tim_baseHandle->Instance == TIM2)
    {
        /* TIM2 clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef * tim_baseHandle)
{
    if(tim_baseHandle->Instance == TIM2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM2_CLK_DISABLE();
    }
}
