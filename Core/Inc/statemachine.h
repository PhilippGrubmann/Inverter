/*
 * statemachine.h
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_RUN,
    STATE_FAULT
} state_t;

typedef enum {
    FAULT_NONE = 0,
    FAULT_OVERCURRENT,
    FAULT_OVERVOLTAGE,
    FAULT_UNDERVOLTAGE,
    FAULT_OVERTEMP,
    FAULT_ENCODER,
    FAULT_SELFTEST,
    FAULT_CAN_TIMEOUT
} fault_code_t;

state_t       statemachine_get_state(void);
fault_code_t  statemachine_get_fault_code(void);
void          statemachine_set_fault(fault_code_t code);

// zum testen
void statemachine_set_run(void);

#endif
