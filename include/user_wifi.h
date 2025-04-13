#ifndef __USER_WIFI_H__
#define __USER_WIFI_H__

#include "esp_common.h"
#include "user_common.h"


/********** MACROS **********/
#ifndef WIFI_SSID
#define WIFI_SSID "DefaultSSID"
#endif

#ifndef WIFI_PASS
#define WIFI_PASS "DefaultPassword"
#endif

/********** PROTOTYPES **********/

void wifi_event_handler(System_Event_t* event);
void init_wifi(void);


#endif /* __USER_WIFI_H__ */