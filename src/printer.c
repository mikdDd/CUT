#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "printer.h"
/**
 * @brief Printer struct containing cpu count
 * 
 */
struct Printer{
    uint8_t cpu_count;
};

/**
 * @brief Function that creates a new Printer
 * 
 * @param cpu_count count of CPUs for which we want to print
 * @return Printer* pointer to new Printer
 */
Printer* printer_new(const uint8_t cpu_count){
    if(cpu_count == 0)return NULL;

    Printer* p = calloc(1,sizeof(Printer));
    if(p == NULL)return NULL;
    p->cpu_count = cpu_count;
    return p;
}

void printer_delete(Printer* const printer){
    if(printer == NULL)return;
    free(printer);
}

void printer_print_data(const Printer* const printer, const float usage_array[const]){
    if(printer == NULL)return;
    if(usage_array == NULL)return;
    
    printf("| Total CPU usage: %.1f%% ", (double)usage_array[0]);
    
    for(size_t i = 0; i < printer->cpu_count; i++){
        printf("| CPU(%zu) usage: %.1f%% ",i, (double)usage_array[i]);
    }

    printf("\n");
}
