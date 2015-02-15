#ifndef SENSORS_H__
#define SENSORS_H__

#include "systick.h"

enum {SENSOR_VALUE_INT, SENSOR_VALUE_FLOAT};
enum {
    SENSOR_FLAG_INITIALIZED = 1, 
};

struct sensor_t {
    char model[8];
    char input_name[16]; //"temperature", "humidity", etc.
    char unit[4]; //"C", "Pa", etc.
    uint8_t value_type;
    uint8_t flags;
    uint8_t reserved[2];
    union {
        int value_int;
        float value_float;
    } value;
    SysTick_t latest_sample;
    SysTick_t sample_rate;
    bool (*driver_init)(struct sensor_t *);
    bool (*measure)(struct sensor_t *);
};

extern const int sensors_count;
extern struct sensor_t *sensors[];

#endif
