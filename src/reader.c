#include "reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static uint8_t cpu_count(void);

enum{MAX_BUF_LEN = 4000};   
/**
 * @brief Reader struct containing cpu count and cpu record array
 * 
 */
struct Reader
{   
    uint8_t cpu_count;
    Data cpu_record_arr[];    
};

/**
 * @brief Function that creates a new Reader
 * 
 * @return Reader* pointer to new Reader
 */
Reader* reader_new(void){
    uint8_t cpu_cnt = cpu_count();
    Reader* reader = calloc(1,sizeof(Reader) + (sizeof(Data)*cpu_cnt));
    if(reader == NULL)return NULL;

    reader->cpu_count = cpu_cnt;
  
    return reader;

}
/**
 * @brief Function that deletes given Reader
 * 
 * @param reader to deletion
 */
void reader_delete(Reader* const reader){
    if(reader == NULL)return;
    free(reader);
}

/**
 * @brief static function counting "cpu" occurs in proc/stat file
 * called in reader_new function
 * 
 * @return uint8_t count of CPUs
 */
static uint8_t cpu_count(void){
    char* p;
    size_t buf_size = MAX_BUF_LEN;
    char* tmp_buf = calloc(buf_size,sizeof(char));
    uint8_t count = 0;

    bool was_error = true;

    while(was_error){
        FILE* fp = fopen("/proc/stat","rb");
        fread(tmp_buf,1,buf_size,fp);

        if(ferror(fp) == 0 && feof(fp)!=0){
            was_error = false;
        }
        if(was_error){
            buf_size*=2;
            tmp_buf = realloc(tmp_buf,sizeof(char) * buf_size);
        }
        fclose(fp);
    }
 
    for (p = tmp_buf; (p = strstr(p, "cpu")) != NULL; p++)
    {count++;}
        free(tmp_buf);
    return count;
}
/**
 * @brief cpu count getter function
 * 
 * @param reader whose cpu count we want to get
 * @return uint8_t cpu count of reader
 */
uint8_t reader_get_cpu_count(const Reader* const reader){
    if(reader == NULL)return 0;
    return reader->cpu_count;
}

/**
 * @brief function that reads content of proc/stat file
 * 
 * @param reader whose file will be readen
 * @return Data* array of cpu records
 */
Data* reader_read_data(Reader* const reader){

    if(reader == NULL)return NULL;
    size_t buf_size = MAX_BUF_LEN;
    char* tmp_buf = calloc(buf_size,sizeof(char));
    

     bool was_error = true;

    while(was_error){
        FILE* fp = fopen("/proc/stat","rb");
        fread(tmp_buf,1,buf_size,fp);

        if(ferror(fp) == 0 && feof(fp)!=0){
            was_error = false;
        }
        if(was_error){
            buf_size*=2;
            tmp_buf = realloc(tmp_buf,sizeof(char) * buf_size);
        }
        fclose(fp);
    }

    char * row = strtok(tmp_buf," \n");

    size_t iterator = 0 ;

    size_t arr_iterator = 0;
    while(iterator < (reader->cpu_count ) * 11 ){
        switch (iterator%11){
            case 0:
                if(iterator!=0){arr_iterator++;}
            break;

            case 1:
                reader->cpu_record_arr[arr_iterator].user = (uint32_t)atoi(row);
            break;

            case 2:
                reader->cpu_record_arr[arr_iterator].nice = (uint32_t)atoi(row);
            break;

            case 3:
                reader->cpu_record_arr[arr_iterator].system = (uint32_t)atoi(row);
            break;

            case 4:
                reader->cpu_record_arr[arr_iterator].idle = (uint32_t)atoi(row);
            break;

            case 5:
                reader->cpu_record_arr[arr_iterator].iowait = (uint32_t)atoi(row);
            break;

            case 6:
                reader->cpu_record_arr[arr_iterator].irq = (uint32_t)atoi(row);
            break;

            case 7:
                reader->cpu_record_arr[arr_iterator].softirq = (uint32_t)atoi(row);
            break;

            case 8:
                reader->cpu_record_arr[arr_iterator].steal = (uint32_t)atoi(row);
            break;

            case 9:
                reader->cpu_record_arr[arr_iterator].guest = (uint32_t)atoi(row);
            break;

            case 10:
                reader->cpu_record_arr[arr_iterator].guest_nice = (uint32_t)atoi(row);
            break;
        
            default:
            break;
        }
        
        row = strtok(NULL, " \n");
        iterator++;
    }
    free(tmp_buf);
return  reader->cpu_record_arr;
}
