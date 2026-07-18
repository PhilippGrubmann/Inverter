/*
 * svwm.c
 *
 *  Created on: 15.07.2026
 *      Author: grubm
 */

#include "svpwm.h"

void svpwm_init(svpwm *data, uint16_t arr)
{
    data->u_a = 0.0f;
    data->u_b = 0.0f;
    data->u_c = 0.0f;
    data->d_u = 0.5f;   /* 50 % Duty = Nullspannung am Motor */
    data->d_v = 0.5f;
    data->d_w = 0.5f;
    data->arr = arr;
    data->ccr_u = (uint16_t)(0.5f * (float)arr + 0.5f);
    data->ccr_v = data->ccr_u;
    data->ccr_w = data->ccr_u;
}

void svpwm_update(svpwm *data, float u_alpha, float u_beta, float u_dc)
{
    /* --- Schutz: ungueltige Zwischenkreisspannung -> Nullspannung ---
     * Verhindert Division durch 0. 50 % auf allen Phasen = alle drei
     * Motorklemmen auf gleichem Potential = 0 V am Motor.            */
    if (u_dc < 1.0f) {
        data->d_u = 0.5f;
        data->d_v = 0.5f;
        data->d_w = 0.5f;
        data->ccr_u = (uint16_t)(0.5f * (float)data->arr + 0.5f);
        data->ccr_v = data->ccr_u;
        data->ccr_w = data->ccr_u;
        return;
    }

    /* --- Inverse Clarke: alpha/beta -> drei Phasenspannungen --- */
    data->u_a = u_alpha;
    data->u_b = -0.5f * u_alpha + 0.8660254f * u_beta;  /* sqrt(3)/2 */
    data->u_c = -0.5f * u_alpha - 0.8660254f * u_beta;  /* sqrt(3)/2 */

    /* --- Min-Max-Injektion (Nullsystem) ---
     * Verschiebt den Raumzeiger so, dass er symmetrisch im
     * verfuegbaren Spannungsband liegt. Ergibt exakt symmetrische
     * SVPWM und Aussteuerung bis U_DC/sqrt(3).                    */
    float u_max = data->u_a;
    if (data->u_b > u_max) { u_max = data->u_b; }
    if (data->u_c > u_max) { u_max = data->u_c; }

    float u_min = data->u_a;
    if (data->u_b < u_min) { u_min = data->u_b; }
    if (data->u_c < u_min) { u_min = data->u_c; }

    float u_offset = -0.5f * (u_max + u_min);

    /* --- Normierung auf Duty Cycle: 0.5 = Nullspannung --- */
    data->d_u = (data->u_a + u_offset) / u_dc + 0.5f;
    data->d_v = (data->u_b + u_offset) / u_dc + 0.5f;
    data->d_w = (data->u_c + u_offset) / u_dc + 0.5f;

    /* --- Clamping [0..1]: Schutz gegen Uebermodulation ---
     * Greift nur, wenn der Sollvektor > U_DC/sqrt(3) ist (sollte
     * durch das PI-Anti-Windup-Limit bereits verhindert sein --
     * doppelter Boden).                                          */
    if (data->d_u > 1.0f) { data->d_u = 1.0f; }
    if (data->d_u < 0.0f) { data->d_u = 0.0f; }
    if (data->d_v > 1.0f) { data->d_v = 1.0f; }
    if (data->d_v < 0.0f) { data->d_v = 0.0f; }
    if (data->d_w > 1.0f) { data->d_w = 1.0f; }
    if (data->d_w < 0.0f) { data->d_w = 0.0f; }

    /* --- Compare-Werte: Duty * ARR, +0.5f = kaufmaennisch runden --- */
    data->ccr_u = (uint16_t)(data->d_u * (float)data->arr + 0.5f);
    data->ccr_v = (uint16_t)(data->d_v * (float)data->arr + 0.5f);
    data->ccr_w = (uint16_t)(data->d_w * (float)data->arr + 0.5f);
}
