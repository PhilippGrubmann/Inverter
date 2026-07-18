/*
 * safety.c
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */


#include "safety.h"
#include "inverter_config.h"
#include <math.h>



bool safety_check_current(float i_u, float i_v, float i_w)
{
    if (fabsf(i_u) > CURRENT_LIMIT_SW_A ||
        fabsf(i_v) > CURRENT_LIMIT_SW_A ||
        fabsf(i_w) > CURRENT_LIMIT_SW_A)
    {
        return false;   /* Überstrom */
    }
    return true;        /* alles ok */
}
