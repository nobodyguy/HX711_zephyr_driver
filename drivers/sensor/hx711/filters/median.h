#ifndef ZEPHYR_DRIVERS_SENSOR_HX711_MEDIAN_FILTER_H_
#define ZEPHYR_DRIVERS_SENSOR_HX711_MEDIAN_FILTER_H_
#include <stdint.h>

typedef struct
{
    int32_t window[CONFIG_HX711_MEDIAN_FILTER_WINDOW_SIZE]; // Array to hold the values in the window
    int index;                   // Current index in the window
} median_filter_t;

void median_filter_init(median_filter_t *f, int32_t initial_value);
int32_t median_filter_update(median_filter_t *f, int32_t measurement);

#endif /* ZEPHYR_DRIVERS_SENSOR_HX711_MEDIAN_FILTER_H_ */