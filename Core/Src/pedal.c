/*
 * pedal.c
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */
#include "stm32f4xx_hal.h"
#include "pedal.h"

#define PEDAL_ADC_MAX  3959		/* wert runter damit 100% throttle schon früher */

static ADC_HandleTypeDef *pedal_adc;

void pedal_init(ADC_HandleTypeDef *hadc)
{
    pedal_adc = hadc;
}

float pedal_read(void)
{
    HAL_ADC_Start(pedal_adc);
    HAL_ADC_PollForConversion(pedal_adc, 2);
    uint32_t raw = HAL_ADC_GetValue(pedal_adc);
    HAL_ADC_Stop(pedal_adc);

    float normiert = (float)raw / (float)PEDAL_ADC_MAX;

    if (normiert > 1.0f) normiert = 1.0f;

    return normiert;
}
