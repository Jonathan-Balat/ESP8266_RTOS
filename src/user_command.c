#include "user_command.h"


static uint32_t cmd_compute_sum(const uint8_t *data_buffer);
static void cmd_blink(const uint8_t *data_buffer);


/**
 * @brief Command Frame Structure Definition
 * 
 * The command frame follows a standard protocol with a fixed header and variable payload.
 * 
 * @details Frame Layout:
 *   - **Byte[0]**: Start Byte (0xBD) - Frame delimiter
 *   - **Byte[1]**: Command ID - Identifies the command type
 *   - **Byte[2+]**: Payload - Structure varies based on Command ID
 * 
 * @section Command ID Reference
 * 
 * @subsection cmd_sum SUM (CID 0)
 *   - Byte[2]: Payload Length - Number of data bytes
 *   - Byte[3..N]: Payload Data - Variable length data
 * 
 * @subsection cmd_blink BLINK (CID 1)
 *   - Byte[2]: Payload Length (Fixed: 3 bytes)
 *   - Byte[3]: ON Duration (milliseconds)
 *   - Byte[4]: OFF Duration (milliseconds)
 *   - Byte[5]: Repeat Count - Number of blink cycles
 */
bool command_function(uint8_t cmd_id, uint8_t *cmd_data)
{
    // Process Command ID
    switch(cmd_id)
    {
        case CMD_SUM: // Data Sum
            cmd_compute_sum(cmd_data);
            break;

        case CMD_BLINK: // Blink Command
            cmd_blink(cmd_data);
            break;

        default:
            printf("Unknown Command ID: %d\n", cmd_id);
            break;
    }
}

/***** Command Functions *****/
static uint32_t cmd_compute_sum(const uint8_t *data_buffer)
{
    uint8_t payload_len = data_buffer[2];
    uint8_t idx = 0;
    uint32_t sum = 0;

    for (idx = 0; idx < payload_len; idx++)
    {
        sum += data_buffer[3 + idx];
    }

    // printf("Data Sum: %d\n", sum);
    return sum;
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