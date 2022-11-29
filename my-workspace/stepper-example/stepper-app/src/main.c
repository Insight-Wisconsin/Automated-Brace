/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_NODE_2 DT_ALIAS(gpio2)
#define GPIO_NODE_3 DT_ALIAS(gpio3)
#define GPIO_NODE_4 DT_ALIAS(gpio4)
#define GPIO_NODE_5 DT_ALIAS(gpio5)

// create gpio device from rpi_pico.overlay
static const struct gpio_dt_spec gpio2 = GPIO_DT_SPEC_GET(GPIO_NODE_2, gpios);
static const struct gpio_dt_spec gpio3 = GPIO_DT_SPEC_GET(GPIO_NODE_3, gpios);
static const struct gpio_dt_spec gpio4 = GPIO_DT_SPEC_GET(GPIO_NODE_4, gpios);
static const struct gpio_dt_spec gpio5 = GPIO_DT_SPEC_GET(GPIO_NODE_5, gpios);

void step_forward(int steps){
	for(int i = 0; i < steps; i++){
		// First step
		gpio_pin_set_dt(&gpio5, 0);
		gpio_pin_set_dt(&gpio2, 1);
		k_msleep(15);
		// Second step
		gpio_pin_set_dt(&gpio2, 0);
		gpio_pin_set_dt(&gpio3, 1);
		k_msleep(15);
		// Third step
		gpio_pin_set_dt(&gpio3, 0);
		gpio_pin_set_dt(&gpio4, 1);
		k_msleep(15);
		// Fourth step
		gpio_pin_set_dt(&gpio4, 0);
		gpio_pin_set_dt(&gpio5, 1);
		k_msleep(15);
	}
}

void step_backward(int steps){
	for(int i = 0; i < steps; i++){
		// First step
		gpio_pin_set_dt(&gpio4, 0);
		gpio_pin_set_dt(&gpio5, 1);
		k_msleep(15);
		// Second step
		gpio_pin_set_dt(&gpio3, 0);
		gpio_pin_set_dt(&gpio4, 1);
		k_msleep(15);
		// Third step
		gpio_pin_set_dt(&gpio2, 0);
		gpio_pin_set_dt(&gpio3, 1);
		k_msleep(15);
		// Fourth step
		gpio_pin_set_dt(&gpio5, 0);
		gpio_pin_set_dt(&gpio2, 1);
		k_msleep(15);
	}
}

void main(void){
	// config the LED pin as output
	gpio_pin_configure_dt(&gpio2, GPIO_OUTPUT);
	gpio_pin_configure_dt(&gpio3, GPIO_OUTPUT);
	gpio_pin_configure_dt(&gpio4, GPIO_OUTPUT);
	gpio_pin_configure_dt(&gpio5, GPIO_OUTPUT);

	// Loop forever blinking the light
	while (1) {
		// step forward 100
		step_forward(500);
		k_msleep(1000);
		step_backward(500);
	}
}

