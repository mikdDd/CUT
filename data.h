#ifndef DATA_H
#define DATA_H

#include <inttypes.h>
typedef struct Data
{   
    uint32_t user;
    uint32_t nice;
    uint32_t system;
    uint32_t idle;
    uint32_t iowait;
    uint32_t irq;
    uint32_t softirq;
    uint32_t steal;
    uint32_t guest;
    uint32_t guest_nice;
} Data;

#endif

