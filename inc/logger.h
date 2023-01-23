#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void init_csv_log();
void *add_to_csv_log();
void close_csv_log();
void update_log_buffer(float internalTemp, float externalTemp, float referenceTemp, int resistorSignal, int fanSignal);

#endif