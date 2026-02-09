#ifndef __DATA_PARSER_H__
#define __DATA_PARSER_H__

#include "esp_common.h"

typedef enum
{
    STAT_BEGIN = 0,
    STAT_NORMAL_DISC = STAT_BEGIN,
    STAT_NORMAL_CONN,
    STAT_ERROR,
    STAT_MAX
} app_status_t;

/* Function Declarations */
void parse_data(const uint8_t* data, size_t length);

#endif /* __DATA_PARSER_H__ */