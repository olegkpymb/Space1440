/*!
 * \file exti.h
 * \brief Драйвер внешних прерываний
 * \author Крутяков Олег
 */

#ifndef _EXTI_H
#define _EXTI_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "main.h"

/*!
 * \brief Инициализация драйвера
 * \return Результат выполнения функции
 */
Result_t extiInit(void);

extern void exti0Callback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
