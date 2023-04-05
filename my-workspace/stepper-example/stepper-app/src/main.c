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
#define GPIO_NODE_6 DT_ALIAS(gpio6)
#define GPIO_NODE_7 DT_ALIAS(gpio7)

#define SLEEPTIME 15
#define STEPS 500

// create gpio device from rpi_pico.overlay
// motor gpio
static const struct gpio_dt_spec gpio2 = GPIO_DT_SPEC_GET(GPIO_NODE_2, gpios);
static const struct gpio_dt_spec gpio3 = GPIO_DT_SPEC_GET(GPIO_NODE_3, gpios);
static const struct gpio_dt_spec gpio4 = GPIO_DT_SPEC_GET(GPIO_NODE_4, gpios);
static const struct gpio_dt_spec gpio5 = GPIO_DT_SPEC_GET(GPIO_NODE_5, gpios);
// button gpio
static const struct gpio_dt_spec gpio6 = GPIO_DT_SPEC_GET(GPIO_NODE_6, gpios); // up button
static const struct gpio_dt_spec gpio7 = GPIO_DT_SPEC_GET(GPIO_NODE_7, gpios); // down button

void step_forward(int steps){
	for(int i = 0; i < steps; i++){
		// First step
		gpio_pin_set_dt(&gpio5, 0);
		gpio_pin_set_dt(&gpio2, 1);
		k_msleep(SLEEPTIME);
		// Second step
		gpio_pin_set_dt(&gpio2, 0);
		gpio_pin_set_dt(&gpio3, 1);
		k_msleep(SLEEPTIME);
		// Third step
		gpio_pin_set_dt(&gpio3, 0);
		gpio_pin_set_dt(&gpio4, 1);
		k_msleep(SLEEPTIME);
		// Fourth step
		gpio_pin_set_dt(&gpio4, 0);
		gpio_pin_set_dt(&gpio5, 1);
		k_msleep(SLEEPTIME);
	}
}

void step_backward(int steps){
	for(int i = 0; i < steps; i++){
		// First step
		gpio_pin_set_dt(&gpio4, 0);
		gpio_pin_set_dt(&gpio5, 1);
		k_msleep(SLEEPTIME);
		// Second step
		gpio_pin_set_dt(&gpio3, 0);
		gpio_pin_set_dt(&gpio4, 1);
		k_msleep(SLEEPTIME);
		// Third step
		gpio_pin_set_dt(&gpio2, 0);
		gpio_pin_set_dt(&gpio3, 1);
		k_msleep(SLEEPTIME);
		// Fourth step
		gpio_pin_set_dt(&gpio5, 0);
		gpio_pin_set_dt(&gpio2, 1);
		k_msleep(SLEEPTIME);
	}
}

void main(void){
	// config the motor pin as output
	gpio_pin_configure_dt(&gpio2, GPIO_OUTPUT);
	gpio_pin_configure_dt(&gpio3, GPIO_OUTPUT);
	gpio_pin_configure_dt(&gpio4, GPIO_OUTPUT);
	gpio_pin_configure_dt(&gpio5, GPIO_OUTPUT);
	// config button pin as input
	gpio_pin_configure_dt(&gpio6, GPIO_INPUT);
	gpio_pin_configure_dt(&gpio7, GPIO_INPUT);
	
	while (1) {
		if (gpio_pin_get_dt(&gpio6) && gpio_pin_get_dt(&gpio7)) continue;
		if (gpio_pin_get_dt(&gpio6)) {
			step_forward(STEPS);
			k_msleep(1000);
		}
		if (gpio_pin_get_dt(&gpio7)) {
			step_backward(STEPS);
			k_msleep(1000);
		}
	}
}

