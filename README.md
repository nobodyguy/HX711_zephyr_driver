# Out of tree HX711 sensor driver module for Zephyr RTOS
*HX711 is a precision 24-bit analog-to-digital converter (ADC) designed for weigh scale applications.*
## Usage
### Module installation
Add this project to your main manifest west.yml:
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
### Driver usage
```c
#include <drivers/sensor.h>
#include <sensor/hx711/hx711.h>

// TODO
```