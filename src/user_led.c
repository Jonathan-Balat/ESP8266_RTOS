#include "user_led.h"
#include "user_gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"  // This header contains semaphore definitions


static xSemaphoreHandle led_semaphore = NULL;

void led_claim(void)
{
    xSemaphoreTake(led_semaphore, portMAX_DELAY);
}

void led_yield(void)
{
    xSemaphoreGive(led_semaphore);
}

void init_led(void)
{
    /* Initialize GPIO2 pin as output */
    gpio_pin_mode_output_set(1<<LED_PIN);

    /* Start with LED off (GPIO2 High) */
    gpio_pin_value_set(1<<LED_PIN);

    vSemaphoreCreateBinary(led_semaphore);
}

void blink(uint16_t t_high, uint16_t t_low)
{
    if (t_high > 0)
    {
        gpio_pin_value_clear(1<<LED_PIN); // Clear GPIO2 to Low to turn LED on
        vTaskDelay(DELAY_MS(t_high));
    }

    if (t_low > 0)
    {
        gpio_pin_value_set(1<<LED_PIN); // Set GPIO2 High to turn LED off
        vTaskDelay(DELAY_MS(t_low)); 
    }
}

