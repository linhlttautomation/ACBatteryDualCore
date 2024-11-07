/*
 * PWM_3HB_CLA.h
 *
 *  Created on: 20 Feb 2017
 *      Author: dinhngock6
 */

#ifndef _PWM_3T_CLA_H_
#define _PWM_3T_CLA_H_

typedef struct {
        Uint16 PeriodMax;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        float MfuncA1;        // Input: EPWM8 A&B Duty cycle ratio (Pu)
        float MfuncA2;        // Input: EPWM7 A&B Duty cycle ratio (Pu)
        float MfuncB1;        // Input: EPWM11 A&B Duty cycle ratio (Pu)
        float MfuncB2;        // Input: EPWM9 A&B Duty cycle ratio (Pu)
        float MfuncC1;        // Input: EPWM10 A&B Duty cycle ratio (Pu)
        float MfuncC2;        // Input: EPWM6 A&B Duty cycle ratio (Pu)
        } PWMGEN_3T_CLA ;


#define PWM_3T_MACRO(v)                                                     \
 EPwm1Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA1*v.PeriodMax);                  \
 EPwm2Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA2*v.PeriodMax);                  \
                                                                            \
 EPwm3Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncB1*v.PeriodMax);                  \
 EPwm4Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncB2*v.PeriodMax);                  \
                                                                            \
 EPwm5Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncC1*v.PeriodMax) ;                 \
 EPwm6Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncC2*v.PeriodMax);


#endif /* _PWM_3HB_CLA_H_ */
