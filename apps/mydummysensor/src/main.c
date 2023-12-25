/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>

int main(void)
{
	int ret;
	const struct device *sensor;

	printk("Zephyr My Dummy Sensor Application \n");

	sensor = DEVICE_DT_GET(DT_NODELABEL(mydummysensor0));
	if (!device_is_ready(sensor)) {
		printk("Sensor not ready\n");
		return 0;
	}

	while (1) {
		struct sensor_value val;

		ret = sensor_sample_fetch(sensor);
		if (ret < 0) {
			printk("Could not fetch sample (%d)\n", ret);
			return 0;
		}

		ret = sensor_channel_get(sensor, SENSOR_CHAN_PROX, &val);
		if (ret < 0) {
			printk("Could not get sample (%d)\n", ret);
			return 0;
		}

		printk("Sensor value: %d\n", val.val1);

		k_sleep(K_MSEC(1000));
	}

	return 0;
}