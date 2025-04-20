#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "user_led.h"
#include "user_uart.h"
#include "user_wifi.h"


/********** MACROS **********/

/********** GLOBALS **********/
app_status_t gStatus = STAT_ERROR;

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}


void task_blink(void* ignore)
{  
    do
    {   
        /* Blink LED based on status */ 
        led_status(gStatus);
    } while (true);

    /* Delete the task after it has run once */
    vTaskDelete(NULL);
}


void task_wifi_connect(void* ignore)
{
    printf("Running Wifi Connect Task...\n");

    /* Connect to the Wi-Fi network */
    wifi_station_connect();

    /* Wait for connection */
    while (wifi_station_get_connect_status() != STATION_GOT_IP)
    {
        printf("Connecting to Wi-Fi...\n");
        vTaskDelay(1000 / portTICK_RATE_MS); // Delay for 1 second
    }

    printf("Connected to Wi-Fi. IP address acquired.\n");
   
    init_client(IP_ADDR, PORT_ADDR);
    
    vTaskDelete(NULL); // Delete the task after it has run once
}


/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    /* Initialize Modules */
    init_uart();
    init_led();
    init_wifi();

    /* Start tasks */
    xTaskCreate(&task_blink, "startup", 2048, NULL, 2, NULL);
    xTaskCreate(&task_wifi_connect, "wifi_task", 2048, NULL, 1, NULL);
}

