/*!
 * \file hal.h
 * \brief Драйвер HAL
 * \author Крутяков Олег
 */

#ifndef _HAL_H
#define _HAL_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "main.h"

/*!
 * \brief Инициализация драйвера
 * \return Результат выполнения функции
 */
Result_t halInit(void);

/*!
 * \brief Перезагрузка
 */
void halSystemReset(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
