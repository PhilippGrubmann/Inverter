/*
 * safety.h
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */

#ifndef INC_SAFETY_H_
#define INC_SAFETY_H_

#include <stdbool.h>

bool safety_check_current(float i_u, float i_v, float i_w);

#endif /* INC_SAFETY_H_ */
