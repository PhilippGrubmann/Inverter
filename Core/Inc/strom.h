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

/* ========== Umrechnungskonstante ========== */
/* LEM HAIS 150-P: 0,625 V / I_nominal (150 A) → 4,1667 mV/A am Sensor
 * Spannungsteiler 15k/22k auf MCU-Seite: Gain = 22k/(15k+22k) = 0,5946
 * ADC: 3,3 V / 4096 counts = 0,8057 mV/count
 *
 * Empfindlichkeit am ADC: 4,1667 mV/A * 0,5946 = 2,477 mV/A
 * → counts pro Ampere: 2,477 mV/A / 0,8057 mV/count = 3,074 counts/A
 * → Ampere pro count:  1 / 3,074 = 0,3253 A/count
 *
 * HINWEIS: Der Wert 0,0709 A/count aus deinem Memory basiert auf einer
 * anderen Rechnung — wir VERIFIZIEREN das mit dem Labornetzteil!
 * Vorläufig nehmen wir hier einen sauberen theoretischen Wert,
 * der dann beim Gegentest kalibriert wird.
 */
#define STROM_ADC_TO_AMPERE   0.3253f    /* A pro ADC-count — ZU VERIFIZIEREN */

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
