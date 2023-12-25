/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_mydummysensor

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(mydummysensor, CONFIG_SENSOR_LOG_LEVEL);

struct mydummysensor_data {
	int state;
};

struct mydummysensor_config {
	int dummy;
};

static int mydummysensor_sample_fetch(const struct device *dev,
				      enum sensor_channel chan)
{
	printk("my dummy sensor sample fetch!");
	const struct mydummysensor_config *config = dev->config;
	struct mydummysensor_data *data = dev->data;	
	
    if (data->state == 1) {
        data->state = 1;
    }
    else {
        data->state = 0;
    }
    
	return 0;
}

static int mydummysensor_channel_get(const struct device *dev,
				     enum sensor_channel chan,
				     struct sensor_value *val)
{
	struct mydummysensor_data *data = dev->data;
    printk("my dummy sensor channel get!");
	if (chan != SENSOR_CHAN_PROX) {
		return -ENOTSUP;
	}

	val->val1 = data->state;

	return 0;
}

static const struct sensor_driver_api mydummysensor_api = {
	.sample_fetch = &mydummysensor_sample_fetch,
	.channel_get = &mydummysensor_channel_get,
};

static int mydummysensor_init(const struct device *dev)
{
	printk("my dummy sensor init!");
	const struct mydummysensor_config *config = dev->config;

	return 0;
}

#define MYDUMMYSENSOR_INIT(i)						       					\
	static struct mydummysensor_data mydummysensor_data_##i;	      	 	\
									       									\
	static const struct mydummysensor_config mydummysensor_config_##i = {  	\
		.dummy = 0,		       			\
	};																		\
									       									\
	DEVICE_DT_INST_DEFINE(i, mydummysensor_init, NULL,		       			\
			      &mydummysensor_data_##i,			       					\
			      &mydummysensor_config_##i, POST_KERNEL,	       			\
			      CONFIG_SENSOR_INIT_PRIORITY, &mydummysensor_api);

DT_INST_FOREACH_STATUS_OKAY(MYDUMMYSENSOR_INIT)