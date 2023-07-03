/*
 * main.c
 *
 *  Created on: Sep 18, 2022
 *      Author: Clara Isaac
 */

/*
 * MCU1
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"
#include "std_types.h"
#include "gpio.h"
#include "DC_motor.h"
#include "lm35_sensor.h"
#include "pwm_timer0.h"
#include "uart.h"
#include "adc.h"

/* Port and Pin of Push Button */
#define PUSH_BOTTON_PORT PORTD_ID
#define PUSH_BOTTON_PIN PIN2_ID

/* Interrupt Service Routine of Interrupt 0*/
ISR(INT0_vect)
{
	UART_sendByte(200);
}

/* Interrupt initialization */
void INT0_Init(void)
{
	GPIO_setupPinDirection(PUSH_BOTTON_PORT, PUSH_BOTTON_PIN, PIN_INPUT); // set push button pin as input pin
	MCUCR |= (1<<ISC00) | (1<<ISC01);   // Trigger INT0 with the raising edge
	GICR  |= (1<<INT0);// Enable external interrupt pin INT0
	SREG |= (1<<7); // Enable the Global Interrupt (I-bit)
}


int main(void)
{
	/* ADC For Temp Sensor Configuration */
	ADC_ConfigType ADC_Config = {INTERNAL_VOLTAGE, F_CPU_8};
	ADC_init (&ADC_Config);

	/* UART initialization */
	UART_ConfigType uart_type = {EIGHT_BIT, DISABLED, TWO_BIT, 9600}; // UART specs
	UART_init(&uart_type); // initialize UART

	/* Peripherals Initialization */
	INT0_Init();
	DC_motor_init();

	/* variable declaration */
	uint8 temperature, speed;

	while(1)
	{
		temperature = LM35_getTemperature(); /* get the temperature from temperature sensor */
		UART_sendByte(temperature); /* send the temperature to MCU2 */

		speed = UART_recieveByte(); /* receive from MCU2 the speed of the MOTOR */

		/* if speed of MOTOR greater than 70, turn on the fan */
		if (speed >= 70)
		{
			speed_of_motor(100);
		}
		else speed_of_motor(0);
	}
}
