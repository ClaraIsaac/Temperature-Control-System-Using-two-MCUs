/*
 * main.c
 *
 *  Created on: Sep 18, 2022
 *      Author: Clara Isaac
 */

/*
 * MCU2
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "common_macros.h"
#include "adc.h"
#include "std_types.h"
#include "gpio.h"
#include "DC_motor.h"
#include "pwm_timer0.h"
#include "uart.h"

/* Ports and Pins of LEDs and the channel of the ADC */
#define SENSOR_CHANNEL_ID 0
#define GREEN_LED_PORT PORTB_ID
#define YELLOW_LED_PORT PORTB_ID
#define RED_LED_PORT PORTB_ID
#define GREEN_LED_PIN PIN2_ID
#define YELLOW_LED_PIN PIN1_ID
#define RED_LED_PIN PIN0_ID


int main(void)
{
	/* Specify HW ports and pins directions */
	GPIO_setupPinDirection(GREEN_LED_PORT, GREEN_LED_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(YELLOW_LED_PORT, YELLOW_LED_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(RED_LED_PORT, RED_LED_PIN, PIN_OUTPUT);

	/* ADC For Potentiometer Configuration */
	ADC_ConfigType ADC_Config = {INTERNAL_VOLTAGE, F_CPU_8};
	ADC_init (&ADC_Config);

	/* UART initialization */
	UART_ConfigType uart_type = {EIGHT_BIT, DISABLED, TWO_BIT, 9600}; // UART specs
	UART_init(&uart_type); // initialize UART

	/* Peripherals Initialization */
	DC_motor_init();

	/* variables declaration */
	uint16 adc_value;
	uint8 motor_speed, temperature, i;

	while(1)
	{
		temperature = UART_recieveByte(); /* receive the temperature from MCU1 */

		/* if temperature more than or equal to 200, slow down the motor */
		if (temperature >= 200)
		{
			for(i = motor_speed; i > 0; i--)
			{
				speed_of_motor(i);
				_delay_ms(30);
			}
		}
		else
		{
			/* get the motor speed from the potentiometer */
			adc_value = ADC_readChannel(SENSOR_CHANNEL_ID);
			motor_speed=(uint8)((uint16)(((float)adc_value*2.56/(1023*2))*100));
			/* send the speed percentage to MCU1 */
		 	UART_sendByte(motor_speed);
			speed_of_motor(motor_speed); /* move the motor with the specified speed */
		}

		/* control the lights */
		if (temperature < 20)
		{
			/* Illuminate green LED only */
			GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_HIGH);
			GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_LOW);
			GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
			//speed_of_DC_motor(temperature);
		}
		else if (temperature >= 20 && temperature <= 40)
		{
			/* Illuminate yellow LED only */
			GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_HIGH);
			GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
			GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
			//speed_of_DC_motor(temperature);
		}
		else if (temperature > 40)
		{
			/* Illuminate red LED only */
			GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_HIGH);
			GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
			GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_LOW);
		}
	}
}

