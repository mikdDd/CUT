#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * @brief Logger struct containing pointer to file and file name
 * 
 */
struct Logger{
    FILE* fptr;
    char* file_name;
};
/**
 * @brief Function that creates a new Logger
 * 
 * @param file_name in which logs will be stored
 * @return Logger* pointer to new Logger
 */
Logger* logger_new(char* const file_name){
    if(file_name == NULL)return NULL;
    if(strlen(file_name) == 0)return NULL;

    Logger* l = calloc(1, sizeof(Logger));
    if(l == NULL)return NULL;
    l->file_name = file_name;
    return l;
}
/**
 * @brief Function that deletes given Logger
 * 
 * @param logger to deletion
 */
void logger_delete(Logger* const logger){
    if(logger == NULL)return;
    free(logger);
}

/**
 * @brief Function that logs given string in file
 * 
 * @param logger
 * @param string string message to save in logs 
 */
void logger_log_data(Logger* const logger, const char* const string){
    if(logger == NULL)return;
    if(string == NULL)return;
    if(strlen(string) == 0)return;
    logger->fptr = fopen(logger->file_name,"a");
    fprintf(logger->fptr,"%s\n",string);
    fclose(logger->fptr);
}
