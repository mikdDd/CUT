#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Logger{
    FILE* fptr;
    char* file_name;
};
Logger* logger_new(char* const file_name){
    if(file_name == NULL)return NULL;
    if(strlen(file_name) == 0)return NULL;

    Logger* l = calloc(1, sizeof(Logger));
    if(l == NULL)return NULL;
    l->file_name = file_name;
    return l;
   // l->fptr = fopen(file_name,"a");
}
void logger_delete(Logger* const logger){
    if(logger == NULL)return;
    free(logger);
}
void logger_log_data(Logger* const logger, const char* const string){
    if(logger == NULL)return;
    if(string == NULL)return;
    if(strlen(string) == 0)return;
    logger->fptr = fopen(logger->file_name,"a");
    fprintf(logger->fptr,"%s\n",string);
    fclose(logger->fptr);
}
