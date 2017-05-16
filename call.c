//
// Created by songx on 2017/5/16.
//

#include "type.h"
#include "struct.h"
#include "call.h"
#include "bottom.h"


void init(){
    
}

int read(data_unit *data, v_address address, m_pid_t pid){
    return 0;
}

int write(data_unit data, v_address address, m_pid_t pid){
    return 0;   
}

int allocate(v_address *address, m_size_t size, m_pid_t pid){
    return 0;
}

int free(v_address address, m_pid_t pid){
    return 0;
}