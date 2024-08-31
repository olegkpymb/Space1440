/*!
 * \file gpio.h
 * \brief Драйвер выводов
 * \author Крутяков Олег
 */

#ifndef _GPIO_H
#define _GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "main.h"

// Структура объекта вывода
typedef struct _GPIODevice_t
{
    uint32_t number;                                            ///< Номера порта и вывода
    Result_t (*pinsel)(uint32_t number, const void * config);   ///< Настройка функции вывода
    Result_t (*dir)(uint32_t number, int32_t out);              ///< Установка направления вывода
    Result_t (*set)(uint32_t number);                           ///< Установка состояния вывода в 1
    Result_t (*clr)(uint32_t number);                           ///< Установка состояния вывода в 0
    int32_t (*pin)(uint32_t number);                            ///< Чтение состояния вывода
} GPIODevice_t;

/*!
 * \brief Инициализация драйвера
 * \return Результат выполнения функции
 */
Result_t gpioInit(void);

/*!
 * \brief Конфигурация вывода
 * \param device - Указатель на вывод
 * \param config - Указатель на конфигурацию
 * \return Результат выполнения функции
 */
Result_t pinsel(const GPIODevice_t * device, const void * config);

/*!
 * \brief Установка направления вывода
 * \param device - Указатель на вывод
 * \param out - Направление вывода: 1 - выход, 0 - вход
 * \return Результат выполнения функции
 */
Result_t dir(const GPIODevice_t * device, int32_t out);

/*!
 * \brief Установка состояния вывода в 1
 * \param device - Указатель на вывод
 * \return Результат выполнения функции
 */
Result_t set(const GPIODevice_t * device);

/*!
 * \brief Установка состояния вывода в 0
 * \param device - Указатель на вывод
 * \return Результат выполнения функции
 */
Result_t clr(const GPIODevice_t * device);

/*!
 * \brief Чтение состояния вывода
 * \param device - Указатель на вывод
 * \return Состояние на входе
 */
int32_t pin(const GPIODevice_t * device);

// Вывод управления светодиодом
extern const GPIODevice_t GpioLed;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // _GPIO_H
