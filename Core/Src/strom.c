/*
 * strom.c
 *
 *  Created on: 20.04.2026
 *      Author: Philipp
 */


/*
 *  Phasenstrommessung über 3x LEM HAIS 150-P
 */

#include "strom.h"

/* ========== Strom_Messen ==========
 * Liest eine komplette Sequenz (3 Kanäle) aus dem ADC.
 * Ablauf: Start → Warten bis Kanal U fertig → lesen → Kanal V → lesen → Kanal W → lesen → Stop
 */
void Strom_Messen(ADC_HandleTypeDef *hadc, Strom_t *s)
{
    /* Sequenz starten */
    HAL_ADC_Start(hadc);

    /* Rank 1: I_U */
    HAL_ADC_PollForConversion(hadc, 10);
    s->raw_U = HAL_ADC_GetValue(hadc);

    /* Rank 2: I_V */
    HAL_ADC_PollForConversion(hadc, 10);
    s->raw_V = HAL_ADC_GetValue(hadc);

    /* Rank 3: I_W */
    HAL_ADC_PollForConversion(hadc, 10);
    s->raw_W = HAL_ADC_GetValue(hadc);

    /* ADC stoppen — sonst startet er beim nächsten Aufruf nicht sauber neu */
    HAL_ADC_Stop(hadc);

    /* Umrechnung in Ampere (offset-kompensiert) */
    s->i_U = ((float)s->raw_U - (float)s->offset_U) * STROM_ADC_TO_AMPERE;
    s->i_V = ((float)s->raw_V - (float)s->offset_V) * STROM_ADC_TO_AMPERE;
    s->i_W = ((float)s->raw_W - (float)s->offset_W) * STROM_ADC_TO_AMPERE;
}

/* ========== Strom_Kalibrieren ==========
 * Mittelt 1000 Samples pro Kanal → Offset bei 0 A.
 * WICHTIG: Motor/Inverter muss stromlos sein!
 * Dauert ca. 1000 * 4 µs = 4 ms.
 */
void Strom_Kalibrieren(ADC_HandleTypeDef *hadc, Strom_t *s)
{
    uint32_t sum_U = 0;
    uint32_t sum_V = 0;
    uint32_t sum_W = 0;
    const uint16_t N = 1000;

    for (uint16_t i = 0; i < N; i++)
    {
        HAL_ADC_Start(hadc);

        HAL_ADC_PollForConversion(hadc, 10);
        sum_U += HAL_ADC_GetValue(hadc);

        HAL_ADC_PollForConversion(hadc, 10);
        sum_V += HAL_ADC_GetValue(hadc);

        HAL_ADC_PollForConversion(hadc, 10);
        sum_W += HAL_ADC_GetValue(hadc);

        HAL_ADC_Stop(hadc);
    }

    s->offset_U = (uint16_t)(sum_U / N);
    s->offset_V = (uint16_t)(sum_V / N);
    s->offset_W = (uint16_t)(sum_W / N);
}
