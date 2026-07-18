/**
 * @file pi_controller.h
 * @brief PI-Regler in Geschwindigkeitsform
 *
 */

#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

#include <stdint.h>

typedef struct {
    float uk;
    float uk_1;
    float ek;
    float ek_1;
    float Kr;
    float Tn;
    float T;
    float limit;
} pi_controller;


void init_pi(pi_controller *data, float Kr, float T, float Tn);

void pi_update(pi_controller *data, float ek, float limit);

void pi_reset(pi_controller *data);


#endif /* PI_CONTROLLER_H */
