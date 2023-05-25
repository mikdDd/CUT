#ifndef BUFFER_H
#define BUFFER_H
#include "data.h"
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
typedef struct Data_buffer Data_buffer;
Data_buffer* data_buffer_new(size_t elem_size, size_t capacity);
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
int buffer_wait_for_producer_timedwait(Data_buffer* db, struct timespec* ts);
void thread__producer_cleanup(void* arg);
void thread__consumer_cleanup(void* arg);
void thread__log_producer_put_to_buffer(Data_buffer* db, char* data);
bool buffer_is_to_deletion(const Data_buffer* const db);
void buffer_thread_producer(Data_buffer* db, void* data);
void buffer_thread_consumer(Data_buffer* db, void** data);
void buffer_watchdog_thread_consumer(Data_buffer* db, char (*error_string)[], char* error_message, bool* cancel_signal, struct timespec* ts);

#endif
