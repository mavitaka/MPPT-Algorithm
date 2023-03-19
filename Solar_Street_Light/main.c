#include "peripheral_setup.h"
#include "MPPT_Algorithm.h"

// constant
#define RELAY (*((volatile uint32_t *)(0x42000000+0x053FC*32+0x2*4))) // Pin B2
#define VOLTAGE_GAIN      	   11
#define IDLE					0
#define BATTERY_CHARGING		1
#define LED_DRIVING				2
#define LED_DIMMING				3
#define D_dimming				1000

// global variables

float V, V_pv, P, I_pv, V_bat, I_bat, V_led, I_led;
float diff;
uint8_t mode = 0;
uint8_t time = 1;
uint16_t D_zeta = 640;
uint16_t D_sepic = 640;
uint32_t ADC0Value[8];

void StartButtonDown()	// start button is pushed
{
	MAP_GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);  // Clear interrupt flag
	MAP_GPIOIntDisable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);	//disable interrupt PF4
	if(mode == IDLE)
	{
		mode = BATTERY_CHARGING;
		RELAY=1;
		time = 1;
	}
	/*else if(mode == BATTERY_CHARGING)
	{
		mode = LED_DRIVING;
		time = 1;
		RELAY=0;
	}
	else if(mode == LED_DRIVING)
	{
		mode = LED_DIMMING;
		RELAY=0;
	}
	else
	{
		mode = IDLE;
		RELAY=0;
		mode_change(IDLE);
	}*/
	//mode = LED_DIMMING;
	//RELAY=0;
	//MAP_GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);		// enable Interrupt for Pin PF4
	GPIOIntRegister(GPIO_PORTF_BASE, StartButtonDown);
}

void Timer0A_Handler()  // do once every 0.02s;
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);      // confirm TIMER0A interrupt, clear interrupt flag
	ADCIntClear(ADC0_BASE, 0);
	ADCProcessorTrigger(ADC0_BASE, 0);
	while(!MAP_ADCIntStatus(ADC0_BASE, 0, false))
	{
	}
	MAP_ADCSequenceDataGet(ADC0_BASE, 0, ADC0Value);


	if(mode == BATTERY_CHARGING)
	{
		V_pv = ((float)ADC0Value[0]+(float)ADC0Value[1])*1.65*VOLTAGE_GAIN/4095;  // pv volage
		I_pv = ((float)ADC0Value[2] + (float)ADC0Value[3] + (float)ADC0Value[4] + (float)ADC0Value[5])*8.25/4095 - 25.4;    // pv current
		V_bat = ((float)ADC0Value[6])*3.3*VOLTAGE_GAIN/4095;  // battery voltage
		I_bat = ((float)ADC0Value[7])*33/4095 - 25.4;    // battery current

		float P_temp;

		// accu is fullly charged
		if (V_bat >= 28)
		{
			MAP_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, false); //  // disable PWM -> disable MOSFET
		}

		// start the MPPT
		if (time == 1)  // first time measuring
		{
			D_zeta=576; // duty cycle 0.45
			mode_change(BATTERY_CHARGING);
			time=2;
		}
		else if(time == 2)  // second time measuring
		{
			V=V_pv;
			P=V_pv*I_pv;
			D_zeta=550; // duty cycle 0.43
			time = 3;
		}
		else  // from third time onward
		{
			P_temp = V_pv*I_pv;
			diff = (P_temp-P)/(V_pv-V);
			D_zeta = Hill_Climbing(D_zeta,diff);
			P = P_temp;		// update new value of power
			V = V_pv;		// update new value of voltage
		}
		MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, D_zeta);	// changing duty cycle
	}
	else if(mode == LED_DRIVING)
	{
		V_led = ((float)ADC0Value[0]+(float)ADC0Value[1])*1.65*VOLTAGE_GAIN/4095;
		I_led = 25.3 - ((float)ADC0Value[2] + (float)ADC0Value[3] + (float)ADC0Value[4] + (float)ADC0Value[5])*8.25/4095;
		V_bat = ((float)ADC0Value[6])*3.3*VOLTAGE_GAIN/4095;  // battery voltage
		I_bat = 25.3 - ((float)ADC0Value[7])*33/4095;    // battery current
		if(time == 1)
		{
			D_sepic = 640;
			mode_change(LED_DRIVING);
			time = 2;
		}
		else
		{
			if(V_led < 27)
				D_sepic = D_sepic + 10;
			else if (V_led < 30)
				D_sepic = D_sepic + 5;
			else if(V_led < 34.4)
				D_sepic = D_sepic + 2;
			else if(V_led > 34.6)
				D_sepic = D_sepic - 2;
			else if(V_led > 36)
				D_sepic = D_sepic - 5;

			if(D_sepic > 1000)
				D_sepic=1000;
			else if(D_sepic < 200)
				D_sepic=200;
		}
		MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, D_sepic);	// change sepic duty cycle
	}
	else if(mode == LED_DIMMING)
	{
		V_led = ((float)ADC0Value[0]+(float)ADC0Value[1])*1.65*VOLTAGE_GAIN/4095;
		I_led = 25.3 - ((float)ADC0Value[2] + (float)ADC0Value[3] + (float)ADC0Value[4] + (float)ADC0Value[5])*8.25/4095;
		V_bat = ((float)ADC0Value[6])*3.3*VOLTAGE_GAIN/4095;  // battery voltage
		I_bat = 25.3 - ((float)ADC0Value[7])*33/4095;    // battery current
		MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, D_dimming);
		mode_change(LED_DIMMING);
	}
}

 void main(void) {
	Clock_setup(); // clock set up
	GPIO_setup();	// GPIO set up
	GPIOIntRegister(GPIO_PORTF_BASE, StartButtonDown);
	RELAY = 0; // disable relay pin
	ADC_setup();// adc set up
	Timer_setup();// timer set up
	TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0A_Handler);
	PWM_setup();// PWM set up
	while(1);
}
