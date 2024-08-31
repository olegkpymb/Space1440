/*!
 * \file main.h
 * \brief Определения
 * \author Крутяков Олег
 */

#ifndef _MAIN_H
#define _MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stm32g4xx.h"

#ifndef TRUE
#define TRUE                                        (1)
#endif

#ifndef FALSE
#define FALSE                                       (0)
#endif

typedef char BOOL;

// Результат работы
typedef ErrorStatus Result_t;

// Функциональное состояние
typedef FunctionalState State_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
