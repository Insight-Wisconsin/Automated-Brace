#ifndef MOTOR_H_
#define MOTOR_H_

// Initialize GPIo pins
void setup_mcpwm_pins(void);

//
void setup_mcpwm(void);

// sets duty cycle
void set_duty_cycle(10000);


#endif