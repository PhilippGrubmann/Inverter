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

void PI_Init(PI_Controller *pi, float Kr, float Tn, float T, float limit)
{
    /* Parameter setzen */
    pi->Kr    = Kr;
    pi->Tn    = Tn;
    pi->T     = T;
    pi->limit = limit;

    /* Zustand auf Null — sauberer Start */
    pi->uk    = 0.0f;
    pi->uk_1  = 0.0f;
    pi->ek    = 0.0f;
    pi->ek_1  = 0.0f;
}

float PI_Update(PI_Controller *pi, float error, uint8_t enable)
{
    if (!enable)
    {
        /*
         * Regler ist aus → kompletter Reset
         * Warum: Wenn der Inverter abgeschaltet ist, darf
         * kein alter Zustand im Speicher bleiben.
         * Sonst gibt's beim Wiedereinschalten einen Stromstoß.
         */
        PI_Reset(pi);
        return 0.0f;
    }

    /* Fehler speichern */
    pi->ek = error;

    /*
     * Geschwindigkeitsform berechnen:
     *   uk = uk_1 + Kr * (ek - ek_1 + T/Tn * ek)
     *
     * Aufgedröselt:
     *   delta_p = ek - ek_1        → wie schnell ändert sich der Fehler
     *   delta_i = T/Tn * ek        → aktueller Fehler, skaliert
     *   delta_u = Kr * (delta_p + delta_i)
     *   uk = uk_1 + delta_u        → neuer Wert = alter Wert + Änderung
     */
    float delta_u = pi->Kr * ((pi->ek - pi->ek_1) + (pi->T / pi->Tn) * pi->ek);
    pi->uk = pi->uk_1 + delta_u;

    /*
     * Anti-Windup: Clamping auf ±limit
     * Die Diplomarbeit hat nur die positive Seite begrenzt — das ist ein Bug!
     * Negative Begrenzung ist genauso wichtig (z.B. beim Abbremsen).
     */
    if (pi->uk > pi->limit)
    {
        pi->uk = pi->limit;
    }
    if (pi->uk < -(pi->limit))
    {
        pi->uk = -(pi->limit);
    }

    /* Werte für den nächsten Zyklus speichern */
    pi->uk_1 = pi->uk;
    pi->ek_1 = pi->ek;

    return pi->uk;
}

void PI_Reset(PI_Controller *pi)
{
    pi->uk    = 0.0f;
    pi->uk_1  = 0.0f;
    pi->ek    = 0.0f;
    pi->ek_1  = 0.0f;
}
