/**
 * @file pi_controller.c
 * @brief PI-Regler Implementierung in Geschwindigkeitsform
 *
 * KERNGLEICHUNG:
 *   uk = uk_1 + Kr * (ek - ek_1 + T/Tn * ek)
 *         │       │      │           │
 *         │       │      │           └─ I-Anteil: sammelt Fehler auf
 *         │       │      └───────────── P-Anteil: reagiert auf Änderung
 *         │       └──────────────────── Verstärkung
 *         └──────────────────────────── Aufaddieren auf letzten Wert
 *
 * ANTI-WINDUP:
 *   1. Clamping: uk wird auf ±limit begrenzt
 *   2. Reset: wenn enable=0, werden alle Speicherwerte genullt
 */

#include "pi_controller.h"



void init_pi(pi_controller *data, float Kr, float T, float Tn) {
    data->Kr = Kr;
    data->T  = T;
    data->Tn = Tn;

    data->ek   = 0.0f;
    data->ek_1 = 0.0f;
    data->uk   = 0.0f;
    data->uk_1 = 0.0f;
    data->limit =0.0f;
}


void pi_update(pi_controller *data, float ek, float limit) {
    data->ek = ek;
    // PI formel
    data->uk = data->uk_1 + data->Kr * (ek - data->ek_1) + data->Kr * (data->T/data->Tn)*ek;

    if (data->uk > limit) {
		data->uk = limit;
    } else if (data->uk < -limit) {
		data->uk = -limit;
	}



    data->uk_1 = data->uk;
    data->ek_1 = ek;
    data->limit = limit;
}

void pi_reset(pi_controller *data) {
    data->uk   = 0.0f;
    data->uk_1 = 0.0f;
    data->ek   = 0.0f;
    data->ek_1 = 0.0f;
}





