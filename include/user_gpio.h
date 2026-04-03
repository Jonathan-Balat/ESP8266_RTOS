#ifndef __USER_LED_H__
#define __USER_LED_H__

#include "esp_common.h"
#include "user_common.h"
#include "gpio.h"

/********** Macros **********/
#define GPIO_OUTPUT_SET_INV(gpio_num, bit_val)  (GPIO_OUTPUT_SET(gpio_num, (!bit_val))) /* Inverted the use of GPIO_OUTPUT_SET due to LED appearing LOW when trying to set as HIGH */
#define LED_PIN (2) // GPIO2 is used for the LED

#define DELAY_1000_MS (1000 / portTICK_RATE_MS) /* 1000 ms delay for FreeRTOS */
#define DELAY_MS(x_ms) (x_ms / portTICK_RATE_MS) 


typedef enum
{
    PIN_DISABLE = 0,
    PIN_ENABLE = 1
} gpio_state_t;

/********** Prototypes **********/
void gpio_init(void);
void gpio_pin_set_output(uint16_t pins_bitp);
void gpio_pin_set_input(uint16_t pins_bitp);
void gpio_pin_set(uint16_t pins_bitp);
void gpio_pin_clear(uint16_t pins_bitp);

#endif /* __USER_LED_H__ */