/*
 * PWM_3PHASE_CLA.h
 *
 *  Created on: 19 Feb 2017
 *      Author: dinhngock6
 */

#ifndef _PWM_3PHASE_CLA_H_
#define _PWM_3PHASE_CLA_H_

typedef struct {
        Uint16 PeriodMax;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        float MfuncC1;        // Input: EPWM1 A&B Duty cycle ratio (Pu)
        float MfuncC2;        // Input: EPWM2 A&B Duty cycle ratio (Pu)
        float MfuncC3;        // Input: EPWM3 A&B Duty cycle ratio (Pu)
        } PWMGEN_3PHASE_CLA ;


#define PWM_3PHASE_MACRO(v)								     \
 EPwm1Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncC1*v.PeriodMax) ;  \
 EPwm2Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncC2*v.PeriodMax) ;  \
 EPwm3Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncC3*v.PeriodMax);


#endif /* 2_LIBRUARY_APP_LIB_PWM_INCLUDE_PWM_3PHASE_CLA_H_ */
