#ifndef PWM_H
#define PWM_H

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

#define RESISTOR 4
#define FAN 5
 
int pwm_control(int pin, int value);
int pwm_turn_off(int pin);
void gpio_stop_all();

#endif