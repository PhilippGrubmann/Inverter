/*
 * foc.h
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */

#include "pi_controller.h"
#include "svpwm.h"

#ifndef INC_FOC_H_
#define INC_FOC_H_



typedef struct clarke{
	float A;
	float B;
	float C;
	float alpha;
	float beta;
}clarke;

typedef struct park{
	float alpha;
	float beta;
	float d;
	float q;
	float theta;
}park;

typedef struct inverse_park {
	float alpha;
	float beta;
	float d;
	float q;
	float theta;
}inverse_park;

typedef struct {
    clarke clarke_data;
    park park_data;
    pi_controller regler_id;
    pi_controller regler_iq;
    inverse_park inv_park_data;
    svpwm       svpwm_data;   /* NEU */

} foc_state;


void init_clarke(clarke *data);
void clarke_transform(clarke *data, float A, float B, float C);
void init_park(park *data);
void park_transform(park *data, float alpha, float beta, float theta);
void init_inverse_park(inverse_park *data);
void inverse_park_transform(inverse_park *data, float d, float q, float theta);

void foc_init(foc_state *state, uint16_t arr);
void foc_run(foc_state *state,
             float I_U, float I_V, float I_W,
             float theta,
             float id_soll, float iq_soll,
             float limit);





#endif /* INC_FOC_H_ */





