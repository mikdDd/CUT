#include "reader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static size_t cpu_count(void);

enum{MAX_BUF_LEN = 10000};   
struct Reader
{   
    
    uint8_t cpu_count;
    char buf[MAX_BUF_LEN];
    uint32_t cpu_record_arr[];       //FAM
    
};

Reader* reader_new(void){
    size_t cpu_cnt = cpu_count();
   // printf("SDSDSD%zu\n",sizeof(Reader));
    Reader* reader = calloc(1,sizeof(Reader) + sizeof(uint32_t)*cpu_cnt*10);
   // printf("SDSDSD%zu\n",sizeof(*reader));
    reader->cpu_count = cpu_cnt;
    //printf("SDASDAD: %zu, %zu\n",sizeof(*reader),sizeof(Reader_cpu_record)*cpu_count());
    return reader;

}
void reader_delete(Reader* reader){
    free(reader);
}

static size_t cpu_count(void){
    char* p;
    char s[MAX_BUF_LEN]={0};
    int count = 0;
    size_t buflen = 0;
    FILE* fp = fopen("/proc/stat","rb");
    if(fp==NULL){return 0;}

    while(!feof(fp)){
        buflen = fread(s,1,MAX_BUF_LEN,fp);
    }
    fclose(fp);
    for (p = s; (p = strstr(p, "cpu")) != NULL; p++)
        count++;
    return count;
}

uint8_t reader_get_cpu_count(Reader* reader){
    return reader->cpu_count;
}

uint32_t* reader_read_data(Reader* reader){
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
    printf("SIZE : %zu\n",buflen);
    
    
  
   char * row = strtok(reader->buf," \n");

   size_t iterator = 0 ;

    size_t arr_iterator = 0;
   while(iterator < (reader->cpu_count ) * 11 ){
    //printf("{%s}\n",row);
    
    if(iterator%11 != 0){
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
   }
    //printf("SDSDSD%zu\n",sizeof(*reader));
    //uint32_t  (*ptr)[10] = reader->cpu_record_arr;
    return  reader->cpu_record_arr;
}
