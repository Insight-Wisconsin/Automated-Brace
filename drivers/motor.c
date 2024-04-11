#include "motor.h"
#include "drivers/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

// MCPWM Pins
static const unsigned Q = 16;     // GPIO16;
static const unsigned Q_BAR = 17; // GPIO17;

static mcpwm_dev_t *MCPWM[2] = {&MCPWM0, &MCPWM1};

static void setup_mcpwm_pins()
{
   Serial.println("initializing mcpwm control gpio...n");
   mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, Q);
   mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, Q_BAR);
} // setup_pins()

static void setup_mcpwm()
{
   setup_mcpwm_pins();

   Serial.println("Configuring initial parameters ...\n");
   // Not that this is initial configuration only, further, more
   // detailed configuration will often be required.
   mcpwm_config_t pwm_config;
   pwm_config.frequency = 10000;  //frequency = 1000Hz
   pwm_config.cmpr_a = 30.0;      //duty cycle of PWMxA = 30.0%
   pwm_config.cmpr_b = 30.0;      //duty cycle of PWMxB = 30.0%
   pwm_config.counter_mode = MCPWM_UP_DOWN_COUNTER; // Up-down counter (triangle wave)
   pwm_config.duty_mode = MCPWM_DUTY_MODE_0; // Active HIGH
   mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings

   // The following calls set what happens on transitions relative to the
   // comparators.  The API documentation doesn't make this clear, but
   // I believe that these set the 'actions' described under "PWM Signal Generation"
   // in section 15.3.3 on pg. 341 of the reference manual (v2.7).  What is odd is
   // that 'toggle' is not supported.  I don't need these for my application, but
   // I did experiment with them and they work.
   // mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
   // mcpwm_set_signal_high(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
	
   // MCPWMXA to duty mode 1 and MCPWMXB to duty mode 0 or vice versa
   // will generate MCPWM compliment signal of each other, there are also
   // other ways to do it
   mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
   mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, MCPWM_DUTY_MODE_1);

   // Allows the setting of a minimum dead time between transitions to prevent
   // "shoot through" a totem-pole switch pair, which can be disastrous.
//	mcpwm_deadtime_enable(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_BYPASS_FED, 100, 100);   //Deadtime of 10us

   mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);
} // setup_mcpwm

static void set_duty_cycle(float dc)
{
   // dc is the duty cycle, as a percentage (not a fraction.)
   // (e.g. 30.0 is 30%)
   mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, dc);
   mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 100.0 - dc);
} // set_duty_cycle()

static void set_frequency(uint32_t freq)
{
   // freq is the desired frequency, in Hz.
   mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_0, freq*2U);
} // Set the PWM frequency.
