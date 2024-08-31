/*!
 * \file uart.h
 * \brief Драйвер UART
 * \author Крутяков Олег
 */

#ifndef _UART_H
#define _UART_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "main.h"

/*!
 * \brief Инициализация драйвера
 * \return Результат выполнения функции
 */
Result_t uartInit(void);

// Дескрипторы портов
extern UART_HandleTypeDef huart2;

// Переопределяемые функции
extern void uart2RxCpltCallback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
