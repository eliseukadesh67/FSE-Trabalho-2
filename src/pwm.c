#include "pwm.h"

int pwm_control(int pin, int value)
{
  if (wiringPiSetup() == -1)
  {
    return 1;
  }

  pinMode(pin, OUTPUT);
  softPwmCreate(pin, 1, 100);
  softPwmWrite(pin, value);
  delay(10);

  return 0;
}

int pwm_turn_off(int pin)
{
  if (wiringPiSetup() == -1)
  {
    return 1;
  }

  pinMode(pin, OUTPUT);
  softPwmCreate(pin, 1, 100);
  softPwmWrite(pin, 0);
  delay(10);

  return 0;
}

void gpio_stop_all()
{
  while (pwm_control(RESISTOR, 0) != 0)
    ;
  while (pwm_control(FAN, 0) != 0)
    ;
  while (pwm_turn_off(FAN))
    ;
  while (pwm_turn_off(RESISTOR))
    ;
}