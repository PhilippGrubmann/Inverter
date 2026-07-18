/*
 * foc.c
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */


#include "foc.h"
#include <math.h>
#include "svpwm.h"
#include "inverter_config.h"


void init_clarke(clarke *data){
	data->A=0;
	data->B=0;
	data->C=0;
    data->alpha=0;
    data->beta=0;
}


void clarke_transform(clarke *data, float A, float B, float C){
	data->A=A;
	data->B=B;
	data->C=C;
	data->alpha = 0.666667f * data->A - 0.333333f * (data->B + data->C);
	data->beta  = 0.577350f * (data->B - data->C);
}


void init_park(park *data){
	data->alpha=0;
	data->beta=0;
	data->d=0;
	data->q=0;
	data->theta=0;
}

void park_transform(park *data, float alpha, float beta, float theta){
	data->alpha=alpha;
	data->beta=beta;
	data->theta=theta;
	float c = cosf(theta);
	float s = sinf(theta);
	data->d = data->alpha * c + data->beta * s;
	data->q = data->beta * c - data->alpha * s;
}

void init_inverse_park(inverse_park *data){
	data->alpha=0;
	data->beta=0;
	data->d=0;
	data->q=0;
	data->theta=0;
}

void inverse_park_transform(inverse_park *data, float d, float q, float theta){
	data->d=d;
	data->q=q;
	data->theta=theta;
	float c = cosf(theta);
	float s = sinf(theta);
	data->alpha = data->d * c - data->q * s;
	data->beta = data->q * c + data->d * s;
}

void foc_init(foc_state *state, uint16_t arr)
{
    init_clarke(&state->clarke_data);
    init_park(&state->park_data);
    init_pi(&state->regler_id, REGLER_KR, REGLER_T_S, REGLER_TN);
    init_pi(&state->regler_iq, REGLER_KR, REGLER_T_S, REGLER_TN);
    init_inverse_park(&state->inv_park_data);
    svpwm_init(&state->svpwm_data, arr);
}



void foc_run(foc_state *state,
             float I_U, float I_V, float I_W,
             float theta,
             float id_soll, float iq_soll,
             float limit)
{
    // Schritt 1 — Clarke: I_U, I_V, I_W  →  I_alpha, I_beta
	clarke_transform(&state->clarke_data, I_U, I_V, I_W);

    // Schritt 2 — Park: I_alpha, I_beta, theta  →  I_d_ist, I_q_ist
    park_transform(&state->park_data, state->clarke_data.alpha, state->clarke_data.beta, theta);

    // Schritt 3 — Fehler bilden (Soll - Ist)
    float ek_id = id_soll - state->park_data.d;
    float ek_iq = iq_soll - state->park_data.q;

    // Schritt 4 — PI-Regler für d und q
    pi_update(&state->regler_id, ek_id, limit);
    pi_update(&state->regler_iq, ek_iq, limit);

    // Schritt 5 — Inverse Park: u_d, u_q, theta  →  u_alpha, u_beta
    inverse_park_transform(&state->inv_park_data, state->regler_id.uk, state->regler_iq.uk, theta);

    /* Gen1: keine U_DC-Messung verfuegbar (kommt erst mit Gen2-Board).
     * Konservativer Fixwert, gemessen am Akku unter Volllast am DATUM.
     * MUSS ersetzt werden, sobald udc.c auf Gen2 fertig ist! */
    svpwm_update(&state->svpwm_data, state->inv_park_data.alpha, state->inv_park_data.beta, U_DC_GEN1_FIXED_V);

}





