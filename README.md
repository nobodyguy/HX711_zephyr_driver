# Out of tree HX711 sensor driver module for Zephyr RTOS
*HX711 is a precision 24-bit analog-to-digital converter (ADC) designed for weigh scale applications.*

## Supported Zephyr versions
* 2.7.0 LTS2 Release (October 2021)
## Usage
### Module installation
Add this project to your `west.yml` manifest:
```yaml
- name: HX711
  path: modules/HX711
  revision: main
  url: https://github.com/nobodyguy/HX711_zephyr_driver
```

So your projects should look something like this:
```yaml
manifest:
  projects:
    - name: zephyr
      url: https://github.com/zephyrproject-rtos/zephyr
      revision: v2.7-branch
      import: true
    - name: HX711
      path: modules/HX711
      revision: main
      url: https://github.com/nobodyguy/HX711_zephyr_driver
```

This will import the driver and allow you to use it in your code.

Additionally make sure that you run `west update` when you've added this entry to your `west.yml`.

### Driver configuration
Enable sensor driver subsystem and HX711 driver by adding these entries to your `prj.conf`:
```ini
CONFIG_SENSOR=y
CONFIG_HX711=y
```

Define HX711 in your board `.overlay` like this example:
```dts
/{
	hx711 {
		compatible = "avia,hx711";
		status = "okay";
		label = "HX711";
		dout-gpios = <&gpio0 26 (GPIO_ACTIVE_HIGH | GPIO_PULL_UP) >;
		sck-gpios = <&gpio0 27 GPIO_ACTIVE_HIGH>;
		rate-gpios = <&gpio0 2 GPIO_ACTIVE_HIGH>;
	};
};
```

### Driver usage
```c
#include <device.h>
#include <drivers/sensor.h>
#include <sensor/hx711/hx711.h>
#include <logging/log.h>
#include <stddef.h>
#include <sys/util.h>
#include <zephyr/types.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *hx711;

void set_rate(enum hx711_rate rate)
{
	static struct sensor_value rate_val;

	rate_val.val1 = rate;
	sensor_attr_set(hx711,
			HX711_SENSOR_CHAN_WEIGHT,
			HX711_SENSOR_ATTR_SAMPLING_FREQUENCY,
			&rate_val);
}

void measure(void)
{
	static struct sensor_value weight;
	int ret;

	ret = sensor_sample_fetch(hx711);
	if (ret != 0) {
		LOG_ERR("Cannot take measurement: %d  ", ret);
	} else {
		sensor_channel_get(hx711, HX711_SENSOR_CHAN_WEIGHT, &weight);
		LOG_INF("Weight: %d.%06d", weight.val1, weight.val2);
	}
}

void main(void)
{
	int calibration_weight = 100; // grams
	hx711 = DEVICE_DT_GET(DT_ALIAS(hx711_sensor));
	__ASSERT(hx711 == NULL, "Failed to get device binding");

	LOG_INF("Device is %p, name is %s", hx711, hx711->name);
	LOG_INF("Calculating offset...");
	avia_hx711_tare(hx711, 5);

	LOG_INF("Waiting for known weight of %d grams...",
		calibration_weight);

	for (int i = 5; i >= 0; i--) {
		LOG_INF(" %d..", i);
		k_msleep(1000);
	}

	LOG_INF("Calculating slope...");
	avia_hx711_calibrate(hx711, calibration_weight, 5);

	while (true) {
		k_msleep(1000);
		LOG_INF("== Test measure ==");
		LOG_INF("= Setting sampling rate to 10Hz.");
		set_rate(HX711_RATE_10HZ);
		measure();

		k_msleep(1000);
		LOG_INF("= Setting sampling rate to 80Hz.");
		set_rate(HX711_RATE_80HZ);
		measure();
	}
}
```