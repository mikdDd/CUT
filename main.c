
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

#define WAIT_TIME_SEC 2

Data_buffer* buffer = NULL;        //globalne     TODO: sprawdzanie czy buffer zainicjalizowany 
Data_buffer* usage_buffer = NULL;
Data_buffer* watch_buffer = NULL;



void* thread_reader(void* arg){              
     Reader* reader = (Reader*)arg;
      pthread_cleanup_push(thread__producer_cleanup,buffer);
     while(1){

          pid_t tid = syscall(__NR_gettid);
          printf("[%d]READER\n",tid);
          printf("[%d]START WORKING\n",tid);
          Data* data_ptr = reader_read_data(reader);
          printf("[%d]END WORKING\n",tid);
          //pthread_setcancelstate()
         
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(buffer);               //SEKCJA KRYTYCZNA
               if(buffer_is_full(buffer)){
                    printf("[%d]BUFFER FULL, WAITING\n",tid);
                    buffer_wait_for_consumer(buffer);
               }
             //  void* ptr = malloc(sizeof(Data));
               printf("[%d]SPACE FOUND\n",tid);
               buffer_put(buffer,&data_ptr);        
              // free(ptr);
               buffer_call_consumer(buffer);
          


          buffer_unlock(buffer);
          printf("R CANCELED\n");
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
          printf("[%d]GETTING OUT OF BUFFER\n",tid);
          
          


          

   }
   pthread_cleanup_pop(1);
  // pthread_cleanup_pop(1);
    // return NULL;
}

void* thread_analyzer(void* arg){          
     Analyzer* analyzer = (Analyzer*)arg;
    // pthread_cleanup_push(thread_cleanup,buffer);
    // pthread_cleanup_push(thread_cleanup,usage_buffer);
    pthread_cleanup_push(thread__consumer_cleanup,buffer);
    pthread_cleanup_push(thread__producer_cleanup,usage_buffer);
     while(1){
          pid_t tid = syscall(__NR_gettid);
          printf("[%d]ANALYZER\n",tid);
          
          
          
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          
          buffer_lock(buffer);               //SEKCJA KRYTYCZNA
          sleep(2);
               if(buffer_is_empty(buffer)){
                    printf("[%d]BUFFER EMPTY, WAITING\n",tid);
                    buffer_wait_for_producer(buffer);
               }
               printf("[%d]GETTING FROM BUFFER\n",tid);
               Data* data = NULL;
               buffer_get(buffer,&data);
               
               buffer_call_producer(buffer);
          buffer_unlock(buffer);
          printf("AN CANCELED\n");
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
          

          printf("[%d]START ANALYZING\n",tid);
              
               float* usage_arr = analyzer_analyze_data(analyzer,data);

          

          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(usage_buffer);
               if(buffer_is_full(usage_buffer)){
                    printf("[%d]USAGE-BUFFER FULL, WAITING\n",tid);
                    buffer_wait_for_consumer(usage_buffer);
               }

               printf("[%d]SPACE IN UB FOUND\n",tid);
               buffer_put(usage_buffer,&usage_arr);           //ZMIANY
               //free(data_ptr);
               buffer_call_consumer(usage_buffer);

          buffer_unlock(usage_buffer);
          printf("analyz CANCELED\n");
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
             //  free(data);
              printf("[%d] %f\n",tid,usage_arr[0]);
               sleep(1);
               
               printf("[%d]END ANALYZING\n",tid);
          printf("[%d]GETTING OUT OF BUFFER\n",tid);
          
    }
  //  pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    // 
    // return NULL;
}

void* thread_printer(void* arg){          
     Printer* printer = (Printer*)arg;
     pthread_cleanup_push(thread__consumer_cleanup,usage_buffer);
     pthread_cleanup_push(thread__producer_cleanup,watch_buffer);
     while(1){
          pid_t tid = syscall(__NR_gettid);
          printf("[%d]PRINTER\n",tid);
          
          float* usage_data = NULL;
          
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(usage_buffer);               //SEKCJA KRYTYCZNA
               if(buffer_is_empty(usage_buffer)){
                    printf("[%d]USAGE-BUFFER EMPTY, WAITING\n",tid);
                    buffer_wait_for_producer(usage_buffer);
               }
               printf("[%d]GETTING FROM USAGE-BUFFER\n",tid);
               
               buffer_get(usage_buffer,&usage_data);
               
               buffer_call_producer(usage_buffer);
          buffer_unlock(usage_buffer);
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();

          printf("[%d]START PRINTING\n",tid);
              
              
               printer_print_data(printer,usage_data);
               sleep(1);
               
               printf("[%d]END PRINTING\n",tid);
          printf("[%d]GETTING OUT OF USAGE-BUFFER\n",tid);

          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(watch_buffer);
               printf("[%d]PUTTING IN READERBUFF\n",tid);
               if(buffer_is_full(watch_buffer)){
                    printf("[%d]CHECKING!!!!\n",tid);
                   buffer_wait_for_consumer(watch_buffer);
                   
                   
               } else{

                    printf("[%d]HERE!!!!\n",tid);
                    buffer_put(watch_buffer, usage_data);
                    buffer_call_consumer(watch_buffer);
               }
              
          buffer_unlock(watch_buffer);
           printf("[%d]HERE2\n",tid);
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
          
    }
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
}


void* thread_watchdog(void* arg){          
     Watchdog* watchdog = (Watchdog*)arg;
     struct timespec ts;
     timespec_get(&ts, TIME_UTC);
     //ts.tv_sec +=2;
     struct timeval tp;
     int rc = 0;
     bool end = false;
    pthread_cleanup_push(thread__consumer_cleanup,watch_buffer);
    
     while(!end){
          pid_t tid = syscall(__NR_gettid);
          printf("[%d]WATCHDOG\n",tid);
          
          
          //ts.tv_sec = tp.tv_sec;
         // ts.tv_nsec = tp.tv_usec * 1000;
         // ts.tv_sec += WAIT_TIME_SEC;
          timespec_get(&ts, TIME_UTC);
          ts.tv_sec +=2;
          printf("SEKKRYT WATCHODG\n");
          pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(watch_buffer);               //SEKCJA KRYTYCZNA
          printf("[%d]LOCK?\n",tid);

               if(buffer_is_empty(watch_buffer)){
                  printf("[%d]READER-INFO BUFFER EMPTY, WAITING\n",tid);
                  rc = buffer_wait_for_producer_timedwait(watch_buffer,&ts);
                    //buffer_wait_for_producer(watch_reader_buffer);
               }
               if(rc == ETIMEDOUT){
                    printf("BRAK INFORMACJI PRZEZ 2 SEKUNDY\n");
                    
                    Data* dummy_ptr = NULL;
                    buffer_get(watch_buffer,&dummy_ptr);
                    //end = true;
                    buffer_call_producer(watch_buffer);
                    
                    watchdog_cancel_threads(watchdog);
                    //pthread_cancel(pthread_self());
                    // pthread_exit(NULL);
               } else {
                    
                   printf("UZYSKANO INFORMACJE\n");
                    Data* dummy_ptr = NULL;
                    buffer_get(watch_buffer,&dummy_ptr);
                    buffer_call_producer(watch_buffer);
               }
               
              
          buffer_unlock(watch_buffer);
          printf("WD CANCELED\n");
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
    }
    pthread_cleanup_pop(1);
}


int main(void){
    
     Reader* reader = reader_new();
     Analyzer* analyzer = analyzer_new(reader_get_cpu_count(reader));
     Printer* printer = printer_new(reader_get_cpu_count(reader));
     
   
     buffer = data_buffer_new(sizeof(Data*),5);
     usage_buffer = data_buffer_new(sizeof(float*),5);
     watch_buffer = data_buffer_new(sizeof(Data*),1);
     pthread_t t_reader, t_analyzer, t_printer, t_watchdog;

     Watchdog* watchdog = watchdog_new((pthread_t*[]){&t_reader,&t_analyzer,&t_printer,&t_watchdog},4);         //COMPOUND LITERAL

     pthread_create(&t_reader, NULL, thread_reader, (void*)reader);
     pthread_create(&t_analyzer, NULL, thread_analyzer, (void*)analyzer);
     pthread_create(&t_printer, NULL, thread_printer, (void*)printer);
     pthread_create(&t_watchdog, NULL, thread_watchdog, (void*)watchdog);

     printf("JOINING START\n");
     pthread_join(t_reader, NULL);
     printf("READER JOINED\n");
     pthread_join(t_analyzer, NULL);
     printf("ANALYZER JOINED\n");
     pthread_join(t_printer,NULL);
     printf("PRINTER JOINED\n");
     pthread_join(t_watchdog,NULL);
     printf("WATCHD JOINED\n"); 
     printf("!!!!!!!!!!!!!!!![THREADS JOINED]!!!!!!!!!!!!!!!!!!!!\n");
     
     data_buffer_delete(buffer);
     data_buffer_delete(usage_buffer);

     data_buffer_delete(watch_buffer);
     


    
    printer_delete(printer);
    analyzer_delete(analyzer);
    reader_delete(reader);
    watchdog_delete(watchdog);
    return 0;
}
