#include "user_wifi.h"


/* Wi-Fi Event Handler */
void wifi_event_handler(System_Event_t* event)
{
    extern int gStatus; // TODO: We don't like this design approach, eliminate in the future.

    switch (event->event_id)
    {
        case EVENT_STAMODE_CONNECTED:
            printf("Wi-Fi Connected to SSID: %s\n", event->event_info.connected.ssid);
            gStatus = STAT_NORMAL_CONN; 
            break;

        case EVENT_STAMODE_DISCONNECTED:
            printf("Wi-Fi Disconnected. Reason: %d\n", event->event_info.disconnected.reason);
            gStatus = STAT_NORMAL_DISC; 
            break;

        case EVENT_STAMODE_GOT_IP:
            printf("IP Address Acquired: " IPSTR "\n", IP2STR(&event->event_info.got_ip.ip));
            gStatus = STAT_NORMAL_CONN; 
            break;

        default:
            printf("Unhandled Wi-Fi Event: %d\n", event->event_id);
            gStatus = STAT_ERROR; 
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