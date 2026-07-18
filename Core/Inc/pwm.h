/*
 * pwm.h
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "stm32f4xx_hal.h"
#include "svpwm.h"

/**
 * @brief  Schreibt die von SVPWM berechneten Compare-Werte in die
 *         TIM1-Register (CCR1/CCR2/CCR3 = Phase U/V/W).
 * @param  htim   Pointer auf den Timer-Handle (in main.c: &htim1)
 * @param  data   Pointer auf die svpwm-Struktur mit den fertigen ccr_u/v/w
 */
void pwm_update(TIM_HandleTypeDef *htim, const svpwm *data);

void pwm_enable(void);

void pwm_disable(void);



#endif /* INC_PWM_H_ */
