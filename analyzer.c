#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "analyzer.h"

struct Analyzer
{   
    uint8_t cpu_count;                                                                        // [0]    [1]     [2]   [3]    [4]   [5]    [6]      [7]    [8]     [9] 
    uint32_t* prev_values;      //[0] = prev_user; [1] = prev_nice; [2] = prev_system [3] ... // user  nice   system  idle  iowait  irq  softirq  steal  guest  guest_nice
    float usage_array[];
    
};

Analyzer* analyzer_new(uint8_t cpu_count){
    Analyzer* a = calloc(1,sizeof(Analyzer) + sizeof(float)*cpu_count);
    a->cpu_count = cpu_count;
    a->prev_values = calloc(cpu_count * 10, sizeof(uint32_t));
    return a;
}
void analyzer_delete(Analyzer* analyzer){
    free(analyzer->prev_values);
    free(analyzer);
}

float* analyzer_analyze_data(Analyzer* analyzer, uint32_t arr[]){
                   
    for(size_t i = 0; i < analyzer->cpu_count; i++){
        uint32_t prev_idle = analyzer->prev_values[i*10 + 3] + analyzer->prev_values[i*10 + 4];     //10 bo taka wielkość wiersza w proc/stat
        
        uint32_t idle = arr[i*10 + 3] + arr[i*10 + 4];

        uint32_t prev_non_idle = analyzer->prev_values[i*10 + 0] + analyzer->prev_values[i*10 + 1] + analyzer->prev_values[i*10 + 2] + analyzer->prev_values[i*10 + 5] + analyzer->prev_values[i*10 + 6] + analyzer->prev_values[i*10 + 7];
        uint32_t non_indle = arr[i*10 + 0] + arr[i*10 + 1] + arr[i*10 + 2] + arr[i*10 + 5] + arr[i*10 + 6] + arr[i*10 + 7];

        uint32_t prev_total = prev_idle + prev_non_idle;
        uint32_t total = idle + non_indle;

        uint32_t total_diff = total - prev_total;
        uint32_t idle_diff = idle - prev_idle;
      //  if(total_diff != 0){
            float debug_t = ((float)(total_diff - idle_diff)/total_diff);
            analyzer->usage_array[i] = debug_t * 100;
       
       //moze byc problem gdy odczyty zbyt czesto
       // }
        
        

    }

    memcpy(analyzer->prev_values, arr, analyzer->cpu_count*10 * sizeof(uint32_t));
    
    return analyzer->usage_array;
}
