#include "test_logger.h"
#include "logger.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
void test_logger_run_all(void){
    test_logger_new();
    test_logger_delete();
    test_logger_log_data();
}
void test_logger_new(void){
    //USUAL BEHAVIOR
    {
    Logger* logger = logger_new("FILE");
    assert(logger != NULL);
    }
    //NULL FILE
    {
    Logger* logger = logger_new(NULL);
    assert(logger == NULL);
    }
    //EMPTY FILE NAME
    {
    Logger* logger = logger_new("");
    assert(logger == NULL);
    }
    
}

void test_logger_delete(void){
    //CHECK IF NOT CRASHING
    {
    Logger* logger = NULL;
    logger_delete(logger);
    }

    //USUAL BEHAVIOR
    {
    Logger* logger = logger_new("FILE");
    logger_delete(logger);
    }
}

void test_logger_log_data(void){
    
    //USUAL BEHAVIOR
    {
    Logger* logger = logger_new("FILE");
    logger_log_data(logger,(char*){"LOGGER TEST"});
    int rem = remove("FILE");
    assert(rem == 0);
    }
    //NULL LOGGER
    {
    Logger* logger = NULL;
    logger_log_data(logger,(char*){"LOGGER TEST"});
    }
    //NULL MESSAGE
    {
    Logger* logger = logger_new("FILE");
    logger_log_data(logger,NULL);
    }
}


