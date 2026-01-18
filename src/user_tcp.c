#include "user_tcp.h"


void init_tcp_client(void)
{    
    struct espconn *tcp_cfg = (struct espconn *) os_zalloc(sizeof(struct espconn));

    /* Allocation guard */
    if (!tcp_cfg) 
    {
        printf("TCP config structure allocation failed\n");
        vTaskDelete(NULL);
        return;
    }

    /* Configure initial TCP state */
    tcp_cfg->type = ESPCONN_TCP;
    tcp_cfg->state = ESPCONN_NONE;
    
    tcp_cfg->proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));

    /* Allocation guard */
    if (!tcp_cfg->proto.tcp)
    {
        printf("TCP protocol allocation failed\n");
        os_free(tcp_cfg);
        vTaskDelete(NULL);
        return;
    }
    
    /* Configure Local Port */
    tcp_cfg->proto.tcp->local_port = espconn_port();

    /* Register Callbacks */
    // Only connect is needed to have an initial connection, the others
    // are registered so the IP/Port info is available in the callbacks.
    espconn_regist_connectcb(tcp_cfg, &tcp_conn_cb);
    espconn_regist_reconcb(tcp_cfg, tcp_recon_cb);

    sint8 res = espconn_accept(tcp_cfg);
    if (res != ESPCONN_OK)
    {
        printf("TCP Client: espconn_accept failed with error code %d\n", res);
        if (tcp_cfg->proto.tcp) os_free(tcp_cfg->proto.tcp);
        os_free(tcp_cfg);
        vTaskDelete(NULL);
        return;
    }
    printf("TCP Client: Listening on port %d\n", tcp_cfg->proto.tcp->local_port);
}

void tcp_conn_cb(void *arg)
{
    struct espconn *conn = (struct espconn *)arg;
    printf("TCP Client: Connected to server (conn=%p)\n", conn);

    espconn_regist_disconcb(conn, &tcp_discon_cb);
    espconn_regist_recvcb(conn, &tcp_recv_cb);
    espconn_regist_sentcb(conn, &tcp_sent_cb);

    espconn_sent(conn, "Connected to ESP!", 18);
}

void tcp_recon_cb(void *arg, sint8 err)
{
    printf("TCP Client: Connection failed. Error code: %d\n", err);
}


void tcp_discon_cb(void *arg)
{
    struct espconn *conn = (struct espconn *)arg;
    printf("TCP Client: Disconnected from server (conn=%p)\n", conn);
}
void tcp_recv_cb(void *arg, char *pdata, unsigned short len)
{
    struct espconn *conn = (struct espconn *)arg;
    printf("TCP Client: received %d bytes: %s\n", len, pdata);

    /* TODO: Process data */

    /* TODO: Optional, send ACK */
}

void tcp_sent_cb(void* arg)
{
    struct espconn *conn = (struct espconn *)arg;
    printf("TCP Client: data sent (conn=%p)\n", conn);
}