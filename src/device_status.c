#include "device_status.h"
#include "user_led.h"


static app_status_t current_status = STAT_ERROR;


void run_device_status(void)
{
    led_in_use(0x0);
    /* Time in milliseconds */
    switch (current_status)
    {
        case STAT_NORMAL_DISC:
            blink(1000, 1000);
            break;

        case STAT_NORMAL_CONN:
            blink(100, 1900);
            break;

        case STAT_ERROR:
            blink(100, 50);
            blink(100, 1750);
            break;

        default:
            /* Do nothing - Invalid case */
            break;
    }

    led_yield(0x0);
}

void set_device_status(app_status_t new_status)
{
    if ((STAT_BEGIN <= new_status) && (new_status < STAT_MAX))
    {
        current_status = new_status;
    }
}

app_status_t get_device_status(void)
{
    return current_status;
}