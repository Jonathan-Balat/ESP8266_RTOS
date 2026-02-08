#ifndef __USER_LED_H__
#define __USER_LED_H__

#include "esp_common.h"
#include "gpio.h"
#include "user_common.h"

/********** Macros **********/
#define GPIO_OUTPUT_SET_INV(gpio_num, bit_val)  (GPIO_OUTPUT_SET(gpio_num, (!bit_val))) /* Inverted the use of GPIO_OUTPUT_SET due to LED appearing LOW when trying to set as HIGH */
#define LED_PIN (2) // GPIO2 is used for the LED

#define DELAY_1000_MS (1000 / portTICK_RATE_MS) /* 1000 ms delay for FreeRTOS */
#define DELAY_MS(x_ms) (x_ms / portTICK_RATE_MS) 


typedef enum
{
    LED_OFF = 0,
    LED_ON = 1
} led_state_t;

/********** Prototypes **********/
void init_led(void);
void led_in_use(uint8_t val);
void led_yield(uint8_t val);
void blink(uint16_t t_high, uint16_t t_low);

#endif /* __USER_LED_H__ */