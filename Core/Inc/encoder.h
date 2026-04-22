/*
 * encoder.h
 *
 *  Created on: 21.04.2026
 *      Author: grubm
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#define ENCODER_POLE_PAIRS    4
/* 3/5 Vdd = 3V * 15/22 = 2,045V
 * ADC_OFFSET = 2,045 * 4095 / 3,3 = 2537
 */
#define ENCODER_ADC_OFFSET       2537
#define ENCODER_ADC_AMPLITUDE    931

void  encoder_init(void);
void  encoder_update(void);
float encoder_get_angle_mech(void);
float encoder_get_angle_elec(void);


#endif /* INC_ENCODER_H_ */
