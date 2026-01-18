#ifndef __DEVICE_STATUS_H__
#define __DEVICE_STATUS_H__


typedef enum
{
    STAT_BEGIN = 0,
    STAT_NORMAL_DISC = STAT_BEGIN,
    STAT_NORMAL_CONN,
    STAT_ERROR,
    STAT_MAX
} app_status_t;

/* Function Declarations */
void run_device_status(void);
void set_device_status(app_status_t new_status);
app_status_t get_device_status(void);

#endif /* __DEVICE_STATUS_H__ */