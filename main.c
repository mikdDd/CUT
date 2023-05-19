#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "analyzer.h"
#include "printer.h"
#include "data.h"
#include <time.h>

int main(void){

    
    Reader* reader = reader_new();
    Analyzer* analyzer = analyzer_new(reader_get_cpu_count(reader));
    Printer* printer = printer_new(reader_get_cpu_count(reader));

     //Data* arr = calloc(reader_get_cpu_count(reader),sizeof(Data));
     
     while (1)
     {    
          sleep(1);
          Data* arr = reader_read_data(reader);
    
     float* ar = analyzer_analyze_data(analyzer,arr);
     
     printer_print_data(printer, ar);
     }
     
     
     
    
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
