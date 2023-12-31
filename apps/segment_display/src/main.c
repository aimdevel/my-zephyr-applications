/*
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file io_expander controll
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#define MAX_GPIOS 1<<7 - 1

static const struct device *const gpio_ex =
                DEVICE_DT_GET(DT_NODELABEL(gpio_expander_0));

int main(void)
{
	printk("start!\n");
	if (!device_is_ready(gpio_ex)) {
		return 0;
	}
	printk("device is ready!\n");

	for (int i=0;i<8;i++) {
		printk("gpio configure loop\n");
		int ret = gpio_pin_configure(gpio_ex, i, GPIO_OUTPUT | GPIO_OUTPUT_INIT_LOW);
	}

	k_msleep(10000);

	int gpio_n = 0;
	while (1) {
		printk("loop gpio_n=%d!\n",gpio_n);
		gpio_port_set_masked_raw(gpio_ex, 0x7f, gpio_n);
		//gpio_port_set_bits_raw(gpio_ex, gpio_n);
		k_msleep(1000);
		gpio_n++;
		if (gpio_n > MAX_GPIOS) {
			gpio_port_clear_bits_raw(gpio_ex, 0x7f);
			gpio_n = 0;
		}
	}
	return 0;
}
