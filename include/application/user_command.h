#ifndef __USER_COMMAND_H__
#define __USER_COMMAND_H__

#include "esp_common.h"

typedef enum
{
    CMD_GPIO = 0,
    CMD_SUM,
    CMD_BLINK,
    CMD_MAX
} eCommandId;

typedef enum
{
    GPIO_CMD_CONF = 0,
    GPIO_CMD_OUT,
    GPIO_CMD_IN,
    GPIO_CMD_HI,
    GPIO_CMD_LO,
    GPIO_CMD_INT_SET,
    GPIO_CMD_INT_CLR,
    GPIO_CMD_READ
}eGpioCmd;


#define START_BYTE (0xBD)

#define IDX_START_BYTE (0)
#define CMD_BYTE_IDX (1)


bool command_function(uint8_t cmd_id, uint8_t *cmd_data);

#endif /* __USER_COMMAND_H__ */
