#include "data_parser.h"

void parse_data(const uint8_t* data, size_t length)
{
    uint8_t payload_len = 0;
    uint8_t idx = 0;
    uint32_t sum = 0;
    /* Command Frame
        [0]      : Start Byte (0xBD)
        [1]      : Command ID
        Proceeding structure will vary on Command ID
        CID 0  = SUM
        [2]   : Payload Length (Big Endian)
        [N]   : Payload Data
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

    if (data[0] != 0xBD)
    {
        printf("Invalid Start Byte: 0x%02X\n", data[0]);
        return;
    }

    switch(data[1])
    {
        case 0: // Data Sum
            payload_len = data[2];

            for (idx = 0; idx < payload_len; idx++)
            {
                sum += data[3 + idx];
            }
            printf("Data Sum: %llu\n", sum);
            break;
    }
}