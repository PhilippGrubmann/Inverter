/*
 * svpwm.h
 *
 *  Created on: 15.07.2026
 *      Author: grubm
 */

#ifndef INC_SVPWM_H_
#define INC_SVPWM_H_

#include <stdint.h>

/* SVPWM per Min-Max-Injektion (mathematisch identisch zu symmetrischer
 * Raumzeigermodulation, Aussteuerung bis U_DC/sqrt(3)).
 * Hardware-frei: rechnet nur u_alpha/u_beta -> Timer-Compare-Werte. */

typedef struct {
    float u_a;          /* Phasenspannung U nach inverser Clarke [V]     */
    float u_b;          /* Phasenspannung V nach inverser Clarke [V]     */
    float u_c;          /* Phasenspannung W nach inverser Clarke [V]     */
    float d_u;          /* Duty Cycle Phase U, 0.0 ... 1.0               */
    float d_v;          /* Duty Cycle Phase V, 0.0 ... 1.0               */
    float d_w;          /* Duty Cycle Phase W, 0.0 ... 1.0               */
    uint16_t ccr_u;     /* Compare-Wert fuer TIM1->CCR1                  */
    uint16_t ccr_v;     /* Compare-Wert fuer TIM1->CCR2                  */
    uint16_t ccr_w;     /* Compare-Wert fuer TIM1->CCR3                  */
    uint16_t arr;       /* Timer-Periode (ARR), bei uns 4499             */
} svpwm;

void svpwm_init(svpwm *data, uint16_t arr);
void svpwm_update(svpwm *data, float u_alpha, float u_beta, float u_dc);



#endif /* INC_SVPWM_H_ */
