#ifndef __USER_WIFI_H__
#define __USER_WIFI_H__

#include "esp_common.h"
#include "user_common.h"
#include "espconn.h"



/********** MACROS **********/
#ifndef WIFI_SSID
#define WIFI_SSID "DefaultSSID"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "DefaultPassword"
#endif

#define HOST_SSID "ESPNET"
#define HOST_PASS "Espnet_0001"

/********** PROTOTYPES **********/

void wifi_event_handler(System_Event_t* event);
void init_wifi(void);
void init_wifi_host(void);


#endif /* __USER_WIFI_H__ */