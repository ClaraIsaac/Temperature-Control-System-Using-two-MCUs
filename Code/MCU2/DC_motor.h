/*
 * DC_motor.h
 *
 *  Created on: Sep 11, 2022
 *      Author: Clara Isaac
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/* DC motor HW Ports and Pins Ids */
#define DC_motor_A_PORT_ID PORTB_ID
#define DC_motor_B_PORT_ID PORTB_ID

#define DC_motor_A_PIN_ID PIN4_ID
#define DC_motor_B_PIN_ID PIN5_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the DC motor:
 * 1. Setup the DC motor pins directions by use the GPIO driver.
 * 2. Initialize Values on pins.
 */
void DC_motor_init(void);

/*
 * Description:
 * Move the motor with a specific speed using PWM timer0
 */
void speed_of_motor(uint8 speed);

/*
 * Description:
 * Specify the direction of the motor: Forward or Backward.
 */
void direction_of_motor(uint8 direction);

#endif /* DC_MOTOR_H_ */
