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

/********** CALLBACK FUNCTIONS **********/
void client_sent_cb(void* arg)
{
    printf("Client sent data\n");
}

void client_recv_cb(void* arg, char *pdata, unsigned short len)
{
    printf("Client received data: %s\n", pdata);
}

void client_discon_cb(void* arg)
{
    printf("Client disconnected\n");
}

void client_conn_cb(void* arg)
{
    printf("Client connected\n");

    // Send data to the server
    const char *msg = "Hello from ESP8266 TCP Client!";
    struct espconn *conn = (struct espconn *)arg;
    espconn_sent(conn, (uint8_t *)msg, strlen(msg));
}

void client_recon_cb(void *arg, sint8 err)
{
    printf("Connection failed. Error code: %d\n", err);
}

//TODO: Move this to another file
/********** PROTOCOL FUNCTIONS **********/
void init_client(const char *server_ip, uint16_t server_port)
{
    sint8 result = 0;
    uint32_t local_port = espconn_port(); // Random local port - TODO: replace with specific port   
    struct espconn *client_cfg = (struct espconn *) os_zalloc(sizeof(struct espconn));

#ifdef USER_CLIENT_TCP

    /* Configure as TCP Client */
    client_cfg->type = ESPCONN_TCP;
    client_cfg->state = ESPCONN_NONE;

    client_cfg->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
    client_cfg->proto.tcp->local_port = local_port;
    client_cfg->proto.tcp->remote_port = server_port;
    
    /* Correctly set the Remote IP (Server IP) */
    copy_ip_str(client_cfg->proto.tcp->remote_ip, server_ip);

    /* Connect Callback functions */
    result |= espconn_regist_connectcb(client_cfg, &client_conn_cb);
    result |= espconn_regist_disconcb(client_cfg, &client_discon_cb);
    result |= espconn_regist_recvcb(client_cfg, &client_recv_cb);
    result |= espconn_regist_sentcb(client_cfg, &client_sent_cb);
    result |= espconn_regist_reconcb(client_cfg, client_recon_cb);

    if (result>0)
    {
        printf("Failed to register callbacks. Error code: %d\n", result);
        return;
    }

    result = espconn_connect(client_cfg);

    if (result>0)
    {
        printf("Connection failed. Error code: %d\n", result);
    }
    else
    {
        printf("Client initialized with IP: %s, Port: %d\n", server_ip, server_port);
    }


#endif /* USER_CLIENT_TCP */

#ifdef USER_CLIENT_UDP

    /* Select UDP protocol */
    client_cfg->type = ESPCONN_UDP;
    client_cfg->state = ESPCONN_NONE;

    /* Set UDP configuration */
    client_cfg->proto.udp = (esp_udp *)zalloc(sizeof(esp_udp));
    client_cfg->proto.udp->local_port = local_port;
    client_cfg->proto.udp->remote_port = server_port;

    /* Correctly set the Remote IP (Server IP) */
    copy_ip_str(client_cfg->proto.udp->remote_ip, server_ip);

    /* Register Receive Callback */
    // result |= espconn_regist_connectcb(client_cfg, &client_conn_cb); // TODO: Figure why it enters fault.
    // result |= espconn_regist_reconcb(client_cfg, client_recon_cb);

    result |= espconn_regist_disconcb(client_cfg, &client_discon_cb);
    result |= espconn_regist_recvcb(client_cfg, &client_recv_cb);
    result |= espconn_regist_sentcb(client_cfg, &client_sent_cb);

    if (result>0)
    {
        printf("Failed to register callbacks. Error code: %d\n", result);
        return;
    }

    /* Start UDP Connection */
    result = espconn_create(client_cfg);
    if (result>0)
    {
        printf("Failed to Connect to UDP Server. Error code: %d\n", result);
    }
    else
    {
        printf("Connected to server with IP: %d.%d.%d.%d, Port: %d\n", client_cfg->proto.udp->remote_ip[0],
                                                                       client_cfg->proto.udp->remote_ip[1],
                                                                       client_cfg->proto.udp->remote_ip[2],
                                                                       client_cfg->proto.udp->remote_ip[3], 
                                                                       client_cfg->proto.udp->remote_port);
        printf("Local Port: %d\n", client_cfg->proto.udp->local_port);
    }

    /* Send Initial Data */
    const char *msg = "Client Init";
    result = espconn_sendto(client_cfg, (uint8_t *)msg, strlen(msg));

    if (result>0)
    {
        printf("Failed send message. Error code: %d\n", result);
    }
    else
    {
        printf("Message sent to server: %s\n", msg);
    }

#endif /* USER_CLIENT_UDP */
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
