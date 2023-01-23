#include "ui.h"
#include "modbus.h"
#include "user_commands.h"

static char ui_control_mode = DASHBOARD, temp_reference_control_mode = DEBUG_CONTROL_MODE;

void *read_dashboard()
{
  while (1)
  {
    int command = read_user_command();
    process_user_command(command);
    ui_control_mode = get_temperature_control_mode();
    usleep(500000);
  }
}

char get_control_mode()
{
  return ui_control_mode;
}

char get_reference_control_mode()
{
  return temp_reference_control_mode;
}

int is_debug_mode()
{
  return temp_reference_control_mode == DEBUG_CONTROL_MODE;
}

int is_temperature_curve_mode()
{
  return temp_reference_control_mode == TEMPERATURE_CURVE_MODE;
}

int is_dashboard_mode()
{
  return ui_control_mode == DASHBOARD;
}

int is_terminal_mode()
{
  return ui_control_mode == TERMINAL;
}

void switch_reference_mode()
{
  temp_reference_control_mode = !temp_reference_control_mode;
}

void set_control_mode(char mode){
  temp_reference_control_mode = mode; 
}

void set_temp_reference_control_mode(char mode){
  temp_reference_control_mode = mode; 
}