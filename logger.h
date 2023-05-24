#ifndef LOGGER_H
#define LOGGER_H

#include "data.h"

typedef struct Logger Logger;
Logger* logger_new(char* const file_name);
void logger_delete(Logger* logger);
void logger_log_data(Logger* logger, char array[]);

#endif
