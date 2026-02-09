#include "middleware/data_parser.h"
#include "user_led.h"
#include "user_command.h"
bool parse_start_byte(const uint8_t byte)
{
    return (byte == START_BYTE);
}

void parse_data(const uint8_t* data, size_t length)
{
    uint8_t payload_len = 0;
    uint8_t idx = 0;
    uint32_t sum = 0;

    cmd_frame_t *cmd_frame = (cmd_frame_t *)data;

    // Null pointer check
    if (data == NULL)
    {
        printf("Data pointer is NULL\n");
        return;
    }

    // Not enough data to parse
    if (length < 2)
    {
        printf("Data length too short to parse\n");
        return;
    }

    // Validate Start Byte
    if (cmd_frame->start_byte != START_BYTE)
    {
        printf("Invalid Start Byte: 0x%02X\n", cmd_frame->start_byte);
        return;
    }

    command_function(cmd_frame->command_id, cmd_frame->payload);
}

