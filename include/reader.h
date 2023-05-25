#ifndef READER_H
#define READER_H
#include <inttypes.h>
#include "data.h"
typedef struct Reader Reader;
Reader* reader_new(void);
void reader_delete(Reader* const reader);

Data* reader_read_data(Reader* const reader);
uint8_t reader_get_cpu_count(const Reader* const reader);
#endif
