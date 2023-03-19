/*
 * peripheral_setup.h
 *
 *  Created on: Oct 24, 2015
 *      Author: OS
 */

#ifndef PERIPHERAL_SETUP_H_
#define PERIPHERAL_SETUP_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/rom_map.h"
#include "driverlib/adc.h"

void Clock_setup();
void GPIO_setup();
void ADC_setup();
void Timer_setup();
void PWM_setup();
void mode_change(uint8_t mode);



#endif /* PERIPHERAL_SETUP_H_ */
