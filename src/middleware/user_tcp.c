#include "middleware/user_tcp.h"

static struct espconn *tcp_cfg = NULL;

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
    tcp_cfg->proto.tcp->local_port = 3333; // espconn_port();

    /* Register Callbacks */
    // Only connect is needed to have an initial connection, the others
    // are registered so the IP/Port info is available in the callbacks.
    espconn_regist_connectcb(tcp_cfg, &tcp_conn_cb);
    espconn_regist_reconcb(tcp_cfg, tcp_recon_cb);

    /* Accept configuration and start listening */
    sint8 res = espconn_accept(tcp_cfg);
    if (res != ESPCONN_OK)
    {
        printf("TCP Client: espconn_accept failed with error code %d\n", res);
        if (tcp_cfg->proto.tcp) os_free(tcp_cfg->proto.tcp);
        os_free(tcp_cfg);
        vTaskDelete(NULL);
        return;
    }

    /* Store the TCP instance for later use */
    set_tcp_instance(&tcp_cfg);
    // TODO: Where do we call cleanup to delete stored instance?

    printf("TCP Client: Listening on port %d\n", tcp_cfg->proto.tcp->local_port);
}

/********** Setter/Getter Functions **********/
void set_tcp_instance(struct espconn **conn)
{
    tcp_cfg = *conn;
}

void get_tcp_instance(struct espconn **conn)
{
    *conn = tcp_cfg;
}

/********** API Functions **********/
void send_tcp_message(const char* message)
{
    struct espconn *conn;
    get_tcp_instance(&conn);
    
    if (conn && conn->state == ESPCONN_CONNECT)
    {
        espconn_sent(conn, (uint8*)message, strlen(message));
    }
}

/********** Callback Functions **********/
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

    parse_data((const uint8_t *)pdata, len);

    /* TODO: Optional, send ACK */
    espconn_sent(conn, pdata, 7);
    // send_tcp_message(pdata);
}

void tcp_sent_cb(void* arg)
{
    struct espconn *conn = (struct espconn *)arg;
    printf("TCP Client: data sent (conn=%p)\n", conn);
}