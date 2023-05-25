#ifndef PRINTER_H
#define PRINTER_H


typedef struct Printer Printer;
Printer* printer_new(uint8_t cpu_count);
void printer_delete(Printer* printer);
void printer_print_data(Printer* printer,float array[]);


#endif
