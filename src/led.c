#include "led.h"

void init_gpio(void)
{
    // Initialize GPIO2 pin as output
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    gpio_output_conf(0, (1<<LED_PIN), 0, (1<<LED_PIN)); 

    /* Start with LED off (GPIO2 low) */
    GPIO_OUTPUT_SET_INV(LED_PIN, 0);
}

void blink(uint16_t t_high, uint16_t t_low)
{
    GPIO_OUTPUT_SET_INV(LED_PIN, LED_ON);
    vTaskDelay(DELAY_MS(t_high));

    GPIO_OUTPUT_SET_INV(LED_PIN, LED_OFF);
    vTaskDelay(DELAY_MS(t_low)); 
}

void led_status(led_status_t status)
{
    /* Time in milliseconds */
    switch (status)
    {
        case LED_NORMAL:
            blink(1000, 1000);
            break;

        case LED_ERROR:
            blink(100, 50);
            blink(100, 1750);
            break;

        default:
            /* Do nothing - Invalid case */
            break;
    }
}
