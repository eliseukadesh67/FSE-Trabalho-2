#ifndef USER_COMMANDS_H_
#define USER_COMMANDS_H_

#include "modbus.h"

char get_temperature_control_mode();
int process_user_command(int command);
int turn_on_oven();
int turn_off_oven();
int start_warming();
int stop_warming();
int switch_temperature_mode();

#endif