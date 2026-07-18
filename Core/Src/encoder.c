/*
 * encoder.c
 *
 *  Created on: 21.04.2026
 *      Author: grubm
 */

#include "encoder.h"
#include <math.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define PI_F  3.14159265f

extern ADC_HandleTypeDef hadc3;

/* adc3_buffer[0] = Sinus  (PF6, ADC3_IN4)
 * adc3_buffer[1] = Cosinus (PF7, ADC3_IN5)
 * Wird per DMA im Hintergrund fortlaufend befuellt (ADC3 DMAContinuousRequests
 * ist in CubeMX bereits aktiviert) -- encoder_update() liest hier nur noch,
 * startet aber selbst keine neue Wandlung an. */
static volatile uint16_t adc3_buffer[2];

static float angle_mech_rad = 0.0f;   /* Mechanischer Winkel [0, 2*PI)  */
static float angle_elec_rad = 0.0f;   /* Elektrischer Winkel [0, 2*PI)  */

void encoder_init(void)
{
    angle_mech_rad = 0.0f;
    angle_elec_rad = 0.0f;

    /* Startet ADC3 im DMA-Modus, adc3_buffer wird ab jetzt fortlaufend
     * im Hintergrund aktualisiert (kein weiterer Software-Trigger noetig). */
    HAL_ADC_Start_DMA(&hadc3, (uint32_t*)(uint16_t*)adc3_buffer, 2);
}

/* --------------------------------------------------------------------
 * Hardwarefreier Rechenkern -- offline verifiziert (7 Tests gruen,
 * siehe encoder_test.c): alle 4 Quadranten, Amplituden-Unabhaengigkeit,
 * Wrap-Around mech->elek.
 * -------------------------------------------------------------------- */

float encoder_calc_angle_mech(uint16_t sin_raw, uint16_t cos_raw)
{
    /* Offset abziehen -- Amplitude wird NICHT gebraucht: eine gleichmaessige
     * Skalierung beider Achsen aendert den atan2f-Winkel nicht
     * (atan2(k*sin, k*cos) = atan2(sin, cos) fuer k>0).                */
    float fsin = (float)sin_raw - (float)ENCODER_ADC_OFFSET;
    float fcos = (float)cos_raw - (float)ENCODER_ADC_OFFSET;

    float theta = atan2f(fsin, fcos);   /* Ergebnis zunaechst in (-PI, +PI] */

    /* Normieren auf [0, 2*PI) */
    if (theta < 0.0f) {
        theta += 2.0f * PI_F;
    }
    return theta;
}

float encoder_mech_to_elec(float angle_mech, uint8_t pole_pairs)
{
    float theta_elec = angle_mech * (float)pole_pairs;

    theta_elec = fmodf(theta_elec, 2.0f * PI_F);
    if (theta_elec < 0.0f) {
        theta_elec += 2.0f * PI_F;
    }
    return theta_elec;
}

/* -------------------------------------------------------------------- */

void encoder_update(void){
    uint16_t sin_raw = adc3_buffer[0];
    uint16_t cos_raw = adc3_buffer[1];

    angle_mech_rad = encoder_calc_angle_mech(sin_raw, cos_raw);
    angle_elec_rad = encoder_mech_to_elec(angle_mech_rad, ENCODER_POLE_PAIRS);
}

float encoder_get_angle_mech(void) { return angle_mech_rad; }
float encoder_get_angle_elec(void) { return angle_elec_rad; }
