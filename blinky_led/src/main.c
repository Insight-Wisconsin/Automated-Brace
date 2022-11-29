/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_NODE DT_ALIAS(gpio2)

// create gpio device from rpi_pico.overlay
static const struct gpio_dt_spec gpio2 = GPIO_DT_SPEC_GET(GPIO_NODE, gpios);

void main(void){
	// config the LED pin as output
	gpio_pin_configure_dt(&gpio2, GPIO_OUTPUT_ACTIVE);

	// Loop forever blinking the light
	while (1) {
		gpio_pin_toggle_dt(&gpio2);
		k_msleep(1000);
	}
}

