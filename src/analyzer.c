#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "analyzer.h"
/**
 * @brief Analyzer struct containing cpu count, previous cpu records array
 * and array with calculated usage  
 * 
 */
struct Analyzer
{                   
    uint8_t cpu_count;    
    Data* prev_values;     
    float usage_array[];
};
/**
 * @brief Function that creates a new Analyzer
 * 
 * @param cpu_count count of CPUs
 * @return Analyzer* pointer to new Analyzer
 */
Analyzer* analyzer_new(const uint8_t cpu_count){
    if(cpu_count == 0)return NULL;
    Analyzer* a = calloc(1,sizeof(Analyzer) + sizeof(float)*cpu_count);
    if(a == NULL)return NULL;
    a->cpu_count = cpu_count;
    a->prev_values = calloc(cpu_count, sizeof(Data));
    return a;
}
/**
 * @brief Function that deletes given Analyzer
 * 
 * @param analyzer to deletion
 */
void analyzer_delete(Analyzer* const analyzer){
    if(analyzer == NULL)return;
    free(analyzer->prev_values);
    free(analyzer);
}
/**
 * @brief function that calculates usage of every CPU core
 * 
 * @param analyzer 
 * @param arr array of cpu records from Reader
 * @return float* array of usage for every CPU core
 */
float* analyzer_analyze_data(Analyzer* const analyzer, const Data arr[const]){
    if(analyzer == NULL)return NULL;
    if(arr == NULL)return NULL;

    float ret;

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
