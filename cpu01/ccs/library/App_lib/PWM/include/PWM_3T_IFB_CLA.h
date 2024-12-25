/*
 * PWM_3T_IFB_CLA.h
 *
 *  Created on: 6 thg 12, 2024
 *      Author: Admin
 */

#ifndef _PWM_3T_IFB_CLA_H_
#define _PWM_3T_IFB_CLA_H_
typedef struct {
        Uint16 PeriodMax;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        Uint16 DutyA1;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        Uint16 DutyA2;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        Uint16 DutyB1;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        Uint16 DutyB2;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        Uint16 DutyC1;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        Uint16 DutyC2;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        float MfuncA1;        // Input: EPWM8 A&B Duty cycle ratio (Pu)
        float MfuncA2;        // Input: EPWM7 A&B Duty cycle ratio (Pu)
        float MfuncB1;        // Input: EPWM11 A&B Duty cycle ratio (Pu)
        float MfuncB2;        // Input: EPWM9 A&B Duty cycle ratio (Pu)
        float MfuncC1;        // Input: EPWM10 A&B Duty cycle ratio (Pu)
        float MfuncC2;        // Input: EPWM6 A&B Duty cycle ratio (Pu)
        } PWMGEN_3T_IFB_CLA ;

#define PWM_3T_MACRO(v)                                                     \
    EPwm1Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA1*v.PeriodMax);               \
    EPwm2Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA2*v.PeriodMax);               \
    EPwm4Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncB1*v.PeriodMax);               \
    EPwm5Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncB2*v.PeriodMax);

#define PWM_1P_MACRO(v)                                                     \
    EPwm4Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA1*v.PeriodMax);               \
    EPwm6Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA2*v.PeriodMax);               \
    EPwm5Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncB1*v.PeriodMax);               \
    EPwm8Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncB2*v.PeriodMax);               \

#define PWM_3T_V2_MACRO(v)                                                  \
 if(v.MfuncA1 < 0.00020)                                                    \
 {                                                                          \
    EPwm1Regs.CMPA.bit.CMPA = 1;                                            \
 }                                                                          \
 else                                                                       \
 {                                                                          \
    EPwm1Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA1*v.PeriodMax);               \
 }                                                                          \
if(v.MfuncA2 < 0.00020)                                                     \
{                                                                           \
   EPwm2Regs.CMPA.bit.CMPA = 1;                                             \
}                                                                           \
else                                                                        \
{                                                                           \
    EPwm2Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncA2*v.PeriodMax);               \
}                                                                           \
 if(v.MfuncB1 < 0.00020)                                                    \
 {                                                                          \
    EPwm4Regs.CMPA.bit.CMPA = 1;                                            \
 }                                                                          \
 else                                                                       \
 {                                                                          \
     EPwm4Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncB1*v.PeriodMax);              \
 }                                                                          \
 if(v.MfuncB2 < 0.00020)                                                    \
 {                                                                          \
    EPwm5Regs.CMPA.bit.CMPA = 1;                                            \
 }                                                                          \
 else                                                                       \
 {                                                                          \
     EPwm5Regs.CMPA.bit.CMPA =(Uint16)(v.MfuncB2*v.PeriodMax);              \
 }

#define PWM_3T_V3_MACRO(v)                              \
    v.DutyA1 =(Uint16)(v.MfuncA1*v.PeriodMax);          \
    v.DutyA2 =(Uint16)(v.MfuncA2*v.PeriodMax);          \
    v.DutyB1 =(Uint16)(v.MfuncB1*v.PeriodMax);          \
    v.DutyB2 =(Uint16)(v.MfuncB2*v.PeriodMax);          \
                                                        \
    if(v.DutyA1 <= 1)v.DutyA1 = 1;                     \
    if(v.DutyA2 <= 1)v.DutyA2 = 1;                     \
    if(v.DutyB1 <= 1)v.DutyB1 = 1;                     \
    if(v.DutyB2 <= 1)v.DutyB2 = 1;                     \
                                                       \
    EPwm1Regs.CMPA.bit.CMPA = v.DutyA1;                \
    EPwm2Regs.CMPA.bit.CMPA = v.DutyA2;                \
    EPwm4Regs.CMPA.bit.CMPA = v.DutyB1;                \
    EPwm5Regs.CMPA.bit.CMPA = v.DutyB2;


#endif /* _PWM_3HB_CLA_H_ */






