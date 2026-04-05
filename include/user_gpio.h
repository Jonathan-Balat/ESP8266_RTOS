#ifndef __USER_LED_H__
#define __USER_LED_H__

#include "esp_common.h"
#include "user_common.h"
#include "gpio.h"

/********** Macros **********/
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
void gpio_pin_mode_output_set(uint16_t pins_bitp);
void gpio_pin_mode_input_set(uint16_t pins_bitp);
void gpio_pin_value_set(uint16_t pins_bitp);
void gpio_pin_value_clear(uint16_t pins_bitp);

#endif /* __USER_LED_H__ */