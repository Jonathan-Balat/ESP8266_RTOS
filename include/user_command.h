#ifndef __USER_COMMAND_H__
#define __USER_COMMAND_H__

#include "esp_common.h"

typedef enum
{
    CMD_SUM = 0,
    CMD_BLINK,
    CMD_MAX
} command_id_t;


#define START_BYTE (0xBD)

#define IDX_START_BYTE (0)
#define CMD_BYTE_IDX (1)


bool command_function(uint8_t cmd_id, uint8_t *cmd_data);

#endif /* __USER_COMMAND_H__ */
