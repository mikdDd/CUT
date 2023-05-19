#ifndef BUFFER_H
#define BUFFER_H
#include "data.h"
#include <stdbool.h>

typedef struct Data_buffer Data_buffer;
Data_buffer* datar_buffer_new(void);
void data_buffer_delete(Data_buffer* printer);
bool buffer_is_full(Data_buffer* db);
bool buffer_is_empty(Data_buffer* db);
void buffer_put(Data_buffer* db, Data* data);
Data* buffer_get(Data_buffer* db);
void buffer_lock(Data_buffer* db);
void buffer_unlock(Data_buffer* db);
void buffer_call_producer(Data_buffer* db);
void buffer_call_consumer(Data_buffer* db);
void buffer_wait_for_producer(Data_buffer* db);
void buffer_wait_for_consumer(Data_buffer* db);



#endif