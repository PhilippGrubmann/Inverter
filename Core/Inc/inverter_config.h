/*
 * inverter_config.h
 *
 *  Created on: 01.07.2026
 *      Author: grubm
 */

#ifndef INC_INVERTER_CONFIG_H_
#define INC_INVERTER_CONFIG_H_

/* ==================== max Strom ==================== */
#define CURRENT_LIMIT_SW_A 	50.0f



/* ==================== PWM / Timer ==================== */

#define PWM_FREQUENZ_HZ     20000u    /* Ziel-Schaltfrequenz [Hz]              */
#define TIM1_ARR             4499u    /* Timer-Periode (Center-Aligned)         */
#define TIM1_DEADTIME          205u   /* Deadtime-Register, ~2us                */
#define TIM1_REP_COUNTER         1u   /* MUSS 1 sein! 0 verdoppelt ISR auf 40kHz */


/* ==================== Regler (Betragsoptimum) ====================*/


#define REGLER_T_S      (1.0f / PWM_FREQUENZ_HZ)   /* Abtastzeit = 1/f_PWM in s */
#define REGLER_KR        0.22f   /* Verstaerkung Kr = L/(2*T_Sigma)     */
#define REGLER_TN       0.0105f  /* Nachstellzeit Tn = L/R [s]          */




/* TODO: entfernen sobald udc.c auf Gen2 fertig ist!                 */
#define U_DC_GEN1_FIXED_V   45.0f

#endif /* INC_INVERTER_CONFIG_H_ */
