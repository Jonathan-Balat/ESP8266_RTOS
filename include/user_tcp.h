#ifndef __USER_TCP_H__
#define __USER_TCP_H__

#include "esp_common.h"
#include "espconn.h"

/* Function Decalarations */
void init_tcp_client(void);
void set_tcp_instance(struct espconn **conn);
void get_tcp_instance(struct espconn **conn);
void tcp_conn_cb(void *arg);
void tcp_discon_cb(void *arg);
void tcp_recon_cb(void *arg, sint8 err);
void tcp_recv_cb(void *arg, char *pdata, unsigned short len);
void tcp_sent_cb(void *arg);


#endif /* __USER_TCP_H__ */