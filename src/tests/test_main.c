#include "test_reader.h"
#include "test_analyzer.h"
#include "test_printer.h"
#include "test_watchdog.h"
#include "test_logger.h"
#include "test_buffer.h"
int main(void){
    test_reader_run_all();
    test_analyzer_run_all();
    test_printer_run_all();
    test_watchdog_run_all();
    test_logger_run_all();
    test_buffer_run_all();
    printf("ALL TESTs PASSED\n");
    
    return 0;
}