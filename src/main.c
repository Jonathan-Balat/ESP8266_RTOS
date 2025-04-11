#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"


/********** MACROS **********/
#ifndef WIFI_SSID
#define WIFI_SSID "DefaultSSID"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "DefaultPassword"
#endif

/********** GLOBALS **********/
led_status_t gStatus = LED_ERROR;

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

/* Wi-Fi Event Handler */
void wifi_event_handler(System_Event_t* event)
{
    switch (event->event_id)
    {
        case EVENT_STAMODE_CONNECTED:
            printf("Wi-Fi Connected to SSID: %s\n", event->event_info.connected.ssid);
            gStatus = LED_NORMAL; // Update LED to indicate normal operation
            break;

        case EVENT_STAMODE_DISCONNECTED:
            printf("Wi-Fi Disconnected. Reason: %d\n", event->event_info.disconnected.reason);
            gStatus = LED_ERROR; // Update LED to indicate an error
            break;

        case EVENT_STAMODE_GOT_IP:
            printf("IP Address Acquired: " IPSTR "\n", IP2STR(&event->event_info.got_ip.ip));
            gStatus = LED_NORMAL; // Update LED to indicate normal operation
            break;

        default:
            printf("Unhandled Wi-Fi Event: %d\n", event->event_id);
            break;
    }
}

void init_wifi(void)
{
    /* Set Wi-Fi mode to Station */
    wifi_set_opmode(STATION_MODE);

    /* Configure Wi-Fi connection */
    struct station_config stationConf;
    memset(&stationConf, 0, sizeof(stationConf));
    strcpy((char*)stationConf.ssid, WIFI_SSID);
    strcpy((char*)stationConf.password, WIFI_PASS);

    /* Set the station configuration */
    wifi_station_set_config(&stationConf);

    /* Register Wi-Fi event handler */
    wifi_set_event_handler_cb(wifi_event_handler);
}

void task_wifi_connect(void* ignore)
{
    /* Connect to the Wi-Fi network */
    wifi_station_connect();

    /* Wait for connection */
    while (wifi_station_get_connect_status() != STATION_GOT_IP)
    {
        printf("Connecting to Wi-Fi...\n");
        vTaskDelay(1000 / portTICK_RATE_MS); // Delay for 1 second
    }

    printf("Connected to Wi-Fi. IP address acquired.\n");
    
    // DEBUG
    gStatus = LED_NORMAL; // Change LED status to normal

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
    init_gpio();
    init_wifi();

    /* Start tasks */
    xTaskCreate(&task_blink, "startup", 2048, NULL, 2, NULL);
    xTaskCreate(&task_wifi_connect, "wifi_task", 2048, NULL, 1, NULL);
}

