/*
 * pedal.h
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */

#ifndef INC_PEDAL_H_
#define INC_PEDAL_H_

#include "stm32f4xx_hal.h"

void  pedal_init(ADC_HandleTypeDef *hadc);
float pedal_read(void);


#endif /* INC_PEDAL_H_ */
