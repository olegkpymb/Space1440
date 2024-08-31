/*!
 * \file hal.c
 * \brief Драйвер HAL
 * \author Крутяков Олег
 */

#include "hal.h"

Result_t halInit(void)
{
    if(HAL_Init() != HAL_OK)
    {
        return ERROR;
    }

    return SUCCESS;
}

void halSystemReset(void)
{
    HAL_NVIC_SystemReset();
}

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{  
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_PWREx_DisableUCPDDeadBattery();
}
