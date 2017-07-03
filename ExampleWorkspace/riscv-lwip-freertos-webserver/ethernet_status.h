

#ifndef ETHERNET_STATUS_H
#define ETHERNET_STATUS_H

#include "core_tse.h"

typedef struct ethernet_status
{
    tse_speed_t     speed;
    uint8_t         duplex_mode;
} ethernet_status_t;

#endif
