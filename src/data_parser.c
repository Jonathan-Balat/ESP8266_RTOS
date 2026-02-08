#include "data_parser.h"
#include "user_led.h"

#define START_BYTE (0xBD)
#define IDX_START_BYTE (0)


static void cmd_compute_sum(const uint8_t *data_buffer);
static void cmd_blink(const uint8_t *data_buffer);

bool parse_start_byte(const uint8_t byte)
{
    return (byte == START_BYTE);
}

void parse_data(const uint8_t* data, size_t length)
{
    uint8_t payload_len = 0;
    uint8_t idx = 0;
    uint32_t sum = 0;
    /* Command Frame
        [0]      : Start Byte (0xBD)
        [1]      : Command ID
        Proceeding structure will vary on Command ID
        CID 0 = SUM
        [2]   : Payload Length
        [N]   : Payload Data

        CID 1 = BLINK
        [2]  : Payload Length (Constant 3)
        [3]  : ON Duration (ms)
        [4]  : OFF Duration (ms)
        [5]  : Repeat Count
    */

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
    if (!parse_start_byte(data[IDX_START_BYTE]))
    {
        printf("Invalid Start Byte: 0x%02X\n", data[IDX_START_BYTE]);
        return;
    }

    // Process Command ID
    switch(data[1])
    {
        case 0: // Data Sum
            cmd_compute_sum(data);
            break;

        case 1: // Blink Command
            cmd_blink(data);
            break;
    }
}

/***** Command Functions *****/
static void cmd_compute_sum(const uint8_t *data_buffer)
{
    uint8_t payload_len = data_buffer[2];
    uint8_t idx = 0;
    uint32_t sum = 0;

    for (idx = 0; idx < payload_len; idx++)
    {
        sum += data_buffer[3 + idx];
    }

    printf("Data Sum: %d\n", sum);
}

static void cmd_blink(const uint8_t *data_buffer)
{
    uint8_t idx = 0;
    uint8_t payload_len = data_buffer[2];
    uint8_t on_duration = data_buffer[3];
    uint8_t off_duration = data_buffer[4];
    uint8_t repeat_count = data_buffer[5];

    if (payload_len != 3)
    {
        printf("Invalid payload length for BLINK command: %d\n", payload_len);
        return;
    }

    printf("Blink Command - ON: %d ms, OFF: %d ms, Repeat: %d times\n",
            on_duration, off_duration, repeat_count);
    
    led_claim();
    for (idx = 0; idx < repeat_count; idx++)
    {
        blink(on_duration, off_duration);
    }
    led_yield();
    
    send_tcp_message("Blinked!\n");
}