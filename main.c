
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "analyzer.h"
#include "printer.h"
#include "data.h"

#include "data_buffer.h"

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


Data_buffer* buffer = NULL;        //globalne     TODO: sprawdzanie czy buffer zainicjalizowany 
Data_buffer* usage_buffer = NULL;

void* thread_reader(void* arg){              
     Reader* reader = (Reader*)arg;
     while(1){
          pid_t tid = syscall(__NR_gettid);
          printf("[%d]READER\n",tid);
          
          printf("[%d]START WORKING\n",tid);
          Data* data_ptr = reader_read_data(reader);
          printf("[%d]END WORKING\n",tid);
          
          buffer_lock(buffer);               //SEKCJA KRYTYCZNA
               if(buffer_is_full(buffer)){
                    printf("[%d]BUFFER FULL, WAITING\n",tid);
                    buffer_wait_for_consumer(buffer);
               }

               printf("[%d]SPACE FOUND\n",tid);
               buffer_put(buffer,&data_ptr);           //ZMIANY  
               //free(data_ptr);
               buffer_call_consumer(buffer);
          buffer_unlock(buffer);
          //free(data_ptr);
          printf("[%d]GETTING OUT OF BUFFER\n",tid);

   }
    // return NULL;
}

void* thread_analyzer(void* arg){          
     Analyzer* analyzer = (Analyzer*)arg;
     while(1){
          pid_t tid = syscall(__NR_gettid);
          printf("[%d]ANALYZER\n",tid);
          
          
          
          buffer_lock(buffer);               //SEKCJA KRYTYCZNA
               if(buffer_is_empty(buffer)){
                    printf("[%d]BUFFER EMPTY, WAITING\n",tid);
                    buffer_wait_for_producer(buffer);
               }
               printf("[%d]GETTING FROM USAGE-BUFFER\n",tid);
               Data* data = NULL;
               buffer_get(buffer,&data);
               
               buffer_call_producer(buffer);
          buffer_unlock(buffer);

          printf("[%d]START ANALYZING\n",tid);
              
               float* usage_arr = analyzer_analyze_data(analyzer,data);


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
             //  free(data);
              printf("[%d] %f\n",tid,usage_arr[0]);
               sleep(1);
               
               printf("[%d]END ANALYZING\n",tid);
          printf("[%d]GETTING OUT OF BUFFER\n",tid);
          
    }
    // 
    // return NULL;
}

void* thread_printer(void* arg){          
     Printer* printer = (Printer*)arg;
     while(1){
          pid_t tid = syscall(__NR_gettid);
          printf("[%d]PRINTER\n",tid);
          
          
          
          buffer_lock(usage_buffer);               //SEKCJA KRYTYCZNA
               if(buffer_is_empty(usage_buffer)){
                    printf("[%d]USAGE-BUFFER EMPTY, WAITING\n",tid);
                    buffer_wait_for_producer(usage_buffer);
               }
               printf("[%d]GETTING FROM USAGE-BUFFER\n",tid);
               float* usage_data = NULL;
               buffer_get(usage_buffer,&usage_data);
               
               buffer_call_producer(usage_buffer);
          buffer_unlock(usage_buffer);

          printf("[%d]START PRINTING\n",tid);
              
              
               printer_print_data(printer,usage_data);
               sleep(1);
               
               printf("[%d]END PRINTING\n",tid);
          printf("[%d]GETTING OUT OF USAGE-BUFFER\n",tid);
          
    }
    // 
    // return NULL;
}


int main(void){

     
     Reader* reader = reader_new();
     Analyzer* analyzer = analyzer_new(reader_get_cpu_count(reader));
     Printer* printer = printer_new(reader_get_cpu_count(reader));
     buffer = data_buffer_new(sizeof(Data*));
     usage_buffer = data_buffer_new(sizeof(float*));
     pthread_t t_reader, t_analyzer, t_printer;
     pthread_create(&t_reader, NULL, thread_reader, (void*)reader);
     pthread_create(&t_analyzer, NULL, thread_analyzer, (void*)analyzer);
     pthread_create(&t_printer, NULL, thread_printer, (void*)printer);

     pthread_join(t_reader, NULL);
     pthread_join(t_analyzer, NULL);
     pthread_join(t_printer,NULL);
     data_buffer_delete(buffer);
     data_buffer_delete(usage_buffer);
     //Data* arr = calloc(reader_get_cpu_count(reader),sizeof(Data));
     /*
     while (1)
     {    
          sleep(1);
          Data* arr = reader_read_data(reader);
    
     float* ar = analyzer_analyze_data(analyzer,arr);
     
     printer_print_data(printer, ar);
     }
     
     */
     
    
    //printf("DDDD%zu\n",sizeof(Reader));
  // uint32_t arr [13][10] = {0};
   // memcpy(arr,reader_read_data(reader),sizeof(arr));
    //memcpy(arr,reader_read_data(reader),sizeof(arr)
    /*
    while(1){
        sleep(1);

       
         uint32_t* arr = reader_read_data(reader);
    
   // printf("EESIZE: %zu\n",sizeof(*arr));
  //  for(size_t i = 0; i < reader_get_cpu_count(reader) * 10; i++){

       // for(size_t k = 0; k<10; k++){
        //    printf("[%d]", arr[i]);
        //}
   //     if((i+1)%10 == 0){printf("\n");}
        
 //   }

    float* ar = analyzer_analyze_data(analyzer,arr);
    
    for(size_t i = 0; i < 13; i++){

    
         printf("[%f]", ar[i]);
        
        
    }
    printf("\n");
    }
   */
    //printf("%zu\n",sizeof(*reader));
    //reader_read_data(reader);
 
    //Reader tab[10];


    
    printer_delete(printer);
    analyzer_delete(analyzer);
    reader_delete(reader);
    return 0;
}
