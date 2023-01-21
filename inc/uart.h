#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 0

int open_uart_stream();
void close_uart_stream(int uart_stream);
int write_uart_stream(int uart_stream, unsigned char *tx_buffer, int bytes);
int read_uart_stream(int uart_stream, unsigned char *rx_buffer, int bytes);

#endif
