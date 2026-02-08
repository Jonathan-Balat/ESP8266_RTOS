#include "user_led.h"

static volatile bool semaphore = false;

void led_in_use(uint8_t val)
{
    // When already in use, wait.
    while(semaphore)
    {
        vTaskDelay(10 / portTICK_RATE_MS); // Wait 10ms before checking again
        taskYIELD();
    }

    semaphore = true;
}

void led_yield(uint8_t val)
{
    semaphore = false;
}

void init_led(void)
{
    /* Initialize GPIO2 pin as output */
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    gpio_output_conf(0, (1<<LED_PIN), 0, (1<<LED_PIN)); 

    /* Start with LED off (GPIO2 low) */
    GPIO_OUTPUT_SET_INV(LED_PIN, 0);
}

void blink(uint16_t t_high, uint16_t t_low)
{
    if (t_high > 0)
    {
        GPIO_OUTPUT_SET_INV(LED_PIN, LED_ON);
        vTaskDelay(DELAY_MS(t_high));
    }

    if (t_low > 0)
    {
        GPIO_OUTPUT_SET_INV(LED_PIN, LED_OFF);
        vTaskDelay(DELAY_MS(t_low)); 
    }
}

