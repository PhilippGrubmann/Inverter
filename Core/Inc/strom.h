/*
 * strom.h
 *
 *  Phasenstrommessung über 3x LEM HAIS 150-P
 *  I_U = ADC1_IN1  (PA1)
 *  I_V = ADC1_IN2  (PA2)
 *  I_W = ADC1_IN12 (PC2)
 */

#ifndef SRC_STROM_H_
#define SRC_STROM_H_

#include "stm32f4xx_hal.h"



//STROM_ADC_TO_AMPERE = 3.3 / 4096 / (0.625/150 × 15/22)
#define STROM_ADC_TO_AMPERE   0.2836f       /* A pro ADC-count — ZU VERIFIZIEREN */

/* Struktur für alle 3 Phasen */
typedef struct {
    uint16_t raw_U;      /* Rohwert ADC Phase U */
    uint16_t raw_V;      /* Rohwert ADC Phase V */
    uint16_t raw_W;      /* Rohwert ADC Phase W */

    uint16_t offset_U;   /* Nullpunkt bei 0 A (Kalibrierung) */
    uint16_t offset_V;
    uint16_t offset_W;

    float i_U;           /* Strom in Ampere */
    float i_V;
    float i_W;
} Strom_t;

/* ========== Funktionen ========== */

/* Einmaliges Einlesen aller 3 Phasen (blockierend, ~4 µs) */
void Strom_Messen(ADC_HandleTypeDef *hadc, Strom_t *s);

/* Offset-Kalibrierung: 1000 Samples mitteln, stromlos!
 * Muss aufgerufen werden BEVOR der erste Strom fließt. */
void Strom_Kalibrieren(ADC_HandleTypeDef *hadc, Strom_t *s);

#endif /* SRC_STROM_H_ */
