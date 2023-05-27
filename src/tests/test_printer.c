#include "test_printer.h"
#include "printer.h"
#include <assert.h>
#include <stdlib.h>

void test_printer_run_all(void){
   test_printer_new();
   test_printer_delete();
   test_printer_print_data();
}
void test_printer_new(void){
    //USUAL BEHAVIOR
    {
    Printer* printer = printer_new(8);
    assert(printer != NULL);
    }

    
    {
    Printer* printer = printer_new(0);
    assert(printer == NULL);
    }
}

void test_printer_delete(void){
    //CHECK IF NOT CRASHING
    {
    Printer* printer = NULL;
    printer_delete(printer);
    }

    //USUAL BEHAVIOR
    {
    Printer* printer = printer_new(8);
    printer_delete(printer);
    }
}

void test_printer_print_data(void){
    
    //USUAL BEHAVIOR
    {
    Printer* printer = printer_new(3);
    float arr[3] = {0};
    printer_print_data(printer,arr);
    }

    {
    Printer* printer = NULL;
    float arr[3] = {0};
    printer_print_data(printer,arr);
    }

    {
    Printer* printer = NULL;
    float* arr = NULL;
    printer_print_data(printer,arr);
    }
    {
    Printer* printer = printer_new(3);
    float* arr = NULL;
    printer_print_data(printer,arr);
    }
}

