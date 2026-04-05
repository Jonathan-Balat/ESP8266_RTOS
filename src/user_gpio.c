#include "user_gpio.h"
#include "freertos/FreeRTOS.h"


#define GPIO_PINS_MAX   (16)

/* GPIO PinMux Register mapping */
typedef struct
{
    uint32_t register_base: 32;
    uint8_t  func_num: 8; // TODO: Determine if _packed_ is usable, and if ESP doesn't take extra steps during arithmetic.
} gpio_pinmux_t;

static gpio_pinmux_t _gpio_pinmux[GPIO_PINS_MAX] = \
{
    {.register_base = PERIPHS_IO_MUX_GPIO0_U, .func_num = FUNC_GPIO0},
    {.register_base = PERIPHS_IO_MUX_U0TXD_U, .func_num = FUNC_GPIO1},
    {.register_base = PERIPHS_IO_MUX_GPIO2_U, .func_num = FUNC_GPIO2},
    {.register_base = PERIPHS_IO_MUX_U0RXD_U, .func_num = FUNC_GPIO3},
    {.register_base = PERIPHS_IO_MUX_GPIO4_U, .func_num = FUNC_GPIO4},
    {.register_base = PERIPHS_IO_MUX_GPIO5_U, .func_num = FUNC_GPIO5},
    {.register_base = PERIPHS_IO_MUX_SD_CLK_U, .func_num = FUNC_GPIO6}, // RESERVED
    {.register_base = PERIPHS_IO_MUX_SD_DATA0_U, .func_num = FUNC_GPIO7}, // RESERVED
    {.register_base = PERIPHS_IO_MUX_SD_DATA1_U, .func_num = FUNC_GPIO8}, // RESERVED
    {.register_base = PERIPHS_IO_MUX_SD_DATA2_U, .func_num = FUNC_GPIO9}, // RESERVED
    {.register_base = PERIPHS_IO_MUX_SD_DATA3_U, .func_num = FUNC_GPIO10}, // RESERVED
    {.register_base = PERIPHS_IO_MUX_SD_CMD_U, .func_num = FUNC_GPIO11}, // RESERVED
    {.register_base = PERIPHS_IO_MUX_MTDI_U, .func_num = FUNC_GPIO12},
    {.register_base = PERIPHS_IO_MUX_MTCK_U, .func_num = FUNC_GPIO13},
    {.register_base = PERIPHS_IO_MUX_MTMS_U, .func_num = FUNC_GPIO14},
    {.register_base = PERIPHS_IO_MUX_MTDO_U, .func_num = FUNC_GPIO15}
};


/**
 * @brief GPIO configuration and management for ESP8266 RTOS project
 * 
 * @details This module handles GPIO pin initialization.
 *          All GPIO pins are available for use except GPIO 6-11, which are reserved
 *          for flash memory interface and should not be used for general purpose I/O.
 *          Initialization is done for all relevant pins and are set 
 */
void gpio_init(void)
{
    uint8_t pin_idx;

    for (pin_idx = 0; pin_idx < GPIO_PINS_MAX; pin_idx++)
    {
        // Skips over the reserved GPIO pins.
        if ((5<pin_idx) && (pin_idx<12))
        {
            continue;
        }

        gpio_pinmux_t pin = _gpio_pinmux[pin_idx];

        // Selects GPIO Functionality from Pin Mux
        PIN_FUNC_SELECT(pin.register_base, pin.func_num);
        
        // Clears Output to LOW, and sets as an input pin.
        gpio_output_conf(0, (1<<pin_idx), 0, (1<<pin_idx)); 
    }
}

void gpio_pin_reconfigure(uint8_t pin_num, uint8_t pin_func)
{
    PIN_FUNC_SELECT(_gpio_pinmux[pin_num].register_base, pin_func);
}

void gpio_pin_mode_output_set(uint16_t pins_bitp)
{
    gpio_output_conf(0, 0, pins_bitp, 0); 
}

void gpio_pin_mode_input_set(uint16_t pins_bitp)
{
    gpio_output_conf(0, 0, 0, pins_bitp); 
}

void gpio_pin_value_set(uint16_t pins_bitp)
{
    gpio_output_conf(pins_bitp, 0, 0, 0); 
}

void gpio_pin_value_clear(uint16_t pins_bitp)
{
    gpio_output_conf(0, pins_bitp, 0, 0); 
}
