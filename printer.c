#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "printer.h"

struct Printer{
    uint8_t cpu_count;
};

Printer* printer_new(uint8_t cpu_count){
    Printer* p = calloc(1,sizeof(Printer));
    p->cpu_count = cpu_count;
    return p;
}

void printer_delete(Printer* printer){
    free(printer);
}

void printer_print_data(Printer* printer, float usage_array[]){
    printf("| Total CPU usage: %.1f%% ", (double)usage_array[0]);
    
    for(size_t i = 0; i < printer->cpu_count; i++){
        printf("| CPU(%zu) usage: %.1f%% ",i, (double)usage_array[i]);
    }

    printf("\n");
}
