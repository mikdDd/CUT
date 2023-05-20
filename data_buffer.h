#ifndef BUFFER_H
#define BUFFER_H
#include "data.h"
#include <stdbool.h>
#include <stddef.h>
typedef struct Data_buffer Data_buffer;
Data_buffer* data_buffer_new(size_t elem_size);
void data_buffer_delete(Data_buffer* db);
bool buffer_is_full(const Data_buffer* db);
bool buffer_is_empty(const Data_buffer* db);
void buffer_put(Data_buffer* db, const void* data);
void buffer_get(Data_buffer* db, void* ptr);
void buffer_lock(Data_buffer* db);
void buffer_unlock(Data_buffer* db);
void buffer_call_producer(Data_buffer* db);
void buffer_call_consumer(Data_buffer* db);
void buffer_wait_for_producer(Data_buffer* db);
void buffer_wait_for_consumer(Data_buffer* db);



#endif