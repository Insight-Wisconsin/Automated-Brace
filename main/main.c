#include <stdio.h>
#include "drivers/motor.h"

void app_main(void)
{
    setup_mcpwm_pins();

    setup_mcpwm(void);

// sets duty cycle
    set_duty_cycle(float dc);
}