/*
 * peripheral_setup.c
 *
 *  Created on: Oct 24, 2015
 *      Author: OS
 */

#include "peripheral_setup.h"

volatile unsigned long delay;
void Clock_setup()
{
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);  // clock 80MHz
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);	// clock for GPIO port B
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);	// clock for GPIO port F
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);	// clock for Timer0
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);		// clock for ADC0
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);		// clock for PWM1
}

void GPIO_setup()
{
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2);	// Pin B2 is output
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);	// Pin F4 is input
	MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	// Pin F4 has weak pull-up
	MAP_GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);		// set up Interrupt for Pin PF4
	MAP_GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);		// enable Interrupt for Pin PF4
	MAP_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);	// Pin F1 is PWM
	GPIO_PORTF_PCTL_R &= ~GPIO_PCTL_PF1_M & ~GPIO_PCTL_PF2_M & ~GPIO_PCTL_PF3_M;  // PF1 masks
	GPIO_PORTF_PCTL_R |= GPIO_PCTL_PF1_M1PWM5 | GPIO_PCTL_PF2_M1PWM6 | GPIO_PCTL_PF3_M1PWM7;
}

void ADC_setup()
{
	MAP_ADCIntClear(ADC0_BASE, 0);
	MAP_ADCHardwareOversampleConfigure(ADC0_BASE, 64);		// hardware oversampling, take average of 64 sapmle
	MAP_ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);  // sequence 0, step 0, measure channel 0
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH0);  // sequence 0, step 1, measure channel 1
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH1);  // sequence 0, step 2, measure channel 1
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH1);  // sequence 0, step 3, measure channel 1
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH1);  // sequence 0, step 4, measure channel 2
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH1);  // sequence 0, step 5, measure channel 3
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH2);  // sequence 0, step 6, measure channel 3
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH3|ADC_CTL_IE|ADC_CTL_END);  // sequence 0, step 7, measure channel 3, end sequence
	MAP_ADCSequenceEnable(ADC0_BASE, 0);	// enable ADC
}

void Timer_setup()
{
	MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	uint32_t Period = SysCtlClockGet()/62.5;	// timer: 16ms per interrrupt
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, Period -1);	// Timer interrupt 50 times per second
	MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);	// Enable timer interrupt
	MAP_TimerEnable(TIMER0_BASE, TIMER_A);	// enable timer A;
}

void PWM_setup()
{

	// PWM for dimming
	MAP_PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
	MAP_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 2000);

	// PWM for Zeta and SEPIC
	MAP_PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
	MAP_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 1280);

	// enable PWM
	MAP_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	MAP_PWMGenEnable(PWM1_BASE, PWM_GEN_3);
}

void mode_change(uint8_t mode)
{
	switch(mode)
	{
		case 0:
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
			break;
		case 1:
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, false);
			break;
		case 2:
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false);
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
			break;
		case 3:
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false);
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
			break;
	}
}
