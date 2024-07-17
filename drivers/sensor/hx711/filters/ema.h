#ifndef ZEPHYR_DRIVERS_SENSOR_HX711_EMA_FILTER_H_
#define ZEPHYR_DRIVERS_SENSOR_HX711_EMA_FILTER_H_

#include <stdint.h>

typedef struct
{
    float alpha;
    double out;
} ema_filter_t;

void ema_filter_init(ema_filter_t *f, int alpha, double initial_value);
void ema_filter_reset(ema_filter_t *f, double initial_value);
int32_t ema_filter_update(ema_filter_t *f, int32_t measurement);

#endif /* ZEPHYR_DRIVERS_SENSOR_HX711_EMA_FILTER_H_ */