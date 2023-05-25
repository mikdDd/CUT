#ifndef LOGGER_H
#define LOGGER_H

#include "data.h"

typedef struct Logger Logger;
Logger* logger_new(char* const file_name);
void logger_delete(Logger* const logger);
void logger_log_data(Logger* const logger, const char* const string);

#endif
