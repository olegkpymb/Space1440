/*!
 * \file clock.h
 * \brief Драйвер тактирования
 * \author Крутяков Олег
 */

#ifndef _CLOCK_H
#define _CLOCK_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "main.h"

/*!
 * \brief Инициализация драйвера
 * \return Результат выполнения функции
 */
Result_t clockInit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
