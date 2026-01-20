#ifndef __USER_WIFI_H__
#define __USER_WIFI_H__

#include "esp_common.h"
#include "user_common.h"
#include "espconn.h"
#include "device_status.h"



/********** MACROS **********/
#ifndef WIFI_SSID
#define WIFI_SSID "DefaultSSID"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "DefaultPassword"
#endif

#ifndef HOST_SSID
#define HOST_SSID "DefaultHostSSID"
#endif

#ifndef HOST_PASS
#define HOST_PASS "DefaultHostPassword"
#endif

/********** PROTOTYPES **********/

void wifi_event_handler(System_Event_t* event);
void init_wifi(void);
void init_wifi_host(void);


#endif /* __USER_WIFI_H__ */