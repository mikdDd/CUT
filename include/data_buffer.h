#ifndef BUFFER_H
#define BUFFER_H
#include "data.h"
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
typedef struct Data_buffer Data_buffer;
Data_buffer* data_buffer_new(size_t elem_size, size_t capacity);
void data_buffer_delete(Data_buffer* const db);
bool buffer_is_full(const Data_buffer* const db);
bool buffer_is_empty(const Data_buffer* db);
void buffer_put(Data_buffer* const restrict db, const void *const restrict data);
void buffer_get(Data_buffer* const restrict db, void *const restrict ptr);
void buffer_lock(Data_buffer* const db);
void buffer_unlock(Data_buffer* const db);
void buffer_call_producer(Data_buffer* const db);
void buffer_call_consumer(Data_buffer* const db);
void buffer_wait_for_producer(Data_buffer* const db);
void buffer_wait_for_consumer(Data_buffer* const db);
int buffer_wait_for_producer_timedwait(Data_buffer* const db, const struct timespec* const ts);
void buffer_thread_producer_cleanup(void* const arg);
void buffer_thread_consumer_cleanup(void* const arg);
void thread__log_producer_put_to_buffer(Data_buffer* db, char* data);
bool buffer_is_to_deletion(const Data_buffer* const db);
void buffer_thread_producer(Data_buffer* const restrict db, const void* const restrict data);
void buffer_thread_consumer(Data_buffer* const restrict db, void** const restrict data);
void buffer_watchdog_thread_consumer(Data_buffer* const db, const char (*error_string)[], const char* const error_message, bool* const cancel_signal, const struct timespec* const ts);

#endif
