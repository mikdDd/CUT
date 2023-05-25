#ifndef ANALYZER_H
#define ANALYZER_H

#include <inttypes.h>
#include "data.h"

typedef struct Analyzer Analyzer;
Analyzer* analyzer_new(const uint8_t cpu_count);
void analyzer_delete(Analyzer* const analyzer);
float* analyzer_analyze_data(Analyzer* const analyzer, const Data arr[const]);

#endif
