#ifndef PRINTER_H
#define PRINTER_H


typedef struct Printer Printer;
Printer* printer_new(void);
void printer_delete(Printer* printer);


#endif