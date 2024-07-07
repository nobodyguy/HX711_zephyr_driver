# SPDX-License-Identifier: Apache-2.0

zephyr_library()

zephyr_library_sources_ifdef(CONFIG_HX711_ENABLE_MEDIAN_FILTER median.c)
zephyr_library_sources_ifdef(CONFIG_HX711_ENABLE_EMA_FILTER ema.c)
