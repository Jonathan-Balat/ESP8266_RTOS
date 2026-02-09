#ifndef __USER_COMMAND_H__
#define __USER_COMMAND_H__

#include "middleware/data_parser.h"

typedef struct 
{
    uint8_t start_byte: 8;
    uint8_t command_id: 8;
    uint8_t payload[];
}cmd_frame_t;

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
