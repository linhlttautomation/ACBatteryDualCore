/*
 * SinPWM_IFB_CLA.h
 *
 *  Created on: 6 thg 12, 2024
 *      Author: Admin
 */

#ifndef _SINPWM_IFB_CLA_H_
#define _SINPWM_IFB_CLA_H_
typedef struct  { float  Usa;               // Input: reference alpha-axis phase voltage
                  float  Usb;               // Input: reference beta-axis phase voltage
                  float  Usc;
                  float  Usd;               // Input: reference beta-axis phase voltage
                  float  Udc;               // Input: reference Udc voltage

                  float  Ta1;               // Output: reference phase-a switching function (0 , 1)
                  float  Ta2;               // Output: reference phase-a switching function (0 , 1)

                  float  Tb1;               // Output: reference phase-b switching function (0 , 1)
                  float  Tb2;               // Output: reference phase-b switching function (0 , 1)

                  float  T1;                // (-1 , 1)
                  float  T2;                // (-1 , 1)
                  float  T3;                // (-1 , 1)
                  float  Ta;
                  float  Tb;
                  float  Tc;
                  float  Td;
                  float  T1a;
                  float  T1b;
                  float  T1c;
                  float  T1d;
                } SPWM_CLA;
#define SPWM_CLA_INT(v)     \
        v.Usa = 0;          \
        v.Usb = 0;          \
        v.Usc = 0;          \
        v.Udc = 0;          \
        v.Ta = 0;           \
        v.Tb = 0;           \
        v.Tc = 0;           \
        v.T1 = 0;           \
        v.T2 = 0;           \
        v.T3 = 0;

#define SPWM_CLA_MARCO(v)               \
        v.T1 = v.Usa/(0.50*v.Udc);      \
        v.T2 = v.Usb/(0.50*v.Udc);      \
        v.T3 = v.Usc/(0.50*v.Udc);      \
                                        \
        v.Ta = v.T1/2.0 + 0.50;         \
        v.Tb = v.T2/2.0 + 0.50;         \
        v.Tc = v.T3/2.0 + 0.50;         \
         /*Limit*/                          \
        if (v.Ta<0.0) v.Ta = 0.0; else if(v.Ta>1.0) v.Ta = 1.0;     \
        if (v.Tb<0.0) v.Tb = 0.0; else if(v.Tb>1.0) v.Tb = 1.0;     \
        if (v.Tc<0.0) v.Tc = 0.0; else if(v.Tc>1.0) v.Tc = 1.0;

#define SPWM_1PHASE_CLA_MARCO(v)                                            \
        v.T1 = (1.0*Us_max/Udc_max)*v.Usa/v.Udc;                            \
        v.Ta1 = v.T1/2.0 + 0.50;                                            \
        v.Ta2 = -v.T1/2.0 + 0.50;                                           \
        if (v.Ta1 < 0.0) v.Ta1 = 0.0; else if (v.Ta1 > 1.0) v.Ta1 = 1.0;    \
        if (v.Ta2 < 0.0) v.Ta2 = 0.0; else if (v.Ta2 > 1.0) v.Ta2 = 1.0;    \

#define SPWM_1PHASE_BIPOLAR_CLA_MARCO(v)                                    \
        v.T1 = (1.0*Us_max/Udc_max)*v.Usa/v.Udc;                            \
        v.Ta1 = v.T1/2.0 + 0.50;                                            \
        v.Ta2 = v.T1/2.0 + 0.50;                                            \
        if (v.Ta1 < 0.0) v.Ta1 = 0.0; else if (v.Ta1 > 1.0) v.Ta1 = 1.0;    \
        if (v.Ta2 < 0.0) v.Ta2 = 0.0; else if (v.Ta2 > 1.0) v.Ta2 = 1.0;    \

#define SPWM_1PHASE_BIPOLAR_INTERLEAVED_CLA_MARCO(v)                        \
        v.T1 = (1.0*Us_max/Udc_max)*v.Usa/v.Udc;                            \
        v.T2 = (1.0*Us_max/Udc_max)*v.Usb/v.Udc;                            \
        v.Ta1 = v.T1/2.0 + 0.50;                                            \
        v.Ta2 = v.T1/2.0 + 0.50;                                            \
        EPwm6Regs.AQCTLA.all = AQCTLA_INIT_STATE_MODE2;                     \
        v.Tb1 = v.T2/2.0 + 0.50;                                            \
        v.Tb2 = v.T2/2.0 + 0.50;                                            \
        EPwm8Regs.AQCTLA.all = AQCTLA_INIT_STATE_MODE2;                     \
        if (v.Ta1 < 0.0) v.Ta1 = 0.0; else if (v.Ta1 > 1.0) v.Ta1 = 1.0;    \
        if (v.Ta2 < 0.0) v.Ta2 = 0.0; else if (v.Ta2 > 1.0) v.Ta2 = 1.0;    \
        if (v.Tb1 < 0.0) v.Tb1 = 0.0; else if (v.Tb1 > 1.0) v.Tb1 = 1.0;    \
        if (v.Tb2 < 0.0) v.Tb2 = 0.0; else if (v.Tb2 > 1.0) v.Tb2 = 1.0;    \

#define SPWM_1PHASE_BIPOLAR_INTERLEAVED_V2_CLA_MARCO(v)                     \
        v.T1 = (1.0*Us_max/Udc_max)*v.Usa/v.Udc;                            \
        v.T2 = (1.0*Us_max/Udc_max)*v.Usb/v.Udc;                            \
        v.Ta1 = v.T1/2.0 + 0.50;                                            \
        v.Ta2 = v.T1/2.0 + 0.50;                                            \
        EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                \
        EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;                                  \
        v.Tb1 = v.T2/2.0 + 0.50;                                            \
        v.Tb2 = v.T2/2.0 + 0.50;                                            \
        EPwm8Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                \
        EPwm8Regs.AQCTLA.bit.CAD = AQ_SET;                                  \
        if (v.Ta1 < 0.0) v.Ta1 = 0.0; else if (v.Ta1 > 1.0) v.Ta1 = 1.0;    \
        if (v.Ta2 < 0.0) v.Ta2 = 0.0; else if (v.Ta2 > 1.0) v.Ta2 = 1.0;    \
        if (v.Tb1 < 0.0) v.Tb1 = 0.0; else if (v.Tb1 > 1.0) v.Tb1 = 1.0;    \
        if (v.Tb2 < 0.0) v.Tb2 = 0.0; else if (v.Tb2 > 1.0) v.Tb2 = 1.0;    \

#define SPWM_1PHASE_BIPOLAR_INTERLEAVED_V3_CLA_MARCO(v)                     \
        v.Ta = (1.0*Us_max/Udc_max)*v.Usa/v.Udc;                            \
        v.Tb = (1.0*Us_max/Udc_max)*v.Usb/v.Udc;                            \
        v.T1a = v.Ta/2.0 + 0.50;                                            \
        v.T1b = v.Tb/2.0 + 0.50;                                            \
        EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                \
        EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;                                  \
        EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;                                  \
        EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                \
        v.Tc = (1.0*Us_max/Udc_max)*v.Usc/v.Udc;                            \
        v.Td = (1.0*Us_max/Udc_max)*v.Usd/v.Udc;                            \
        v.T1c = v.Tc/2.0 + 0.50;                                            \
        v.T1d = v.Td/2.0 + 0.50;                                            \
        EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                \
        EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;                                  \
        EPwm8Regs.AQCTLA.bit.CAU = AQ_SET;                                  \
        EPwm8Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                \
        if (v.T1a < 0.0) v.T1a = 0.0; else if (v.T1a > 1.0) v.T1a = 1.0;    \
        if (v.T1b < 0.0) v.T1b = 0.0; else if (v.T1b > 1.0) v.T1b = 1.0;    \
        if (v.T1c < 0.0) v.T1c = 0.0; else if (v.T1c > 1.0) v.T1c = 1.0;    \
        if (v.T1d < 0.0) v.T1d = 0.0; else if (v.T1d > 1.0) v.T1d = 1.0;    \





#endif /* LIBRARY_APP_LIB_PWM_INCLUDE_SINPWM_IFB_CLA_H_ */
