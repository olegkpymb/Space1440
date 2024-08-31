/*!
 * \file gpio.c
 * \brief Драйвер выводов
 * \author Крутяков Олег
 */

#include "gpio.h"

// Макрос создания структуры устройства входа по умолчанию
#define GPI_DEVICE(port, pin) \
{ \
    ((port << 16) | pin), \
    gpiPinsel, \
    NULL, \
    NULL, \
    NULL, \
    gpiPin \
}

// Макрос создания структуры устройства выхода по умолчанию
#define GPO_DEVICE(port, pin) \
{ \
    ((port << 16) | pin), \
    gpoPinsel, \
    NULL, \
    gpoSet, \
    gpoClr, \
    gpoPin \
}

// Типы выводов
enum
{
    GPIO_A = 0,
    GPIO_B,
    GPIO_C,
    GPIO_D,
    GPIO_E,
    GPIO_F,
    GPIO_G,
    GPIO_H,
    GPIO_I,
    GPIO_J,
    GPIO_K,
    GPIO_COUNT
};

Result_t gpiPinsel(uint32_t number, const void * config)
{
    uint16_t port;
    uint16_t pin;
    uint32_t base;
    GPIO_InitTypeDef init = {0};

    // Вычисление номера порта и выводов
    port = number >> 16;
    pin = number & GPIO_PIN_All;

    // Вычисление базового адреса порта
    base = AHB2PERIPH_BASE + port * 0x0400U;

    // Инициализация вывода
    init.Pin = pin;
    init.Mode = GPIO_MODE_INPUT;
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init((GPIO_TypeDef *) base, &init);

    return SUCCESS;
}

int32_t gpiPin(uint32_t number)
{
    uint16_t port;
    uint16_t pin;
    uint32_t base;
    int32_t state;

    // Вычисление номера порта и выводов
    port = number >> 16;
    pin = number & GPIO_PIN_All;

    // Вычисление базового адреса порта
    base = AHB2PERIPH_BASE + port * 0x0400U;

    // Считывание значения
    state = (HAL_GPIO_ReadPin((GPIO_TypeDef *) base, pin) == GPIO_PIN_SET) ? 1 : 0;

    return state;
}

Result_t gpoPinsel(uint32_t number, const void * config)
{
    uint16_t port;
    uint16_t pin;
    uint32_t base;
    GPIO_InitTypeDef init = {0};

    // Вычисление номера порта и выводов
    port = number >> 16;
    pin = number & GPIO_PIN_All;

    // Вычисление базового адреса порта
    base = AHB2PERIPH_BASE + port * 0x0400U;

    // Инициализация вывода
    init.Pin = pin;
    init.Mode = GPIO_MODE_OUTPUT_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init((GPIO_TypeDef *) base, &init);

    return SUCCESS;
}

Result_t gpoSet(uint32_t number)
{
    uint16_t port;
    uint16_t pin;
    uint32_t base;

    // Вычисление номера порта и выводов
    port = number >> 16;
    pin = number & GPIO_PIN_All;

    // Вычисление базового адреса порта
    base = AHB2PERIPH_BASE + port * 0x0400U;

    // Установка значения
    HAL_GPIO_WritePin((GPIO_TypeDef *) base, pin, GPIO_PIN_SET);

    return SUCCESS;
}

Result_t gpoClr(uint32_t number)
{
    uint16_t port;
    uint16_t pin;
    uint32_t base;

    // Вычисление номера порта и выводов
    port = number >> 16;
    pin = number & GPIO_PIN_All;

    // Вычисление базового адреса порта
    base = AHB2PERIPH_BASE + port * 0x0400U;

    // Установка значения
    HAL_GPIO_WritePin((GPIO_TypeDef *) base, pin, GPIO_PIN_RESET);

    return SUCCESS;
}

int32_t gpoPin(uint32_t number)
{
    uint16_t port;
    uint16_t pin;
    uint32_t base;
    int32_t state;

    // Вычисление номера порта и выводов
    port = number >> 16;
    pin = number & GPIO_PIN_All;

    // Вычисление базового адреса порта
    base = AHB2PERIPH_BASE + port * 0x0400U;

    // Считывание значения
    state = (HAL_GPIO_ReadPin((GPIO_TypeDef *) base, pin) == GPIO_PIN_SET) ? 1 : 0;

    return state;
}

// Вывод управления светодиодом
const GPIODevice_t GpioLed = GPO_DEVICE(GPIO_B, GPIO_PIN_8);

Result_t gpioInit(void)
{
    // Включение тактирования
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Вывод светодиода
    pinsel(&GpioLed, NULL);
    set(&GpioLed);

    return SUCCESS;
}

Result_t pinsel(const GPIODevice_t * device, const void * config)
{
    Result_t res;

    if(device == NULL)
    {
        return ERROR;
    }

    if(device->pinsel == NULL)
    {
        return ERROR;
    }

    res = (*(device->pinsel))(device->number, config);

    return res;
}

Result_t set(const GPIODevice_t * device)
{
    Result_t res;

    if(device == NULL)
    {
        return ERROR;
    }

    if(device->set == NULL)
    {
        return ERROR;
    }

    res = (*(device->set))(device->number);

    return res;
}

Result_t clr(const GPIODevice_t * device)
{
    Result_t res;

    if(device == NULL)
    {
        return ERROR;
    }

    if(device->clr == NULL)
    {
        return ERROR;
    }

    res = (*(device->clr))(device->number);

    return res;
}

int32_t pin(const GPIODevice_t * device)
{
    int32_t res;

    if(device == NULL)
    {
        return 0;
    }

    if(device->pin == NULL)
    {
        return 0;
    }

    res = (*(device->pin))(device->number);

    return res;
}
