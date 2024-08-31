/*!
 * \file main.c
 * \brief Централизованный диспетчер по синхронизации системных часов
 * \author Крутяков Олег
 */

#include "main.h"
#include "hal.h"
#include "clock.h"
#include "uart.h"
#include "timer.h"
#include "it.h"
#include "gpio.h"
#include "exti.h"

// Размер сообщения с временем RTC
#define RTC_MESSAGE_SIZE            (8)

// Структура дескриптора программы
typedef struct _Handle_t
{
    volatile State_t syncState;                             ///< Состояние получения сигнала синхронизации
    volatile uint32_t syncTimestamp;                        ///< Микросекундная метка времени получения сигнала синхронизации
    volatile uint8_t rtcMessage[RTC_MESSAGE_SIZE];          ///< Буффер для хранения принимаемых байт сообщения с временем RTC
    volatile uint32_t rtcMessageSize;                       ///< Число принятых байт в буффер хранения сообщения с временем RTC
    uint8_t rcvByte;                                        ///< Служебная переменная для организации приема байт
} Handle_t;

// Дескриптор программы
static Handle_t Handle =
{
    DISABLE,
    0,
    {0, 0, 0, 0, 0, 0, 0, 0},
    0,
    0
};

/*!
 * \brief Обработчик ошибок программ
 */
static void SystemErrorHandler(void)
{
    while(1) 
    {
    }
}

/*!
 * \brief Инициализация программы
 * \return Результат выполнения
 */
static Result_t SystemInitHandler(void)
{
    Result_t res;

    // Инициализация бибилиотеки HAL
    res = halInit();
    if(res != SUCCESS)
    {
        return res;
    }

    // Инициализация тактирования
    res = clockInit();
    if(res != SUCCESS)
    {
        return res;
    }

    // Инициализация устройств ввода/вывода
    res = gpioInit();
    if(res != SUCCESS)
    {
        return res;
    }

    // Инициализация таймера
    res = timerInit();
    if(res != SUCCESS)
    {
        return res;
    }

    // Инициализация UART
    res = uartInit();
    if(res != SUCCESS)
    {
        return res;
    }

    // Инициализация внешних прерываний
    res = extiInit();
    if(res != SUCCESS)
    {
        return res;
    }

    return res;
}

/*!
 * \brief Функция микросекундных задержек, блокирующая
 * \param time - Устанавливаемое время в микросекундах
 */
void SystemDelayUs(uint32_t delay)
{
    uint32_t timestamp;
    
    // Обновление метки времени входа в функцию
    timestamp = __HAL_TIM_GetCounter(&htim2);
	// Проверка изменения текущего времени на время задержки 
    while((__HAL_TIM_GetCounter(&htim2) - timestamp) < delay);
}

/*!
 * \brief Установка системного времени
 * \param time - Устанавливаемое время в микросекундах
 */
static void SystemSetTime(uint64_t time)
{
    // Отладочный вариант, переключение светодиода
    (pin(&GpioLed) != 0) ? 
        clr(&GpioLed) : 
        set(&GpioLed);
}

/*!
 * \brief Точка входа в программу
 */
int main(void)
{
    uint32_t usExtraTime;
    uint64_t usResultTime;

    // Инициализация программы
    if(SystemInitHandler() != SUCCESS)
    {
        SystemErrorHandler();
    }

    // Запуск приема нового байта с информационного канала связи
    if(HAL_UART_Receive_IT(&huart2, &Handle.rcvByte, 1) != HAL_OK)
    {
        SystemErrorHandler();
    }

    // Выполнение централизованного диспетчера
    while(1)
    {
        __disable_irq();
		// Проверка на получение сигнала синхронизации
        if(Handle.syncState == ENABLE)
        {
			// Определение интервала времени от момента получения сигнала синхронизации
            usExtraTime = __HAL_TIM_GetCounter(&htim2) - Handle.syncTimestamp;

			// Если сигнал синхронизации не обработан за время 1 с + 15%, то пропускаем его
            if(usExtraTime > 1150000)
            {
                Handle.syncState = DISABLE;
            }
            else
            {
                // Проверка получения всего сообщения в буффер
                if(Handle.rtcMessageSize == RTC_MESSAGE_SIZE)
                {
					// Считаем, что байты приходят по информационному каналу в формате Big Endian, конвертируем их в 64-битную переменную
                    usResultTime = 0;
                    for(uint8_t i = 0; i < 8; i++)
                    {
                        usResultTime <<= 8;
                        usResultTime |= Handle.rtcMessage[i];
                    }
					// Корректировка принятого значения на пройденный интервал времени от момента получения сигнала синхронизации
                    usResultTime += usExtraTime;

					// Установка системного времени
                    SystemSetTime(usResultTime);

                    // Сигнал синхронизации отработан
                    Handle.syncState = DISABLE;
                }
            }
        }
        __enable_irq();

        // Задержка
        SystemDelayUs(1);
    }
}

/*!
 * \brief Обработчик вызова приема байта по каналу связи
 */
void uart2RxCpltCallback(void)
{
	// Проверка получения сигнала синхронизации
    if(Handle.syncState == ENABLE)
    {
		// Проверка наличия места в памяти буффера
        if(Handle.rtcMessageSize < RTC_MESSAGE_SIZE)
        {
			// Добавление полученного байта в буффер
            Handle.rtcMessage[Handle.rtcMessageSize] = Handle.rcvByte;
            Handle.rtcMessageSize++;
        }
    }

	// Запуск приема нового байта с информационного канала связи
    HAL_UART_Receive_IT(&huart2, &Handle.rcvByte, 1);
}

/*!
 * \brief Обработчик вызова приема сигнала синхронизации
 */
void exti0Callback(void)
{
    // Обновление метки времени получения сигнала синхронизации
    Handle.syncTimestamp = __HAL_TIM_GetCounter(&htim2);
	// Сброс в 0 числа полученных байт в буффер
    Handle.rtcMessageSize = 0;
	// Состояние сигнала синхронизации получен
    Handle.syncState = ENABLE;
}
