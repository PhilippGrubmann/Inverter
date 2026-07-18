/*
 * pwm.c
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */


#include "pwm.h"
#include "main.h"
extern TIM_HandleTypeDef htim1;

void pwm_update(TIM_HandleTypeDef *htim, const svpwm *data)
{
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, data->ccr_u);  /* Phase U */
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, data->ccr_v);  /* Phase V */
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, data->ccr_w);  /* Phase W */
}


void pwm_enable(void)
{
    /* Gate-Treiber freigeben (active-low → LOW = an) */
    HAL_GPIO_WritePin(DSP_GDR_EN_GPIO_Port, DSP_GDR_EN_Pin, GPIO_PIN_RESET);
}


void pwm_disable(void)
{
    /* Gate-Treiber sperren (active-low → HIGH = aus) */
    HAL_GPIO_WritePin(DSP_GDR_EN_GPIO_Port, DSP_GDR_EN_Pin, GPIO_PIN_SET);

    /* CCRs auf 50% (Mittelwert = 0V am Motor) */
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 2250);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 2250);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 2250);
}
