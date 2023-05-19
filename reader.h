#ifndef READER_H
#define READER_H
#include <inttypes.h>

typedef struct Reader Reader;
Reader* reader_new(void);
void reader_delete(Reader* reader);

uint32_t* reader_read_data(Reader* reader);
uint8_t reader_get_cpu_count(Reader* reader);
#endif
