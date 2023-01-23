#include "bme280.h"
#include "user_commands.h"
#include "pwm.h"
#include "pid.h"
#include "logger.h"
#include "ui.h"
#include <pthread.h>
#include <signal.h>

float internalTemp = 0, referenceTemp = 0, externalTemp = 0, debugReferenceTemp = 35.0;
double kp = 30.0, ki = 0.20, kd = 400.0;
int menu_counter = 0, resistorSignal = 0, fanSignal = 0, prevMode = DEBUG_CONTROL_MODE, currenMode = DEBUG_CONTROL_MODE;

pthread_t menu_thread_id, dashboard_thread_id, csv_log_thread_id, read_curve_file_pthread_id;

void *read_curve_file()
{

  char buffer[256];
  FILE *file = fopen("utils/curva_reflow.csv", "r");
  int current_time = 0, last_time = 0;
  int current_temperature = 0;
  int count = 0;

  if (!file)
  {
    fprintf(stderr, "Erro ao abrir arquivo de curva temperatura!\n");
    exit(1);
  }
  printf("Iniciando execução do Curva de Temperatura\n\n");
  while (fgets(buffer, 200, file) != NULL)
  {
    count++;
    if (count == 1)
      continue;
    current_time = atoi(strtok(buffer, ","));
    current_temperature = atoi(strdup(strtok(NULL, ",")));
    sleep(current_time - last_time);
    referenceTemp = current_temperature;
    send_reference_signal(current_temperature);
    last_time = current_time;
  }

  fclose(file);

  pthread_exit(0);
}

void temperature_curve()
{
  if (pthread_create(&read_curve_file_pthread_id, NULL, read_curve_file, NULL))
  {
    fprintf(stderr, "Erro ao criar a thread de logs.\n");
    exit(1);
  }
}

void *menu()
{
  while (1)
  {
    system("clear");
    printf("Modo de comando: %s\n\n", is_terminal_mode() ? "Terminal" : "Dashboard");
    if (is_terminal_mode())
    {
      if (menu_counter == 0)
      {
        printf("Selecione o modo de temperatura de referencia:\n");
        printf("(1) Modo DEBUG\n");
        printf("(2) Modo de Curva de Temperatura\n");

        char option = getchar();
        getchar();
        set_temp_reference_control_mode(option - '0' - 1);

        system("clear");
      }
      printf("Modo inicial: %d\n", get_reference_control_mode());
      const char *current_mode = is_debug_mode() ? "Debug" : "Curva de Temperatura";
      const char *change_to_mode = is_debug_mode() ? "Curva de Temperatura" : "Debug";
      prevMode = get_reference_control_mode();
      printf("Menu de opções:\n");
      printf("(1) Alterar Modo de Temperatura de Referência (%s -> %s)\n", current_mode, change_to_mode);
      printf("(2) Definir constantes de PID\n");
      printf("(3) Sair do menu\n");
      printf("Digite a opção: ");
      char option = getchar();
      getchar();
      switch (option)
      {
      case '1':
        switch_reference_mode();
        system("clear");
        printf("Modo %s acionado!\n", is_debug_mode() ? "Debug" : "Curva de Temperatura");
        break;
      case '2':
        system("clear");
        printf("Kp: ");
        scanf("%le", &kp);
        printf("Ki: ");
        scanf("%le", &ki);
        printf("Kd: ");
        scanf("%le", &kd);
        getchar();
        break;
      }

      currenMode = get_reference_control_mode();
      if (is_temperature_curve_mode())
      {
        printf("Usando o modo de curva de temperatura....\n");
        temperature_curve();
      }
      else if ((is_debug_mode() && currenMode != prevMode) || (is_debug_mode() && menu_counter == 0))
      {
        pthread_cancel(read_curve_file_pthread_id);
        printf("Escolha a temperatura de referência: ");
        scanf("%f", &debugReferenceTemp);
        getchar();
        referenceTemp = debugReferenceTemp;
        send_reference_signal(debugReferenceTemp);
      }
      menu_counter++;
    }
    printf("\n\nTemperaturas do sistema:");
    printf("\nTemperatura interna: %.2f | Temperatura externa: %.2f | Temperatura de referencia: %.2f\n\n", internalTemp, externalTemp, referenceTemp);
    printf("Constantes de PID:\n");
    printf("Kp: %.2f | Ki: %.2f | Kd: %.2f\n", kp, ki, kd);
    sleep(1);
  }
  pthread_exit(0);
}

void pwm()
{
  pid_configura_constantes(kp, ki, kd);
  pid_atualiza_referencia(referenceTemp);
  float controle = pid_controle(internalTemp);
  int onPin, offPin;
  int val = controle;
  if (controle > 0)
  {
    onPin = RESISTOR;
    offPin = FAN;
    resistorSignal = controle;
  }
  else if (controle < 0)
  {
    onPin = FAN;
    offPin = RESISTOR;
    val = controle > -40 ? -40 : controle;
    val = val * -1;
    fanSignal = val;
  }
  while (pwm_control(onPin, (int)val) != 0)
    ;
  while (pwm_turn_off(offPin))
    ;
  send_control_signal((int)controle);
}

void update_temperature()
{

  float recievedInternalTemp, recievedReferenceTemp, recievedExternalTemp;

  recievedInternalTemp = get_internal_temperature();
  usleep(10000);

  if (is_dashboard_mode())
  {
    recievedReferenceTemp = get_reference_temperature();
    usleep(10000);
  }

  while (get_bme280_temp(&recievedExternalTemp) != BME280_OK)
    ;

  usleep(10000);

  if (recievedExternalTemp > 0 && recievedExternalTemp < 100)
    externalTemp = recievedExternalTemp;
  if (recievedInternalTemp > 0 && recievedInternalTemp < 100)
    internalTemp = recievedInternalTemp;
  if (recievedReferenceTemp > 0 && recievedReferenceTemp < 100 && (is_dashboard_mode()))
    referenceTemp = recievedReferenceTemp;

  send_ambient_temperature(externalTemp);

  update_log_buffer(internalTemp, externalTemp, referenceTemp, resistorSignal, fanSignal);
}

void shutdown()
{
  system("clear");
  printf("Finalizando sistema.......\n");
  pthread_cancel(menu_thread_id);
  pthread_cancel(dashboard_thread_id);
  pthread_cancel(csv_log_thread_id);
  pthread_cancel(read_curve_file_pthread_id);
  send_control_signal(0);
  stop_warming();
  turn_off_oven();
  close_csv_log();
  exit(0);
}

int main(int argc, const char *argv[])
{
  set_reference_temperature_control_mode(get_control_mode());

  signal(SIGINT, shutdown);

  init_csv_log();
  update_temperature();

  if (pthread_create(&menu_thread_id, NULL, menu, NULL) != 0)
  {
    fprintf(stderr, "Erro ao criar a thread de menu.\n");
    exit(1);
  }

  if (pthread_create(&dashboard_thread_id, NULL, read_dashboard, NULL) != 0)
  {
    fprintf(stderr, "Erro ao criar a thread de dashboard.\n");
    exit(1);
  }

  if (pthread_create(&csv_log_thread_id, NULL, add_to_csv_log, NULL))
  {
    fprintf(stderr, "Erro ao criar a thread de logs.\n");
    exit(1);
  }

#ifdef DEBUG
  fprintf(stdout, "Executando modo debug...\n");
#endif
  while (1)
  {
    update_temperature();
    pwm();
    sleep(1);
  }

  return 0;
}