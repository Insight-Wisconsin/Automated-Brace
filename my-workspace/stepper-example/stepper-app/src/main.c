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
#define GPIO_NODE_8 DT_ALIAS(gpio8)

#define STEP_SLEEP 15
#define STEPS 500
#define CUTOFF 50
#define CUTOFF_REDUCE 10
#define AFTER_INST_SLEEP 1000
#define AFTER_CUTOFF_SLEEP 1000

// create gpio device from rpi_pico.overlay
// motor gpio
static const struct gpio_dt_spec gpio2 = GPIO_DT_SPEC_GET(GPIO_NODE_2, gpios);
static const struct gpio_dt_spec gpio3 = GPIO_DT_SPEC_GET(GPIO_NODE_3, gpios);
static const struct gpio_dt_spec gpio4 = GPIO_DT_SPEC_GET(GPIO_NODE_4, gpios);
static const struct gpio_dt_spec gpio5 = GPIO_DT_SPEC_GET(GPIO_NODE_5, gpios);
// button gpio
static const struct gpio_dt_spec gpio6 = GPIO_DT_SPEC_GET(GPIO_NODE_6, gpios); // up button
static const struct gpio_dt_spec gpio7 = GPIO_DT_SPEC_GET(GPIO_NODE_7, gpios); // down button
static const struct gpio_dt_spec gpio8 = GPIO_DT_SPEC_GET(GPIO_NODE_8, gpios); // emergency stop

void step_forward(int steps){
	for(int i = 0; i < steps; i++){
		// First step
		gpio_pin_set_dt(&gpio5, 0);
		gpio_pin_set_dt(&gpio2, 1);
		k_msleep(STEP_SLEEP);
		// Second step
		gpio_pin_set_dt(&gpio2, 0);
		gpio_pin_set_dt(&gpio3, 1);
		k_msleep(STEP_SLEEP);
		// Third step
		gpio_pin_set_dt(&gpio3, 0);
		gpio_pin_set_dt(&gpio4, 1);
		k_msleep(STEP_SLEEP);
		// Fourth step
		gpio_pin_set_dt(&gpio4, 0);
		gpio_pin_set_dt(&gpio5, 1);
		k_msleep(STEP_SLEEP);
	}
}

void step_backward(int steps){
	for(int i = 0; i < steps; i++){
		// First step
		gpio_pin_set_dt(&gpio4, 0);
		gpio_pin_set_dt(&gpio5, 1);
		k_msleep(STEP_SLEEP);
		// Second step
		gpio_pin_set_dt(&gpio3, 0);
		gpio_pin_set_dt(&gpio4, 1);
		k_msleep(STEP_SLEEP);
		// Third step
		gpio_pin_set_dt(&gpio2, 0);
		gpio_pin_set_dt(&gpio3, 1);
		k_msleep(STEP_SLEEP);
		// Fourth step
		gpio_pin_set_dt(&gpio5, 0);
		gpio_pin_set_dt(&gpio2, 1);
		k_msleep(STEP_SLEEP);
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
	
	int repeat_cmds = 0;
	
	while (1) {
		// emergency stop
		if (gpio_pin_get_dt(&gpio8)) break;
		
		// prevent dual input
		if (gpio_pin_get_dt(&gpio6) && gpio_pin_get_dt(&gpio7)) continue;
		// step forward on forward button press, as long as we haven't reached the cutoff
		if (gpio_pin_get_dt(&gpio6) && repeat_cmds < CUTOFF) {
			step_forward(STEPS);
			repeat_cmds++;
			k_msleep(AFTER_INST_SLEEP);
		}
		// step backward on back button press, as long as we haven't reached the cutoff
		if (gpio_pin_get_dt(&gpio7) && repeat_cmd > -CUTOFF) {
			step_backward(STEPS);
			repeat_cmds--;
			k_msleep(AFTER_INST_SLEEP);
		}
	}
}

