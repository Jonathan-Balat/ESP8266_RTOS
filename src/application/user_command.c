#include "application/user_command.h"


static uint32_t cmd_compute_sum(const uint8_t *data_buffer);
static uint8_t cmd_blink(const uint8_t *data_buffer);

// TODO: Move to common utility file.
static void word_to_bytes(uint32_t value, uint8_t *bytes_out)
{
    bytes_out[0] = value & 0xFF;
    bytes_out[1] = (value >> 8) & 0xFF;
    bytes_out[2] = (value >> 16) & 0xFF;
    bytes_out[3] = (value >> 24) & 0xFF;
}

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
    uint8_t *return_data = cmd_data;
    uint32_t sum = 0;

    // Process Command ID
    switch(cmd_id)
    {
        case CMD_SUM:
            sum = cmd_compute_sum(cmd_data);

            cmd_data[0] = 4; // response payload length
            word_to_bytes(sum, &cmd_data[1]);
            break;

        case CMD_BLINK:
            cmd_data[1] = cmd_blink(cmd_data); // Returns 1 for success, 0 for failure
            cmd_data[0] = 1; // response payload length
            break;

        default:
            printf("Unknown Command ID: %d\n", cmd_id);
            break;
    }
}

/***** Command Functions *****/
static uint32_t cmd_compute_sum(const uint8_t *data_buffer)
{
    uint8_t payload_len = data_buffer[0];
    uint8_t idx = 0;
    uint32_t sum = 0;

    for (idx = 0; idx < payload_len; idx++)
    {
        sum += data_buffer[1 + idx];
    }

    return sum;
}

static uint8_t cmd_blink(const uint8_t *data_buffer)
{
    uint8_t ret_val = 0; // Assume failure.
    uint8_t idx = 0;
    uint8_t payload_len = data_buffer[0];
    uint8_t on_duration = data_buffer[1];
    uint8_t off_duration = data_buffer[2];
    uint8_t repeat_count = data_buffer[3];

    if (payload_len == 3)
    {
        led_claim();
        for (idx = 0; idx < repeat_count; idx++)
        {
            blink(on_duration, off_duration);
        }
        led_yield();

        ret_val = 1; // Indicate success.
    }

    return ret_val;
}