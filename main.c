#define _GNU_SOURCE

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "analyzer.h"
#include "printer.h"
#include "data.h"
#include "watchdog.h"
#include "data_buffer.h"
#include <errno.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "logger.h"
#include <string.h>
#include <signal.h>
#include <sys/signal.h>
#include <sys/time.h>

#define WAIT_TIME_SEC 2
//#define watchdog_term(...) term(EXTRA, __VA_ARGS__)

static void* thread_reader(void*);
static void* thread_analyzer(void*);
static void* thread_printer(void*);
static void* thread_logger(void*);
static void* thread_watchdog(void*);
static void term (int signum);


static Data_buffer* reader_buffer = NULL;        //globalne     TODO: sprawdzanie czy buffer zainicjalizowany 
static Data_buffer* usage_buffer = NULL;
static Data_buffer* reader_watch_buffer = NULL;
static Data_buffer* analyzer_watch_buffer = NULL;
static Data_buffer* printer_watch_buffer = NULL;
static Data_buffer* log_buffer = NULL;
static Watchdog* watchdog = NULL;




static void term (int signum){
     
     watchdog_cancel_threads(watchdog);
}
// void  (*f(Watchdog* wd))(int signum) tt{
     

//      return &term;
// }


static void* thread_reader(void* arg){              
     Reader* reader = (Reader*)arg;
     pthread_cleanup_push(thread__producer_cleanup,log_buffer)
     pthread_cleanup_push(thread__producer_cleanup,reader_buffer)
     pthread_cleanup_push(thread__producer_cleanup,reader_watch_buffer)
       
     while(1){
        //  char* str = "";
          //str = "READER THREAD START";
          
        //  sleep(3);
         // pid_t tid = syscall(__NR_gettid);
        //  printf("[%d]READER\n",tid);
        //  printf("[%d]START WORKING\n",tid);
          Data* data_ptr = reader_read_data(reader);
        //  printf("[%d]END WORKING\n",tid);
          //pthread_setcancelstate()
         thread__log_producer_put_to_buffer(log_buffer,(char*){"READER START PRODUCING"});
         
         buffer_thread_producer(reader_buffer,data_ptr);
          
          thread__log_producer_put_to_buffer(log_buffer,(char*){"READER STOP PRODUCING"});
          
          void* dummy_ptr;
          buffer_thread_producer(reader_watch_buffer, dummy_ptr);
          // pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          // buffer_lock(reader_watch_buffer);
          // if(!buffer_is_to_deletion(reader_watch_buffer)){
          //      if(buffer_is_full(reader_watch_buffer)){
          // //  printf("[%d]PRIONTER W* FOR CUSTOMER\n",tid);
                
          //          buffer_wait_for_consumer(reader_watch_buffer);
                   
                   
          //      }
                    
          //           void* dummy_ptr;
          //           buffer_put(reader_watch_buffer, &dummy_ptr);
                    
          //           buffer_call_consumer(reader_watch_buffer);
          
          // }
               
              
          // buffer_unlock(reader_watch_buffer);

          // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          // pthread_testcancel();

/*
r
*/
          
          
          // pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          // buffer_lock(log_buffer);               //SEKCJA KRYTYCZNA
          //      if(buffer_is_full(log_buffer)){
          //           buffer_wait_for_consumer(log_buffer);
          //      }
               
          //      buffer_put(log_buffer,&str);        
            
          //      buffer_call_consumer(log_buffer);
          
          // buffer_unlock(log_buffer);
          // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          // pthread_testcancel();
          
          


          

   }
   pthread_cleanup_pop(1);
   pthread_cleanup_pop(1);
   pthread_cleanup_pop(1);
  // pthread_cleanup_pop(1);
    // return NULL;
}

static void* thread_analyzer(void* arg){          
     Analyzer* analyzer = (Analyzer*)arg;
     
    // pthread_cleanup_push(thread_cleanup,buffer);
    // pthread_cleanup_push(thread_cleanup,usage_buffer);
     pthread_cleanup_push(thread__producer_cleanup,log_buffer)
    pthread_cleanup_push(thread__consumer_cleanup,reader_buffer)
    pthread_cleanup_push(thread__producer_cleanup,usage_buffer)
    pthread_cleanup_push(thread__producer_cleanup,analyzer_watch_buffer)
   
     while(1){
          
         // printf("[%d]ANALYZER\n",tid);
      //    thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER START CONSUMING"});
          Data* data = NULL;
         //  buffer_thread_consumer(reader_buffer,data);
     //      thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER START CONSUMING"});
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);

          //char* str = "";
          //pid_t tid = syscall(__NR_gettid);
         // Data* data = NULL;
          //sleep(2);
        //  printf("[%d]ANALYZER BEFORE MUTEX\n",tid);
          buffer_lock(reader_buffer);               //SEKCJA KRYTYCZNA
       //  printf("[%d]ANALYZER LOCKED BUFFER\n",tid);
         if(!buffer_is_to_deletion(reader_buffer)){
               if(buffer_is_empty(reader_buffer)){
                //    printf("[%d]BUFFER EMPTY, WAITING\n",tid);
        //         thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER WAITING FOR READER"});
                    buffer_wait_for_producer(reader_buffer);
               }
           //   printf("[%d]GETTING FROM BUFFER\n",tid);
              // str = "ANALYZER GET FROM BUFFER";
               
               buffer_get(reader_buffer,&data);
       //        thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER CONSUMING"});
               buffer_call_producer(reader_buffer);
           //    printf("[%d]ANALYZER PRODUCER CALLED\n",tid);
         }
          buffer_unlock(reader_buffer);

      //    printf("[%d]ANALYZER AFTER MUTEX\n",tid);
          //printf("AN CANCELED\n");



          
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          
          pthread_testcancel();
          thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER STOP CONSUMING"});

         // printf("[%d]START ANALYZING\n",tid);
              float* usage_arr = analyzer_analyze_data(analyzer,data);
               

          
          thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER START PRODUCING"});
         // buffer_thread_producer(usage_buffer,usage_arr);
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(usage_buffer);
          if(!buffer_is_to_deletion(usage_buffer)){
               if(buffer_is_full(usage_buffer)){
              //     printf("[%d]USAGE-BUFFER FULL, WAITING\n",tid);
     //               thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER WAITING FOR PRINTER"});
                    buffer_wait_for_consumer(usage_buffer);
               }
              // str = "ANALYZER PUT TO BUFFER";
             //  printf("[%d]SPACE IN UB FOUND\n",tid);
               buffer_put(usage_buffer,&usage_arr);           //ZMIANY
     //          thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER PRODUCING"});
               //free(data_ptr);
               buffer_call_consumer(usage_buffer);

          }
               
          buffer_unlock(usage_buffer);
         // printf("analyz CANCELED\n");
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
          thread__log_producer_put_to_buffer(log_buffer,(char*){"ANALYZER STOP PRODUCING"});
             //  free(data);
             // printf("[%d] %f\n",tid,usage_arr[0]);
              // sleep(1);
               
             //  printf("[%d]END ANALYZING\n",tid);
       //   printf("[%d]GETTING OUT OF BUFFER\n",tid);


           pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(analyzer_watch_buffer);
          if(!buffer_is_to_deletion(analyzer_watch_buffer)){
               if(buffer_is_full(analyzer_watch_buffer)){
          //  printf("[%d]PRIONTER W* FOR CUSTOMER\n",tid);
                
                   buffer_wait_for_consumer(analyzer_watch_buffer);
                   
                   
               }
                    
                    void* dummy_ptr;
                    buffer_put(analyzer_watch_buffer, &dummy_ptr);
                    
                    buffer_call_consumer(analyzer_watch_buffer);
          
          }
               
              
          buffer_unlock(analyzer_watch_buffer);

          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();



          
    }
  //  pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
   pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
     pthread_cleanup_pop(1);
    // 
    // return NULL;
}

static void* thread_printer(void* arg){          
     Printer* printer = (Printer*)arg;
     pthread_cleanup_push(thread__producer_cleanup,log_buffer)
     pthread_cleanup_push(thread__consumer_cleanup,usage_buffer)
     pthread_cleanup_push(thread__producer_cleanup,printer_watch_buffer)
     
     while(1){
          
         // pid_t tid = syscall(__NR_gettid);
       //   printf("[%d]PRINTER\n",tid);
     //   sleep(2);
          float* usage_data = NULL;

          thread__log_producer_put_to_buffer(log_buffer,(char*){"PRINTER START CONSUMING"});
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
           
          buffer_lock(usage_buffer);               //SEKCJA KRYTYCZNA
          if(!buffer_is_to_deletion(usage_buffer)){
                if(buffer_is_empty(usage_buffer)){
                 //   printf("[%d]USAGE-BUFFER EMPTY, WAITING\n",tid);
              //  thread__log_producer_put_to_buffer(log_buffer,(char*){"PRINTER WAITING FOR ANALYZER"});
                    buffer_wait_for_producer(usage_buffer);
               }
          //    printf("[%d]GETTING FROM USAGE-BUFFER\n",tid);
               if(buffer_is_empty(usage_buffer))printf("WTF\n");
               buffer_get(usage_buffer,&usage_data);
            //   thread__log_producer_put_to_buffer(log_buffer,(char*){"PRINTER CONSUMING"});
               buffer_call_producer(usage_buffer);
          }
              
               
          buffer_unlock(usage_buffer);
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
     thread__log_producer_put_to_buffer(log_buffer,(char*){"PRINTER STOP CONSUMING"});
      //    printf("[%d]START PRINTING\n",tid);
              
              
               printer_print_data(printer,usage_data);
               sleep(1);
               
            //   printf("[%d]END PRINTING\n",tid);
        //  printf("[%d]GETTING OUT OF USAGE-BUFFER\n",tid);
          void* dummy_ptr;
          buffer_thread_producer(printer_watch_buffer,dummy_ptr);
          // pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          // buffer_lock(printer_watch_buffer);
          // if(!buffer_is_to_deletion(printer_watch_buffer)){
          //      if(buffer_is_full(printer_watch_buffer)){
          //  // printf("[%d]PRIONTER W* FOR CUSTOMER\n",tid);
                
          //          buffer_wait_for_consumer(printer_watch_buffer);
                   
                   
          //      }
                    
          //           void* dummy_ptr;
          //           buffer_put(printer_watch_buffer, &dummy_ptr);
                    
          //           buffer_call_consumer(printer_watch_buffer);
          
          // }
               
              
          // buffer_unlock(printer_watch_buffer);

          // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          // pthread_testcancel();


         


          
    }
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}


static void* thread_watchdog(void* arg){          
     Watchdog* wd = (Watchdog*)arg;
     struct timespec ts;
     timespec_get(&ts, TIME_UTC);
     //ts.tv_sec +=2;
     //struct timeval tp;
     int rc = 0;
     //bool end = false;
    pthread_cleanup_push(thread__consumer_cleanup,printer_watch_buffer)
    pthread_cleanup_push(thread__consumer_cleanup,reader_watch_buffer)
    pthread_cleanup_push(thread__consumer_cleanup,analyzer_watch_buffer)
    pthread_cleanup_push(thread__producer_cleanup,log_buffer)
    
     while(1){
        //  pid_t tid = syscall(__NR_gettid);
         
          timespec_get(&ts, TIME_UTC);
          ts.tv_sec += WAIT_TIME_SEC;
          bool cancel_signal_sent = false;
          char str [20] = "";
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);


           buffer_lock(reader_watch_buffer);               //SEKCJA KRYTYCZNA

          if(!buffer_is_to_deletion(reader_watch_buffer) && !cancel_signal_sent){
               if(buffer_is_empty(reader_watch_buffer)){
                 //   printf("WATCHJDOG WAITING\n");
                  rc = buffer_wait_for_producer_timedwait(reader_watch_buffer,&ts);
               
               }
               if(rc == ETIMEDOUT){
            
                    cancel_signal_sent = true;
                    strcpy(str,"READER ERROR");
            
                    
               } else {
                    
                
                    void* dummy_ptr;
                    buffer_get(reader_watch_buffer,&dummy_ptr);
                    buffer_call_producer(reader_watch_buffer);
                }
               
          }
               
              
          buffer_unlock(reader_watch_buffer);



         
         
       //   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
       //   pthread_testcancel();

        //  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
         
         
        //  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
         // pthread_testcancel();


       //  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(analyzer_watch_buffer);               //SEKCJA KRYTYCZNA

          if(!buffer_is_to_deletion(analyzer_watch_buffer) && !cancel_signal_sent){
               if(buffer_is_empty(analyzer_watch_buffer)){
                //    printf("WATCHJDOG WAITING\n");
                  rc = buffer_wait_for_producer_timedwait(analyzer_watch_buffer,&ts);
               
               }
               if(rc == ETIMEDOUT){
            
                    cancel_signal_sent = true;
                    strcpy(str,"ANALYZER ERROR");
            
                    
               } else {
                    
                
                    void* dummy_ptr;
                    buffer_get(analyzer_watch_buffer,&dummy_ptr);
                    buffer_call_producer(analyzer_watch_buffer);
                }
               
          }
               
              
          buffer_unlock(analyzer_watch_buffer);
        //  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE);
         //pthread_testcancel();


           buffer_lock(printer_watch_buffer);               //SEKCJA KRYTYCZNA

          if(!buffer_is_to_deletion(printer_watch_buffer)&& !cancel_signal_sent){
               if(buffer_is_empty(printer_watch_buffer)){
                 //   printf("WATCHJDOG WAITING\n");
                  rc = buffer_wait_for_producer_timedwait(printer_watch_buffer,&ts);
               
               }
               if(rc == ETIMEDOUT){
            
                    cancel_signal_sent = true;
                    strcpy(str,"PRINTER ERROR");
            
                    
               } else {
                    
                
                    void* dummy_ptr;
                    buffer_get(printer_watch_buffer,&dummy_ptr);
                    buffer_call_producer(printer_watch_buffer);
                }
               
          }
               
              
          buffer_unlock(printer_watch_buffer);

          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();


          if(cancel_signal_sent){
               thread__log_producer_put_to_buffer(log_buffer,str);
        
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
    
     pthread_cleanup_push(thread__consumer_cleanup,log_buffer)
     //pthread_cleanup_push(thread__consumer_cleanup,analyzer_log_buffer);
    // pthread_cleanup_push(thread__consumer_cleanup,printer_log_buffer);
    
     while(1){
          //pid_t tid = syscall(__NR_gettid);
        //  printf("[%d]LOGGER START\n",tid);
          char* string = "";
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(log_buffer);               //SEKCJA KRYTYCZNA
       //   printf("[%d]LOGGER ENTERING BUFFER\n",tid);
          if(!buffer_is_to_deletion(log_buffer)){
                if(buffer_is_empty(log_buffer)){
             //     printf("[%d]LOG-BUFFER EMPTY, WAITING\n",tid);
                    buffer_wait_for_producer(log_buffer);
               }
         //    printf("[%d]GETTING FROM LOGBUFFER\n",tid);
               
               buffer_get(log_buffer,&string);
               
               buffer_call_producer(log_buffer);
          }
               
               
          buffer_unlock(log_buffer);
          
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
     
          logger_log_data(logger,string);
         
    }
      pthread_cleanup_pop(1);
     //   pthread_cleanup_pop(1);
     //    pthread_cleanup_pop(1);
}

int main(void){

     printf("SIZE OF CHAR* = %zu\n",sizeof(char*));
     printf("SIZE OF SIZE_T = %zu\n",sizeof(size_t));
     printf("SIZE OF PTHREAD_COND_T = %zu\n",sizeof(pthread_cond_t));
     printf("SIZE OF PTHREAD_MUTEX_T = %zu\n",sizeof(pthread_mutex_t));
     printf("SIZE OF bool = %zu\n",sizeof(bool));
     
//      pthread_mutex_t mutex;
//     pthread_cond_t can_produce;
//     pthread_cond_t can_consume;
//     bool is_to_deletion;
//     uint8_t buffer[];   //FAM



    
  
     Reader* reader = reader_new();
     Analyzer* analyzer = analyzer_new(reader_get_cpu_count(reader));
     Printer* printer = printer_new(reader_get_cpu_count(reader));
     Logger* logger = logger_new("Logs.txt");
     
   
     reader_buffer = data_buffer_new(sizeof(Data*),5);
     usage_buffer = data_buffer_new(sizeof(float*),5);
     reader_watch_buffer = data_buffer_new(sizeof(void*),1);
     analyzer_watch_buffer = data_buffer_new(sizeof(void*),1);
     printer_watch_buffer = data_buffer_new(sizeof(void*),1);

     log_buffer = data_buffer_new(sizeof(char*),5);
   //  analyzer_log_buffer = data_buffer_new(sizeof(char*),5);
    // printer_log_buffer = data_buffer_new(sizeof(char*),5);
     pthread_t t_reader, t_analyzer, t_printer, t_watchdog, t_logger;

     watchdog = watchdog_new((pthread_t*[]){&t_logger,&t_reader,&t_analyzer,&t_printer,&t_watchdog},5);         //COMPOUND LITERAL

      struct sigaction action;
     memset(&action, 0, sizeof(struct sigaction));
     action.sa_handler = term;
     sigaction(SIGTERM, &action, NULL);
     pthread_create(&t_reader, NULL, thread_reader, (void*)reader);
     pthread_create(&t_analyzer, NULL, thread_analyzer, (void*)analyzer);
     pthread_create(&t_printer, NULL, thread_printer, (void*)printer);
     pthread_create(&t_watchdog, NULL, thread_watchdog, (void*)watchdog);
      
     pthread_create(&t_logger, NULL, thread_logger, (void*)logger);

     
     //pthread_cancel(t_logger);
     printf("JOINING START\n");
     pthread_join(t_reader, NULL);
     printf("READER JOINED\n");
     pthread_join(t_analyzer, NULL);
     printf("ANALYZER JOINED\n");
     pthread_join(t_printer,NULL);
     printf("PRINTER JOINED\n");
     pthread_join(t_watchdog,NULL);
pthread_join(t_logger, NULL);
     
     printf("WATCHD JOINED\n"); 
     
     printf("!!!!!!!!!!!!!!!![THREADS JOINED]!!!!!!!!!!!!!!!!!!!!\n");
     
   data_buffer_delete(reader_buffer);
    data_buffer_delete(usage_buffer);

    data_buffer_delete(reader_watch_buffer);
    data_buffer_delete(analyzer_watch_buffer);
    data_buffer_delete(printer_watch_buffer);
    data_buffer_delete(log_buffer);


    
    printer_delete(printer);
    analyzer_delete(analyzer);
    reader_delete(reader);
    watchdog_delete(watchdog);
    logger_delete(logger);
    return 0;
}
