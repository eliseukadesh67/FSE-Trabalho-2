#ifndef UI_H_
#define UI_H_

#include "ui_defs.h"

void *read_dashboard();
char get_control_mode();
char get_reference_control_mode();
int is_debug_mode();
int is_temperature_curve_mode();
int is_dashboard_mode();
int is_terminal_mode();
void switch_reference_mode();
void set_control_mode(char mode);
void set_temp_reference_control_mode(char mode);

#endif