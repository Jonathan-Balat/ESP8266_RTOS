#include "user_led.h"
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
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    gpio_output_conf(0, (1<<LED_PIN), 0, (1<<LED_PIN)); 

    /* Start with LED off (GPIO2 low) */
    GPIO_OUTPUT_SET_INV(LED_PIN, 0);

    vSemaphoreCreateBinary(led_semaphore);
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

