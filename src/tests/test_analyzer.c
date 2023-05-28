#include "test_analyzer.h"
#include "analyzer.h"
#include <assert.h>
#include <stdlib.h>
#include "data.h"
void test_analyzer_run_all(void){
    test_analyzer_new();
    test_analyzer_delete();
    test_analyzer_analyze_data();
}
void test_analyzer_new(void){
    //USUAL BEHAVIOR
    {
    Analyzer* analyzer = analyzer_new(8);
    assert(analyzer != NULL);
    }
    //0 CPU
    {
    Analyzer* analyzer = analyzer_new(0);
    assert(analyzer == NULL);
    }
}

void test_analyzer_delete(void){
    //CHECK IF NOT CRASHING
    {
    Analyzer* analyzer = NULL;
    analyzer_delete(analyzer);
    }

    //USUAL BEHAVIOR
    {
    Analyzer* analyzer = analyzer_new(8);
    analyzer_delete(analyzer);
    }
}

void test_analyzer_analyze_data(void){
    
    //USUAL BEHAVIOR
    {
    Analyzer* analyzer = analyzer_new(3);
    Data arr[3] = {0};
    assert(analyzer_analyze_data(analyzer,arr)!=NULL);
    }
    //NULL ANALYZER
    {
    Analyzer* analyzer = NULL;
    Data arr[3] = {0};
    assert(analyzer_analyze_data(analyzer,arr)==NULL);
    }
    //NULL ARRAY
    {
    Analyzer* analyzer = analyzer_new(3);
    Data* arr = NULL;
    assert(analyzer_analyze_data(analyzer,arr)==NULL);
    }
}

