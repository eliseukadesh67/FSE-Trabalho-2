#include "logger.h"

FILE *csv_log_file;

static char log_buffer[256] = "";

static pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

static void lock_buffer()
{
  pthread_mutex_lock(&buffer_mutex);
}

static void unlock_buffer()
{
  pthread_mutex_unlock(&buffer_mutex);
}

void init_csv_log()
{
  csv_log_file = fopen("temperature_logs.csv", "w+");
  if (!csv_log_file)
  {
    fprintf(stderr, "Falha ao abrir o arquivo de logs\n");
  }
  fprintf(csv_log_file, "Data e hora, Temperatura interna, Temperatura externa, Temperatura definida pelo usuário, %%Resistor, %%Venoinha\n");
}

void *add_to_csv_log()
{
  while (1)
  {
    char str[256] = "";
    if (strcmp(log_buffer, "") != 0)
    {
      strcpy(str, log_buffer);
      fprintf(csv_log_file, str);
    }
    sleep(1);
  }

  pthread_exit(0);
}

void close_csv_log()
{
  fclose(csv_log_file);
}

void update_log_buffer(float internalTemp, float externalTemp, float referenceTemp, int resistorSignal, int fanSignal)
{
  struct tm *info;
  char datetime[80];
  time_t rawtime;
  time(&rawtime);
  info = localtime(&rawtime);
  strftime(datetime, 80, "%d/%m/%Y %X", info);
  sprintf(log_buffer, "%s, %.2f, %.2f, %.2f, %d, %d\n", datetime, internalTemp, externalTemp, referenceTemp, resistorSignal, fanSignal);
}