#define _GNU_SOURCE
#include "analyzer.h"
#include "printer.h"
#include "data.h"
#include "watchdog.h"
#include "buffer.h"
#include "logger.h"
#include "reader.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/signal.h>
#include <sys/time.h>

#define WAIT_TIME_SEC 2

static void* thread_reader(void*);
static void* thread_analyzer(void*);
static void* thread_printer(void*);
static void* thread_logger(void*);
static void* thread_watchdog(void*);
static void term (int signum);

static Buffer* reader_buffer = NULL;      
static Buffer* usage_buffer = NULL;
static Buffer* reader_watch_buffer = NULL;
static Buffer* analyzer_watch_buffer = NULL;
static Buffer* printer_watch_buffer = NULL;
static Buffer* log_buffer = NULL;
static Watchdog* watchdog = NULL;




static void term (int signum){
     
     watchdog_cancel_threads(watchdog);
}


static void* thread_reader(void* arg){              
     Reader* reader = (Reader*)arg;
     pthread_cleanup_push(buffer_thread_producer_cleanup,log_buffer)
     pthread_cleanup_push(buffer_thread_producer_cleanup,reader_buffer)
     pthread_cleanup_push(buffer_thread_producer_cleanup,reader_watch_buffer)
       
     while(1){
      
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          Data* data = reader_read_data(reader);
          void* data_ptr = &data;
          
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
       
          buffer_thread_log_producer(log_buffer,(char*){"READER START PRODUCING"});
          
          buffer_thread_producer(reader_buffer,data_ptr);
          
          buffer_thread_log_producer(log_buffer,(char*){"READER STOP PRODUCING"});
          
          void* dummy = NULL;
          void* dummy_ptr = &dummy;
          buffer_thread_producer(reader_watch_buffer, dummy_ptr);
     }
     
     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
}

static void* thread_analyzer(void* arg){          
     Analyzer* analyzer = (Analyzer*)arg;
     

     pthread_cleanup_push(buffer_thread_producer_cleanup,log_buffer)
     pthread_cleanup_push(buffer_thread_consumer_cleanup,reader_buffer)
     pthread_cleanup_push(buffer_thread_producer_cleanup,usage_buffer)
     pthread_cleanup_push(buffer_thread_producer_cleanup,analyzer_watch_buffer)
   
     while(1){
          Data* data = NULL;
          void* data_ptr = &data;
          
          buffer_thread_consumer(reader_buffer,data_ptr);
    
          buffer_thread_log_producer(log_buffer,(char*){"ANALYZER STOP CONSUMING"});

          float* usage_arr = analyzer_analyze_data(analyzer,data);
          void* usage_arr_ptr = &usage_arr;

          buffer_thread_log_producer(log_buffer,(char*){"ANALYZER START PRODUCING"});
          buffer_thread_producer(usage_buffer,usage_arr_ptr);
  
          buffer_thread_log_producer(log_buffer,(char*){"ANALYZER STOP PRODUCING"});
     
          void* dummy = NULL;
          void* dummy_ptr = &dummy;
          buffer_thread_producer(analyzer_watch_buffer,dummy_ptr);
     }

     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
}

static void* thread_printer(void* arg){          
     Printer* printer = (Printer*)arg;
     pthread_cleanup_push(buffer_thread_producer_cleanup,log_buffer)
     pthread_cleanup_push(buffer_thread_consumer_cleanup,usage_buffer)
     pthread_cleanup_push(buffer_thread_producer_cleanup,printer_watch_buffer)
     
     while(1){
          float* usage_data= NULL;
          void* usage_data_ptr = &usage_data;
          
          buffer_thread_log_producer(log_buffer,(char*){"PRINTER START CONSUMING"});
          
          buffer_thread_consumer(usage_buffer, usage_data_ptr);
         
          buffer_thread_log_producer(log_buffer,(char*){"PRINTER STOP CONSUMING"});
          
          printer_print_data(printer,usage_data);
          sleep(1);
               
          void* dummy_ptr =NULL;
          void* dummy_ptr_p = &dummy_ptr;
          buffer_thread_producer(printer_watch_buffer,dummy_ptr_p);
     }

     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
}


static void* thread_watchdog(void* arg){          
     Watchdog* wd = (Watchdog*)arg;
     struct timespec ts;
 
     pthread_cleanup_push(buffer_thread_consumer_cleanup,printer_watch_buffer)
     pthread_cleanup_push(buffer_thread_consumer_cleanup,reader_watch_buffer)
     pthread_cleanup_push(buffer_thread_consumer_cleanup,analyzer_watch_buffer)
     pthread_cleanup_push(buffer_thread_producer_cleanup,log_buffer)
    
     while(1){
   
          timespec_get(&ts, TIME_UTC);
          ts.tv_sec += WAIT_TIME_SEC;
          bool cancel_signal_sent = false;
          char str[20] = "";
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);

          buffer_watchdog_thread_consumer(reader_watch_buffer,&str,"READER ERROR", &cancel_signal_sent, &ts);
          
          buffer_watchdog_thread_consumer(analyzer_watch_buffer,&str,"ANALYZER ERROR",&cancel_signal_sent, &ts);
      
          buffer_watchdog_thread_consumer(printer_watch_buffer,&str,"PRINTER ERROR",&cancel_signal_sent, &ts);
      

          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();


          if(cancel_signal_sent){
            
               buffer_thread_log_producer(log_buffer,str);
               watchdog_cancel_threads(wd);
          }
     }
     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
     return NULL;
}

static void* thread_logger(void* arg){          
     Logger* logger = (Logger*)arg;
    
     pthread_cleanup_push(buffer_thread_consumer_cleanup,log_buffer)
  
     while(1){
      
          char* string = "";
          void* str_ptr = &string;
     
          buffer_thread_consumer(log_buffer,str_ptr);
          logger_log_data(logger,string);
     }
     
     pthread_cleanup_pop(1);
  
}

int main(void){

     Reader* reader = reader_new();
     Analyzer* analyzer = analyzer_new(reader_get_cpu_count(reader));
     Printer* printer = printer_new(reader_get_cpu_count(reader));
     Logger* logger = logger_new("Logs.txt");
     
   
     reader_buffer = buffer_new(sizeof(Data*),5);
     usage_buffer = buffer_new(sizeof(float*),5);
     reader_watch_buffer = buffer_new(sizeof(void*),1);
     analyzer_watch_buffer = buffer_new(sizeof(void*),1);
     printer_watch_buffer = buffer_new(sizeof(void*),1);

     log_buffer = buffer_new(sizeof(char*),5);
     pthread_t t_reader, t_analyzer, t_printer, t_watchdog, t_logger;

     watchdog = watchdog_new((const pthread_t*[]){&t_logger,&t_reader,&t_analyzer,&t_printer,&t_watchdog},5); 

     struct sigaction action;
     memset(&action, 0, sizeof(struct sigaction));
     action.sa_handler = term;
     sigaction(SIGTERM, &action, NULL);
     
     pthread_create(&t_reader, NULL, thread_reader, (void*)reader);
     pthread_create(&t_analyzer, NULL, thread_analyzer, (void*)analyzer);
     pthread_create(&t_printer, NULL, thread_printer, (void*)printer);
     pthread_create(&t_watchdog, NULL, thread_watchdog, (void*)watchdog);
      
     pthread_create(&t_logger, NULL, thread_logger, (void*)logger);

     
     pthread_join(t_reader, NULL);
     pthread_join(t_analyzer, NULL);
     pthread_join(t_printer,NULL);
     pthread_join(t_watchdog,NULL);
     pthread_join(t_logger, NULL);
     
     
     printf("THREADS JOINED ||| PROGRAM EXITED SUCCESSFULLY\n");
     
     buffer_delete(reader_buffer);
     buffer_delete(usage_buffer);
     buffer_delete(reader_watch_buffer);
     buffer_delete(analyzer_watch_buffer);
     buffer_delete(printer_watch_buffer);
     buffer_delete(log_buffer);


    
     printer_delete(printer);
     analyzer_delete(analyzer);
     reader_delete(reader);
     watchdog_delete(watchdog);
     logger_delete(logger);
     return 0;
}
