#ifndef __USER_COMMON_H__
#define __USER_COMMON_H__


typedef enum
{
  STAT_NORMAL_DISC = 0,
  STAT_NORMAL_CONN,
  STAT_ERROR
} app_status_t;

/* Only declare either one */
#define USER_CLIENT_TCP
// #define USER_CLIENT_UDP


#endif /* __USER_COMMON_H__ */