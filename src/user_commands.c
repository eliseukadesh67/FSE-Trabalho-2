#include "user_commands.h"

char temperature_control_mode = 0;

char get_temperature_control_mode()
{
  return temperature_control_mode;
}

int process_user_command(int command)
{
  int result = 0;
  switch (command)
  {
  case TURN_ON_OVEN:
    result = turn_on_oven();
    break;
  case TURN_OFF_OVEN:
    result = turn_off_oven();
    break;
  case START_WARMING:
    result = start_warming();
    break;
  case CANCEL_PROCESS:
    result = stop_warming();
    break;
  case MENU:
    result = switch_temperature_mode();
    break;
  default:
    break;
  }

  return result;
}

int turn_on_oven()
{
  return send_system_state(1) == 1;
}

int turn_off_oven()
{
  return send_system_state(0) == 0;
}

int start_warming()
{
  return send_functioning_state(1) == 1;
}

int stop_warming()
{
  return send_functioning_state(0) == 0;
}

int switch_temperature_mode()
{
  temperature_control_mode = !temperature_control_mode;
  return set_reference_temperature_control_mode(temperature_control_mode) == temperature_control_mode;
}