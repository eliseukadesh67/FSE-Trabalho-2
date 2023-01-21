#include "uart.h"

const char *UART_DEFAULT_FILE = "/dev/serial0";

int open_uart_stream()
{

  int uart0_filestream = -1;

  uart0_filestream = open(UART_DEFAULT_FILE, O_RDWR | O_NOCTTY | O_NDELAY); // Open in non blocking read/write mode
  if (uart0_filestream == -1)
  {
    fprintf(stderr, "Erro - Não foi possível iniciar a UART.\n");
    exit(1);
  }
  else
  {
    #if DEBUG == 1
      printf("UART inicializada!\n");
    #endif
  }

  return uart0_filestream;
}
void close_uart_stream(int uart_stream)
{
  close(uart_stream);
}

int write_uart_stream(int uart_stream, unsigned char *tx_buffer, int bytes)
{
  int count;

  if (uart_stream == -1)
    exit(1);

  if (uart_stream != -1)
  {
    #if DEBUG == 1
      printf("Escrevendo caracteres na UART ...");
    #endif
    count = write(uart_stream, &tx_buffer[0], bytes);
    if (count < 0)
    {
      #if DEBUG == 1
        printf("UART TX error\n");
      #endif
    }
    else
    {
      #if DEBUG == 1
        printf("escrito.\n");
      #endif
    }
  }
  return count;
}
int read_uart_stream(int uart_stream, unsigned char *rx_buffer, int bytes)
{
  if (uart_stream == -1)
    exit(1);

  int rx_length = read(uart_stream, (void *)rx_buffer, bytes); // Filestream, buffer to store in, number of bytes to read (max)
  if (rx_length < 0)
  { 
    #if DEBUG == 1
      fprintf(stderr, "Erro na leitura.\n"); // An error occured (will occur if there are no bytes)
    #endif
  }
  else if (rx_length == 0)
  {
    #if DEBUG == 1
      printf("Nenhum dado disponível.\n"); // No data waiting
    #endif
  }

  return rx_length;
}
