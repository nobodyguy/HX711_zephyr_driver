/*
 * Copyright (c) 2020 George Gkinis
 * Copyright (c) 2022 Jan Gnip
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DRIVERS_SENSOR_HX711_H_
#define ZEPHYR_DRIVERS_SENSOR_HX711_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#ifdef CONFIG_HX711_ENABLE_MEDIAN_FILTER
#include "filters/median.h"
#endif

#ifdef CONFIG_HX711_ENABLE_EMA_FILTER
#include "filters/ema.h"
#endif

/* Additional custom attributes */
enum hx711_attribute {
	HX711_SENSOR_ATTR_SLOPE = SENSOR_ATTR_PRIV_START,
	HX711_SENSOR_ATTR_GAIN  = SENSOR_ATTR_PRIV_START + 1,
};

enum hx711_channel {
	HX711_SENSOR_CHAN_WEIGHT = SENSOR_CHAN_PRIV_START,
};

enum hx711_gain {
	HX711_GAIN_128X = 1,
	HX711_GAIN_32X,
	HX711_GAIN_64X,
};

enum hx711_rate {
	HX711_RATE_10HZ,
	HX711_RATE_80HZ,
};

enum hx711_power {
	HX711_POWER_ON,
	HX711_POWER_OFF,
};

struct hx711_data {
	const struct device *dev;
	const struct device *dout_gpio;
	const struct device *sck_gpio;
	const struct device *rate_gpio;
	struct gpio_callback dout_gpio_cb;
	struct k_sem dout_sem;

	int32_t reading;

	int offset;
	struct sensor_value slope;
	char gain;
	enum hx711_rate rate;
	enum hx711_power power;
#if defined(CONFIG_HX711_ENABLE_MEDIAN_FILTER) || defined(CONFIG_HX711_ENABLE_EMA_FILTER)
	struct k_mutex filter_lock;
	int32_t reading_unfiltered;
#endif
#ifdef CONFIG_HX711_ENABLE_MEDIAN_FILTER
	median_filter_t median_filter;
#endif
#ifdef CONFIG_HX711_ENABLE_EMA_FILTER
	ema_filter_t ema_filter;
#endif
};

struct hx711_config {
	gpio_pin_t dout_pin;
	const struct device *dout_ctrl;
	gpio_dt_flags_t dout_flags;

	gpio_pin_t sck_pin;
	const struct device *sck_ctrl;
	gpio_dt_flags_t sck_flags;

	gpio_pin_t rate_pin;
	const struct device *rate_ctrl;
	gpio_dt_flags_t rate_flags;
};

/**
 * @brief Zero the HX711.
 *
 * @param dev Pointer to the hx711 device structure
 * @param readings Number of readings to get average offset.
 *        5~10 readings should be enough, although more are allowed.
 * @retval The offset value
 *
 */
int avia_hx711_tare(const struct device *dev, uint8_t readings);

/**
 * @brief Callibrate the HX711.
 *
 * Given a target value of a known weight the slope gets calculated.
 * This is actually unit agnostic.
 * If the target weight is given in grams, lb, Kg or any other weight unit,
 * the slope will be calculated accordingly.
 *
 * @param dev Pointer to the hx711 device structure
 * @param target Target weight in grams.
 *        If target is represented in another unit (lb, oz, Kg) then the
 *        value returned by sensor_channel_get() will represent that unit.
 * @param readings Number of readings to take for calibration.
 *        5~10 readings should be enough, although more are allowed.
 * @retval The slope value
 *
 */
struct sensor_value avia_hx711_calibrate(const struct device *dev,
					 uint32_t target,
					 uint8_t readings);

/**
 * @brief Set the HX711 power.
 *
 * @param dev Pointer to the hx711 device structure
 * @param power one of HX711_POWER_OFF, HX711_POWER_ON
 * @retval The current power state or ENOTSUP if an invalid value pow is given
 *
 */
int avia_hx711_power(const struct device *dev, enum hx711_power power);

#ifdef __cplusplus
}
#endif

#endif
