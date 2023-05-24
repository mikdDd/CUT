#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "analyzer.h"

struct Analyzer
{                   //sprawdzic czy zgodne z FAM
    uint8_t cpu_count;                                                                        // [0]    [1]     [2]   [3]    [4]   [5]    [6]      [7]    [8]     [9] 
    Data* prev_values;      //[0] = prev_user; [1] = prev_nice; [2] = prev_system [3] ... // user  nice   system  idle  iowait  irq  softirq  steal  guest  guest_nice
    float usage_array[];
};

Analyzer* analyzer_new(uint8_t cpu_count){
    Analyzer* a = calloc(1,sizeof(Analyzer) + sizeof(float)*cpu_count);
    a->cpu_count = cpu_count;
    a->prev_values = calloc(cpu_count, sizeof(Data));
    return a;
}
void analyzer_delete(Analyzer* analyzer){
    free(analyzer->prev_values);
    free(analyzer);
}

float* analyzer_analyze_data(Analyzer* analyzer, Data arr[]){
    float ret;
    if(arr == NULL)return NULL;
    if(analyzer == NULL)return NULL;
    for(size_t i = 0; i < analyzer->cpu_count; i++){

        uint32_t prev_idle = analyzer->prev_values[i].idle + analyzer->prev_values[i].iowait;
        
        uint32_t idle = arr[i].idle + arr[i].iowait;

        uint32_t prev_non_idle = analyzer->prev_values[i].user + analyzer->prev_values[i].nice + analyzer->prev_values[i].system + analyzer->prev_values[i].irq + analyzer->prev_values[i].softirq + analyzer->prev_values[i].steal;
        uint32_t non_indle = arr[i].user + arr[i].nice + arr[i].system + arr[i].irq + arr[i].softirq + arr[i].steal;

        uint32_t prev_total = prev_idle + prev_non_idle;
        uint32_t total = idle + non_indle;

        uint32_t total_diff = total - prev_total;
        uint32_t idle_diff = idle - prev_idle;
     
        if(total_diff != idle_diff){
            ret = ((float)(total_diff - idle_diff)/(float)total_diff);
        } else {
            ret = 0.0;
        }
            
            
            analyzer->usage_array[i] = ret * 100;
    }

    memcpy(analyzer->prev_values, arr, analyzer->cpu_count * sizeof(Data));
    
    return analyzer->usage_array;
}
