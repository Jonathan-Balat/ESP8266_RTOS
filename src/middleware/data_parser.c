#include "middleware/data_parser.h"
#include "application/user_command.h"

#define BUFFER_SIZE (200)

/* TODO:
    - DataBuffer functions should be simple -- Store and Retrieve.
    - All validation steps and determining the start and length should be done before calling 
      the store/retrieve functions.
*/

/* Data buffer will be circular
    - It will loop from end to start when reached the end of the buffer.
    - Data is handled as FIFO, so the oldest data will be overwritten when new data comes in.
    - There will be variables that will keep track of the current position in the buffer and the total length of valid data stored.
*/
static uint8_t dataBuff[BUFFER_SIZE] = {0}; // Buffer to store incoming data, size can be adjusted as needed
static uint8_t buffReadPos = 0; // The earliest active data position in the buffer to read from.
static uint8_t buffWritePos = 0; // The last data position in the buffer to write to
static uint8_t stored_available_data = 0; // Total length of valid data stored in the buffer

bool store_data(const uint8_t* data, size_t length)
{
    // Check for null pointer and buffer overflow
    if (data == NULL)
    {
        printf("[ERROR]: Invalid data \n");
        return false;
    }

    // Copy data into buffer
    memcpy(dataBuff, data, length);

    // Update buffer positions and active data length
    buffWritePos = length % BUFFER_SIZE;
    stored_available_data += length;
    return true;
}

uint8_t get_next_data_size()
{
    uint8_t data_size = 0;

    if (stored_available_data == 0)
    {
        printf("[ERROR]: No data available to retrieve.");
        return 0;
    }

    if (dataBuff[buffReadPos] != START_BYTE)
    {
        printf("[ERROR]: Current buffer position shows no start byte, data parse in error.");
        return 0;
    }

    // Accounts for start byte and command ID then payload sizes.
    data_size = 2 + dataBuff[(buffReadPos + 2) % BUFFER_SIZE];

    if (stored_available_data < data_size)
    {
        printf("[ERROR]: Data being retrieved is bigger than actual stored. Data parser in error.");
        return 0;
    }

    return data_size;
}

bool retrieve_data(uint8_t* out_buffer, size_t buffer_size)
{
    uint8_t data_size = get_next_data_size();

    if (data_size == 0)
    {
        printf("[ERROR]: No valid data size to retrieve.");
        return false;
    }

    if (out_buffer == NULL)
    {
        printf("[ERROR]: Output buffer is null\n");
        return false;
    }

    if (buffer_size < data_size)
    {
        printf("[ERROR]: Output buffer is too small.");
        return false;
    }

    // Copy data from internal buffer to output buffer
    memcpy(out_buffer, &dataBuff[buffReadPos], data_size);

    // Subtract data retrieved from buffer.
    stored_available_data -= data_size;

    // Move data buffer position to next available data.
    buffReadPos = (buffReadPos + data_size)% BUFFER_SIZE; 
    
    return true;
}


/* 
    * @brief Receives the data from the source and stores into a buffer.
    * 
    * @param data Pointer to the received data buffer
    * @param length Length of the received data in bytes
    * 
    * @details This function parses the incoming data buffer, validates the start byte.
*/
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

