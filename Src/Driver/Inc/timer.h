/*!
 * \file timer.h
 * \brief Драйвер таймеров
 * \author Крутяков Олег
 */

#ifndef _TIMER_H
#define _TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "main.h"

/*!
 * \brief Инициализация драйвера
 * \return Результат выполнения функции
 */
Result_t timerInit(void);

// Дескрипторы таймеров
extern TIM_HandleTypeDef htim2;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
