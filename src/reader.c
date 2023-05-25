#include "reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static uint8_t cpu_count(void);

enum{MAX_BUF_LEN = 10000};   
struct Reader
{   
    
    uint8_t cpu_count;
    char buf[MAX_BUF_LEN];
    Data cpu_record_arr[];
    //uint32_t cpu_record_arr[];       //FAM
    
};

Reader* reader_new(void){
    uint8_t cpu_cnt = cpu_count();
   // printf("SDSDSD%zu\n",sizeof(Reader));
    Reader* reader = calloc(1,sizeof(Reader) + (sizeof(Data)*cpu_cnt));
    if(reader == NULL)return NULL;
   // printf("SDSDSD%zu\n",sizeof(*reader));
    reader->cpu_count = cpu_cnt;
    //printf("SDASDAD: %zu, %zu\n",sizeof(*reader),sizeof(Reader_cpu_record)*cpu_count());
    return reader;

}
void reader_delete(Reader* const reader){
    if(reader == NULL)return;
    free(reader);
}


static uint8_t cpu_count(void){
    char* p;
    char s[MAX_BUF_LEN]={0};
    uint8_t count = 0;
    //size_t buflen = 0;
    FILE* fp = fopen("/proc/stat","rb");
    if(fp==NULL){return 0;}

    while(!feof(fp)){
        fread(s,1,MAX_BUF_LEN,fp);
    }
    fclose(fp);
    for (p = s; (p = strstr(p, "cpu")) != NULL; p++)
        count++;
    return count;
}

uint8_t reader_get_cpu_count(const Reader* const reader){
    if(reader == NULL)return 0;
    return reader->cpu_count;
}

Data* reader_read_data(Reader* const reader){

    if(reader == NULL)return NULL;
    size_t buflen = 0;
    FILE* fp = fopen("/proc/stat","rb");
    
    if(fp==NULL){return NULL;}

    while(!feof(fp)){
        buflen = fread(reader->buf,1,MAX_BUF_LEN,fp);
    }
    fclose(fp);

    for(size_t i = 0; i < buflen; i++){
       // printf("%c",reader->buf[i]);
    }
   // printf("SIZE : %zu\n",buflen);
    
    
  
   char * row = strtok(reader->buf," \n");

   size_t iterator = 0 ;

    size_t arr_iterator = 0;
   while(iterator < (reader->cpu_count ) * 11 ){
       // size_t debug1 = arr_iterator;
        //uint32_t debug2;
        switch (iterator%11){
            case 0:
                if(iterator!=0){arr_iterator++;}
            break;

            case 1:
                reader->cpu_record_arr[arr_iterator].user = (uint32_t)atoi(row);
                //debug2 = reader->cpu_record_arr[arr_iterator].user;
            break;

            case 2:
                reader->cpu_record_arr[arr_iterator].nice = (uint32_t)atoi(row);
                //debug2 = reader->cpu_record_arr[arr_iterator].nice;
            break;

            case 3:
                reader->cpu_record_arr[arr_iterator].system = (uint32_t)atoi(row);
               // debug2 = reader->cpu_record_arr[arr_iterator].system;
            break;

            case 4:
                reader->cpu_record_arr[arr_iterator].idle = (uint32_t)atoi(row);
               // debug2 = reader->cpu_record_arr[arr_iterator].idle;
            break;

            case 5:
                reader->cpu_record_arr[arr_iterator].iowait = (uint32_t)atoi(row);
                //debug2 = reader->cpu_record_arr[arr_iterator].iowait;
            break;

            case 6:
                reader->cpu_record_arr[arr_iterator].irq = (uint32_t)atoi(row);
                //debug2 = reader->cpu_record_arr[arr_iterator].irq;
            break;

            case 7:
                reader->cpu_record_arr[arr_iterator].softirq = (uint32_t)atoi(row);
                //debug2 = reader->cpu_record_arr[arr_iterator].softirq;
            break;

            case 8:
                reader->cpu_record_arr[arr_iterator].steal = (uint32_t)atoi(row);
                //debug2 = reader->cpu_record_arr[arr_iterator].steal;
            break;

            case 9:
                reader->cpu_record_arr[arr_iterator].guest = (uint32_t)atoi(row);
                //debug2 = reader->cpu_record_arr[arr_iterator].guest;
            break;

            case 10:
                reader->cpu_record_arr[arr_iterator].guest_nice = (uint32_t)atoi(row);
               // debug2 = reader->cpu_record_arr[arr_iterator].guest_nice;
            break;
        
            default:
            break;
        }
        
        row = strtok(NULL, " \n");
        iterator++;
    //printf("{%s}\n",row);
    /*
    if(iterator%11 != 0){                                                                   //pomijamy wyraz "cpu"
        reader->cpu_record_arr[arr_iterator] =  atoi(row);
     //   printf("<%d>",reader->cpu_record_arr[arr_iterator]);
        arr_iterator++;
    } else{
      //  printf("\n");
    }
    row = strtok(NULL, " \n");
    //char * data = row;
     //row = strtok(NULL," ");
   // for(size_t i = 0; i < 11; i++){
       // printf("DATA:<%s>\n",row);
      //row = strtok(NULL," ");
       
   // }
    
    iterator++;
    */
   }
    //printf("SDSDSD%zu\n",sizeof(*reader));
    //uint32_t  (*ptr)[10] = reader->cpu_record_arr;
    return  reader->cpu_record_arr;
}
