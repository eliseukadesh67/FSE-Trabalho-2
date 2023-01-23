#include "modbus.h"

int get_info_uart(const char subcode, char *data)
{
  // 0x01 | 0x23 | 0xC1 | N N N N
  const char esp32_address = ESP32_ADDRESS;
  const char code = GET_INFO_CODE;
  const u_char matricula[4] = {5, 8, 3, 4};
  u_char tx_buffer[9];

  memcpy(tx_buffer, &esp32_address, 1);
  memcpy(tx_buffer + 1, &code, 1);
  memcpy(tx_buffer + 2, &subcode, 1);
  memcpy(tx_buffer + 3, &matricula, 4);

  short crc = calcula_CRC(tx_buffer, 7);

  memcpy(tx_buffer + 7, &crc, 2);

  u_char rx_buffer[9];

  int uart_stream = open_uart_stream();

  write_uart_stream(uart_stream, tx_buffer, 9);

  usleep(100000);

  int numbytes = read_uart_stream(uart_stream, rx_buffer, 9);

#ifdef DEBUG
  printf("Bytes lidos: %d\n", numbytes);
#endif
  close_uart_stream(uart_stream);

  if (!validate_CRC(rx_buffer, numbytes - 2, rx_buffer[numbytes - 2]))
  {
#ifdef DEBUG
    fprintf(stderr, "Erro de crc!\n");
#endif
    return -1;
  }

  memcpy(data, rx_buffer + 3, 4);

  return 0;
}

int send_info_uart(const char subcode, char *info, size_t info_size, char *response)
{
  // 0x01 | 0x23 | 0xC1 | N N N N
  const char esp32_address = ESP32_ADDRESS;
  const char code = GET_INFO_CODE;
  const u_char matricula[4] = {5, 8, 3, 4};
  u_char *tx_buffer = (u_char *)malloc(7 + info_size);

  memcpy(tx_buffer, &esp32_address, 1);
  memcpy(tx_buffer + 1, &code, 1);
  memcpy(tx_buffer + 2, &subcode, 1);
  memcpy(tx_buffer + 3, &matricula, 4);
  memcpy(tx_buffer + 7, info, info_size);

  short crc = calcula_CRC(tx_buffer, 7 + info_size);

  memcpy(tx_buffer + 7 + info_size, &crc, 2);

  u_char rx_buffer[9];

  int uart_stream = open_uart_stream();

  write_uart_stream(uart_stream, tx_buffer, 9 + info_size);

  usleep(100000);

  free(tx_buffer);

  int numbytes = read_uart_stream(uart_stream, rx_buffer, 9);

#ifdef DEBUG
  printf("Bytes lidos: %d\n", numbytes);
#endif
  close_uart_stream(uart_stream);

  if (!validate_CRC(rx_buffer, numbytes - 2, (short)rx_buffer[numbytes - 2]))
  {
#ifdef DEBUG
    fprintf(stderr, "Erro de crc!\n");
#endif
    return -1;
  }

  memcpy(&response, rx_buffer, 7);

  return 0;
}

float get_internal_temperature()
{
  const char subcode = GET_INTERNAL_TEMPERATURE_SUBCODE;
  char data[4];
  float temp;

  get_info_uart(subcode, data);

  memcpy(&temp, data, 4);

  return (float)temp;
}

float get_reference_temperature()
{
  const char subcode = GET_REFERENCE_TEMPERATURE_SUBCODE;
  char data[4];
  float temp;

  get_info_uart(subcode, data);

  memcpy(&temp, data, 4);

  return (float)temp;
}

int read_user_command()
{
  const char subcode = GET_USER_COMMAND_SUBCODE;
  char data[4];
  int temp;

  get_info_uart(subcode, data);

  memcpy(&temp, data, 4);

  return (int)temp;
}

void send_control_signal(int signal)
{
  const char subcode = SEND_C0NTROL_SIGNAL;
  char response[9], info[4];

  memcpy(info, &signal, 4);

  send_info_uart(subcode, info, 4, response);
}

void send_reference_signal(float signal)
{
  const char subcode = SEND_REFERENCE_SIGNAL;
  char response[9], info[4];

  memcpy(info, &signal, 4);

  send_info_uart(subcode, info, 4, response);
}

int send_system_state(char state)
{
  const char subcode = SEND_SYSTEM_STATE;
  char response[9], info[1];
  int response_state;

  memcpy(info, &state, 1);

  send_info_uart(subcode, info, 1, response);

  memcpy(&response_state, response + 3, 4);

  return response_state;
}

int set_reference_temperature_control_mode(char mode)
{
  const char subcode = SET_REFERENCE_TEMPERATURE_CONTROL_MODE;
  char response[9], info[1];
  int response_mode;

  memcpy(info, &mode, 1);

  send_info_uart(subcode, info, 1, response);

  memcpy(&response_mode, response + 3, 4);

  return response_mode;
}

int send_functioning_state(char state)
{
  const char subcode = SEND_FUNCTIONING_STATE;
  char response[9], info[1];
  float response_state;

  memcpy(info, &state, 1);

  send_info_uart(subcode, info, 1, response);

  memcpy(&response_state, response + 3, 4);

  return response_state;
}

float send_ambient_temperature(float temp)
{
  const char subcode = SEND_AMBIENT_TEMPERATURE;
  char response[9], info[4];
  float response_temp;

  memcpy(info, &temp, 4);

  send_info_uart(subcode, info, 4, response);

  memcpy(&response_temp, response + 3, 4);

  return response_temp;
}