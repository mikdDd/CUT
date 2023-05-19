#include "printer.h"
struct Printer{
    
};
Printer* printer_new(void){
    Printer* p = calloc(1,sizeof(Printer));
    return p;
}

void printer_delete(Printer* printer){
    free(printer);
}