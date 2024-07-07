#ifndef ZEPHYR_DRIVERS_SENSOR_HX711_MEDIAN_FILTER_H__EMA_FILTER_H_
#define ZEPHYR_DRIVERS_SENSOR_HX711_MEDIAN_FILTER_H__EMA_FILTER_H_

#include <stdint.h>

typedef struct
{
    float alpha;
    double out;
} ema_filter_t;

void ema_filter_init(ema_filter_t *f, int alpha);
int32_t ema_filter_update(ema_filter_t *f, int32_t measurement);

#endif /* ZEPHYR_DRIVERS_SENSOR_HX711_MEDIAN_FILTER_H__EMA_FILTER_H_ */