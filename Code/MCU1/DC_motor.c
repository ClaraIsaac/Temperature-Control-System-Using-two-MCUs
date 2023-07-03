/*
 * DC_motor.c
 *
 *  Created on: Sep 11, 2022
 *      Author: Clara Isaac
 */

#include "gpio.h"
#include "pwm_timer0.h" /* To use the PWM Function */
#include "DC_motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the DC motor:
 * 1. Setup the DC motor pins directions by use the GPIO driver.
 * 2. Initialize Values on pins.
 */
void DC_motor_init(void)
{
	/* Configure the direction for A, B pins as output pins */
	GPIO_setupPinDirection(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, PIN_OUTPUT);

	/* Set Clockwise direction as initial direction */
	GPIO_writePin(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, LOGIC_LOW);
	GPIO_writePin(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, LOGIC_HIGH);
}

/*
 * Description:
 * Move the motor with a specific speed using PWM timer0
 */
void speed_of_motor(uint8 speed)
{
	uint8 duty_cycle = (speed * 255) / 100; /* Take the percentage speed and multiply by (2^8 - 1) */
	PWM_Timer0_Start(duty_cycle); /* Send the duty cycle to PWM function to generate the PWM signal */
}

/*
 * Description:
 * Specify the direction of the motor: Forward or Backward.
 */
void direction_of_motor(uint8 direction)
{
	if (direction == 'B') /* Backward Direction */
	{
		/* A = 1, B = 0 */
		GPIO_writePin(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, LOGIC_LOW);
	}
	else if (direction == 'F') /* Backward Direction */
	{
		/* A = 0, B = 1 */
		GPIO_writePin(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, LOGIC_HIGH);
	}
}
