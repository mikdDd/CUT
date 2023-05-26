#include "test_reader.h"
#include "reader.h"
#include <assert.h>
#include <stdlib.h>

void test_reader_run_all(void){
    test_reader_new();
    test_reader_delete();
    test_reader_get_cpu_count();
    test_reader_read_data();
}
void test_reader_new(void){

    Reader* reader = reader_new();
    assert(reader != NULL);
}

void test_reader_delete(void){
    //CHECK IF NOT CRASHING
    {
    Reader* reader = NULL;
    reader_delete(reader);
    }

    //USUAL BEHAVIOR
    {
    Reader* reader = reader_new();
    reader_delete(reader);
    }
}

void test_reader_get_cpu_count(void){
    
    //USUAL BEHAVIOR
    {
    Reader* reader = reader_new();
    assert(reader_get_cpu_count(reader)!=0);
    }

    {
    Reader* reader = NULL;
    assert(reader_get_cpu_count(reader)==0);
    }
}
void test_reader_read_data(void){
    
    //USUAL BEHAVIOR
    {
    Reader* reader = reader_new();
    assert(reader_read_data(reader)!=NULL);
    }

    //READ DATA WITH NULL READER
    {
    Reader* reader = NULL;
    assert(reader_read_data(reader)==NULL);
    }

    //1000x READ
    {
    Reader* reader = reader_new();
    for(size_t i = 0; i < 1000; i++){
        assert(reader_read_data(reader)!=NULL);
    }
    
    }
}


