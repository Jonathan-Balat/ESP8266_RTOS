#include "user_wifi.h"

/********** HELPER FUNCTIONS **********/
void copy_ip_str(uint8_t* ip_byte_arr, const char *ip_str)
{
    uint32_t ip = ipaddr_addr(ip_str); // Convert string to 32-bit integer

    /* Extract and assign each bytes */
    ip_byte_arr[3] = (ip >> 24) & 0xFF;
    ip_byte_arr[2] = (ip >> 16) & 0xFF;
    ip_byte_arr[1] = (ip >> 8) & 0xFF; 
    ip_byte_arr[0] = ip & 0xFF;        
}

/********** WIFI FUNCTIONS **********/
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

        case EVENT_SOFTAPMODE_STACONNECTED:
            printf("Device Connected to Network: %02x:%02x:%02x:%02x:%02x:%02x, AID= %d\n", MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);
            // gStatus = STAT_NORMAL_CONN; 
            break;

        case EVENT_SOFTAPMODE_STADISCONNECTED:
            printf("Device Disconnected from Network: %02x:%02x:%02x:%02x:%02x:%02x, AID= %d\n", MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);
            // gStatus = STAT_NORMAL_CONN; 
            break;

        case EVENT_SOFTAPMODE_PROBEREQRECVED:
            printf("Probed by %02x:%02x:%02x:%02x:%02x:%02x - %d\n", MAC2STR(event->event_info.ap_probereqrecved.mac), event->event_info.ap_probereqrecved.rssi);
            break;

        default:
            printf("Unhandled Wi-Fi Event: %d\n", event->event_id);
            gStatus = STAT_ERROR; 
            break;
    }
}

void init_wifi_host(void)
{
    /* Set Wi-Fi mode to Station */
    wifi_set_opmode(SOFTAP_MODE);

    /* Configure Wi-Fi connection */
    struct softap_config softapConf;
    memset(&softapConf, 0, sizeof(softapConf));
    softapConf.authmode = AUTH_WPA2_PSK;
    softapConf.max_connection = 3; // NOTE: Unsure why claims to be full when not set explicitly.
    softapConf.ssid_hidden = 1;
    strcpy((char*)softapConf.ssid, HOST_SSID);
    strcpy((char*)softapConf.password, HOST_PASS);

    /* Set the station configuration */
    wifi_softap_set_config(&softapConf);

    /* Register Wi-Fi event handler */
    wifi_set_event_handler_cb(wifi_event_handler);
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
