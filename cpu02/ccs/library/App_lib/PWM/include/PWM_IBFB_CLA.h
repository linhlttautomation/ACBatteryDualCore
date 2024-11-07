/*
 * PWM_3HB_CLA.h
 *
 *  Created on: 20 Feb 2017
 *      Author: dinhngock6
 */

#ifndef _PWM_IBFB_MACRO_H_
#define _PWM_IBFB_MACRO_H_

typedef struct {
        Uint16 PeriodMax;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        float MfuncA1_PV1;        // Input: EPWM8 A&B Duty cycle ratio (Pu)
        float MfuncA2_PV1;        // Input: EPWM7 A&B Duty cycle ratio (Pu)
        float MfuncA1_PV2;        // Input: EPWM11 A&B Duty cycle ratio (Pu)
        float MfuncA2_PV2;        // Input: EPWM9 A&B Duty cycle ratio (Pu)
        float MfuncA1_PV3;        // Input: EPWM10 A&B Duty cycle ratio (Pu)
        float MfuncA2_PV3;        // Input: EPWM6 A&B Duty cycle ratio (Pu)
        } PWMGEN_IBFB_CLA ;


#define PWM_IBFB_MACRO(v)                                                       \
 EPwm1Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA1_PV1*v.PeriodMax);                  \
 EPwm2Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA2_PV1*v.PeriodMax);                  \
                                                                                \
 EPwm3Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA1_PV2*v.PeriodMax);                  \
 EPwm4Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA2_PV2*v.PeriodMax);                  \
                                                                                \
 EPwm5Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA1_PV3*v.PeriodMax) ;                 \
 EPwm6Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA2_PV3*v.PeriodMax);


#endif /* _PWM_3HB_CLA_H_ */
