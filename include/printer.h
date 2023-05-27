#ifndef PRINTER_H
#define PRINTER_H
#include <inttypes.h>

typedef struct Printer Printer;
Printer* printer_new(const uint8_t cpu_count);
void printer_delete(Printer* const printer);
void printer_print_data(const Printer* const printer, const float usage_array[const]);


#endif
