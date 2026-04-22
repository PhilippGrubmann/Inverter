/*
 * encoder.c
 *
 *  Created on: 21.04.2026
 *      Author: grubm
 */

#include "encoder.h"
#include <math.h>
#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc3;

// adc3_buffer[0] = Sinus  (PF6, ADC3_IN4)
// adc3_buffer[1] = Cosinus (PF7, ADC3_IN5)
static uint16_t adc3_buffer[2];


