/**
 * @file pi_controller.h
 * @brief PI-Regler in Geschwindigkeitsform
 *
 *
 * Verwendung:
 *   PI_Controller regler_alpha;
 *   PI_Init(&regler_alpha, Kr, Tn, T, limit);
 *   ...
 *   float u = PI_Update(&regler_alpha, fehler, ein);
 */

#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

#include <stdint.h>

typedef struct {
    /* Zustandsgrößen (werden vom Regler intern verwaltet) */
    float uk;       /* Stellgröße aktuell (Ausgang)                    */
    float uk_1;     /* Stellgröße vom letzten Zyklus                   */
    float ek;       /* Fehler aktuell                                  */
    float ek_1;     /* Fehler vom letzten Zyklus                       */

    /* Parameter (werden bei Init gesetzt) */
    float Kr;       /* Reglerverstärkung                               */
    float Tn;       /* Nachstellzeit [s]                               */
    float T;        /* Abtastzeit [s] (z.B. 0.00005 für 20 kHz)       */
    float limit;    /* Anti-Windup: max. Betrag der Stellgröße         */
} PI_Controller;

/**
 * @brief  Regler initialisieren — Parameter setzen, Zustand auf Null
 *
 * @param  pi     Pointer auf den Regler
 * @param  Kr     Reglerverstärkung
 * @param  Tn     Nachstellzeit in Sekunden
 * @param  T      Abtastzeit in Sekunden
 * @param  limit  Anti-Windup Begrenzung (positiver Wert)
 */
void PI_Init(PI_Controller *pi, float Kr, float Tn, float T, float limit);

/**
 * @brief  Einen Reglerschritt berechnen — in der ISR aufrufen!
 *
 * @param  pi     Pointer auf den Regler
 * @param  error  Aktueller Fehler (Soll - Ist)
 * @param  enable 1 = Regler aktiv, 0 = Regler aus (Reset)
 * @return float  Stellgröße (begrenzt auf ±limit)
 */
float PI_Update(PI_Controller *pi, float error, uint8_t enable);

/**
 * @brief  Regler zurücksetzen — alle Speicherwerte auf Null
 *         Aufrufen wenn Inverter abgeschaltet wird!
 *
 * @param  pi     Pointer auf den Regler
 */
void PI_Reset(PI_Controller *pi);

#endif /* PI_CONTROLLER_H */
