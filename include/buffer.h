#ifndef BUFFER_H
#define BUFFER_H
#include "data.h"
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
typedef struct Buffer Buffer;
Buffer* buffer_new(size_t elem_size, size_t capacity);
void buffer_delete(Buffer* const db);
bool buffer_is_full(const Buffer* const db);
bool buffer_is_empty(const Buffer* db);
void buffer_put(Buffer* const restrict db, const void *const restrict data);
void buffer_get(Buffer* const restrict db, void *const restrict ptr);
void buffer_lock(Buffer* const db);
void buffer_unlock(Buffer* const db);
void buffer_call_producer(Buffer* const db);
void buffer_call_consumer(Buffer* const db);
void buffer_wait_for_producer(Buffer* const db);
void buffer_wait_for_consumer(Buffer* const db);
int buffer_wait_for_producer_timedwait(Buffer* const db, const struct timespec* const ts);
void buffer_thread_producer_cleanup(void* const arg);
void buffer_thread_consumer_cleanup(void* const arg);
void thread__log_producer_put_to_buffer(Buffer* db, char* data);
bool buffer_is_to_deletion(const Buffer* const db);
void buffer_thread_producer(Buffer* const restrict db, const void** const restrict data);
void buffer_thread_consumer(Buffer* const restrict db, void** const restrict data);
void buffer_watchdog_thread_consumer(Buffer* const db, char (*error_string)[], const char* const error_message, bool* const cancel_signal, const struct timespec* const ts);

#endif
