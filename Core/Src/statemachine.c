/*
 * statemachine.c
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */


#include "statemachine.h"
#include "stm32f4xx_hal.h"


static state_t      current_state = STATE_IDLE;
static fault_code_t current_fault = FAULT_NONE;

state_t statemachine_get_state(void){
    return current_state;
}

fault_code_t statemachine_get_fault_code(void){
    return current_fault;
}

void statemachine_set_fault(fault_code_t code){
    current_fault = code;
    current_state = STATE_FAULT;
}

void statemachine_set_run(void){
    if (current_state != STATE_FAULT){
        current_state = STATE_RUN;
    }
}

void statemachine_set_idle(void){
    current_fault = FAULT_NONE;
    current_state = STATE_IDLE;
}
