#ifndef __DATA_PARSER_H__
#define __DATA_PARSER_H__

#include "esp_common.h"


typedef struct 
{
    uint8_t start_byte: 8;
    uint8_t command_id: 8;
    uint8_t payload[];
}cmd_frame_t;


/* Function Declarations */
void parse_data(const uint8_t* data, size_t length);

#endif /* __DATA_PARSER_H__ */