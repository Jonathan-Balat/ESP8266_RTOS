#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "user_led.h"
#include "user_uart.h"
#include "user_wifi.h"
#include "application/device_status.h"

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

/********** RTOS TASKS **********/
void task_blink(void* ignore)
{  
    do 
    {   
        /* Blink LED based on status */ 
        run_device_status();        
        taskYIELD();
    } while (true);

    vTaskDelete(NULL);
}


void task_wifi_application(void* ignore)
{
    printf("Running Wifi Connect Task...\n");

    /* Connect to the Wi-Fi network */
    wifi_station_connect();

    /* Wait for connection */
    do 
    {
        printf("Connecting to Wi-Fi...\n");
        vTaskDelay(DELAY_MS(5000)); // Delay for 5 seconds
    } while (wifi_station_get_connect_status() != STATION_GOT_IP);
    printf("Connected to Wi-Fi.\n");
    
    printf("Running Wifi Client TCP Listener...\n");
    init_wifi_tcp_client();
    printf("TCP Listener Running.\n");

    /* Keep the task alive while server runs; delete when Wi-Fi goes down */
    while (true)
    {
        vTaskDelay(DELAY_MS(10000));
    }
    
    vTaskDelete(NULL);
}

void task_com_handler(void* ignore)
{
    printf("Running UART Comms Task...\n");   

    while (true)
    {
        uart_process_rx();

        /* Yield to other tasks */
        vTaskDelay(DELAY_MS(10));
    }

    vTaskDelete(NULL);
}


/********** APPLICATION CODE **********/


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
    xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);
    xTaskCreate(&task_wifi_application, "wifi_task", 2048, NULL, 2, NULL);
    xTaskCreate(&task_com_handler, "uart_comms_task", 2048, NULL, 2, NULL);
    // xTaskCreate(&task_wifi_host_server, "wifi_task", 2048, NULL, 1, NULL);
}

