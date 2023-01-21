#ifndef MODBUS_H_
#define MODBUS_H_

#include "uart.h"
#include "codes.h"
#include "crc16.h"

#define DEBUG 0

int get_info_uart(const char subcode, char *data);
int send_info_uart(const char subcode, char *info, size_t info_size, char *response);
float get_internal_temperature();
float get_reference_temperature();
void send_control_signal(int signal);
void send_reference_signal(float signal);
int send_system_state(char state);
int set_reference_temperature_control_mode(char mode);
int send_functioning_state(char state);
float send_ambient_temperature(float temp);
int read_user_command();

#endif
