/*
 * RSPWM_CLA.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Nghialq
 */

#ifndef RSPWM_CLA_H_
#define RSPWM_CLA_H_

typedef struct  { float  Ualpha;            // Input: reference alpha-axis phase voltage
                  float  Ubeta;             // Input: reference beta-axis phase voltage
                  float  Ugamma;            // Input: reference gamma-axis phase voltage
                  float  Udc;               // Input: reference Udc voltage
                  float  da;                // Output: reference phase-a switching function
                  float  db;                // Output: reference phase-b switching function
                  float  dc;                // Output: reference phase-c switching function
                  float  d1;
                  float  d2;
                  float  d3;
                  float  d0;
                  float  inver_Udc;
                  Uint16 Prism;
                } RSPWM_GEN_CLA;

#define RSPWM_GEN_CLA_MACRO(v)                                                                                              \
    /* Normalization */                                                                                                     \
    v.inver_Udc = (Us_max*1.0/Udc_max)*(1.0/v.Udc);                                                                         \
    /* Prism Identification */                                                                                              \
    if ((v.Ualpha*v.Ubeta >= 0) && (v.Ubeta >= 0) && (fabs(v.Ualpha) >= fabs(v.Ubeta)/1.732050807568877)) v.Prism = 1;      \
    if ((v.Ualpha*v.Ubeta >= 0) && (v.Ubeta >= 0) && (fabs(v.Ualpha) < fabs(v.Ubeta)/1.732050807568877)) v.Prism = 2;       \
    if ((v.Ualpha*v.Ubeta >= 0) && (v.Ubeta < 0) && (fabs(v.Ualpha) >= fabs(v.Ubeta)/1.732050807568877)) v.Prism = 4;       \
    if ((v.Ualpha*v.Ubeta >= 0) && (v.Ubeta < 0) && (fabs(v.Ualpha) < fabs(v.Ubeta)/1.732050807568877)) v.Prism = 5;        \
    if ((v.Ualpha*v.Ubeta < 0) && (v.Ubeta >= 0) && (fabs(v.Ualpha) >= fabs(v.Ubeta)/1.732050807568877)) v.Prism = 3;       \
    if ((v.Ualpha*v.Ubeta < 0) && (v.Ubeta >= 0) && (fabs(v.Ualpha) < fabs(v.Ubeta)/1.732050807568877)) v.Prism = 2;        \
    if ((v.Ualpha*v.Ubeta < 0) && (v.Ubeta < 0) && (fabs(v.Ualpha) >= fabs(v.Ubeta)/1.732050807568877)) v.Prism = 6;        \
    if ((v.Ualpha*v.Ubeta < 0) && (v.Ubeta < 0) && (fabs(v.Ualpha) < fabs(v.Ubeta)/1.732050807568877)) v.Prism = 5;         \
    /* Duty Cycle Calculation */                                                                                            \
    switch(v.Prism)                                                                                                         \
    {                                                                                                                       \
    case 0:                                                                                                                 \
        v.da = 0;                                                                                                           \
        v.db = 0;                                                                                                           \
        v.dc = 0;                                                                                                           \
        break;                                                                                                              \
    case 1:                                                                                                                 \
        /* Prism 1 */                                                                                                       \
        v.d1 = (-0.5*v.Ualpha - 0.8660254038*v.Ubeta + 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                    \
        v.d2 = (v.Ualpha - 0.5*v.Ugamma)*v.inver_Udc;                                                                       \
        v.d3 = (0.5*v.Ualpha + 0.8660254038*v.Ubeta + 0.5*v.Ugamma)*v.inver_Udc;                                            \
        v.d0 = (-v.Ualpha - 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                                               \
        v.da = 0.5*v.d1 + 0.5*v.d2 + 0.5*v.d3;                                                                              \
        v.db = 0.5*v.d1 + 0.5*v.d2;                                                                                         \
        v.dc = 0.5*v.d1;                                                                                                    \
        EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        break;                                                                                                              \
    case 2:                                                                                                                 \
        /* Prism 2 */                                                                                                       \
        v.d1 = (0.5*v.Ualpha - 0.8660254038*v.Ubeta - 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                     \
        v.d2 = (0.5*v.Ualpha + 0.8660254038*v.Ubeta + 0.5*v.Ugamma)*v.inver_Udc;                                            \
        v.d3 = (-0.5*v.Ualpha + 0.8660254038*v.Ubeta - 0.5*v.Ugamma)*v.inver_Udc;                                           \
        v.d0 = (-0.5*v.Ualpha - 0.8660254038*v.Ubeta + 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                    \
        v.da = 0.5*v.d1 + 0.5*v.d2;                                                                                         \
        v.db = 0.5*v.d1;                                                                                                    \
        v.dc = 0.5*v.d1 + 0.5*v.d2 + 0.5*v.d3;                                                                              \
        EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        break;                                                                                                              \
    case 3:                                                                                                                 \
        /* Prism 3 */                                                                                                       \
        v.d1 = (v.Ualpha + 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                                                \
        v.d2 = (-0.5*v.Ualpha + 0.8660254038*v.Ubeta - 0.5*v.Ugamma)*v.inver_Udc;                                           \
        v.d3 = (-v.Ualpha + 0.5*v.Ugamma)*v.inver_Udc;                                                                      \
        v.d0 = (0.5*v.Ualpha - 0.8660254038*v.Ubeta - 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                     \
        v.da = 0.5*v.d1;                                                                                                    \
        v.db = 0.5*v.d1 + 0.5*v.d2 + 0.5*v.d3;                                                                              \
        v.dc = 0.5*v.d1 + 0.5*v.d2;                                                                                         \
        EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        break;                                                                                                              \
    case 4:                                                                                                                 \
        /* Prism 4 */                                                                                                       \
        v.d1 = (0.5*v.Ualpha + 0.8660254038*v.Ubeta - 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                     \
        v.d2 = (-v.Ualpha + 0.5*v.Ugamma)*v.inver_Udc;                                                                      \
        v.d3 = (-0.5*v.Ualpha - 0.8660254038*v.Ubeta - 0.5*v.Ugamma)*v.inver_Udc;                                           \
        v.d0 = (v.Ualpha + 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                                                \
        v.da = 0.5*v.d1 + 0.5*v.d2 + 0.5*v.d3;                                                                              \
        v.db = 0.5*v.d1 + 0.5*v.d2;                                                                                         \
        v.dc = 0.5*v.d1;                                                                                                    \
        EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        break;                                                                                                              \
    case 5:                                                                                                                 \
        /* Prism 5 */                                                                                                       \
        v.d1 = (-0.5*v.Ualpha + 0.8660254038*v.Ubeta + 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                    \
        v.d2 = (-0.5*v.Ualpha - 0.8660254038*v.Ubeta - 0.5*v.Ugamma)*v.inver_Udc;                                           \
        v.d3 = (0.5*v.Ualpha - 0.8660254038*v.Ubeta + 0.5*v.Ugamma)*v.inver_Udc;                                            \
        v.d0 = (0.5*v.Ualpha + 0.8660254038*v.Ubeta - 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                     \
        v.da = 0.5*v.d1 + 0.5*v.d2;                                                                                         \
        v.db = 0.5*v.d1;                                                                                                    \
        v.dc = 0.5*v.d1 + 0.5*v.d2 + 0.5*v.d3;                                                                              \
        EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        break;                                                                                                              \
    case 6:                                                                                                                 \
        /* Prism 6 */                                                                                                       \
        v.d1 = (-v.Ualpha - 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                                               \
        v.d2 = (0.5*v.Ualpha - 0.8660254038*v.Ubeta + 0.5*v.Ugamma)*v.inver_Udc;                                            \
        v.d3 = (v.Ualpha - 0.5*v.Ugamma)*v.inver_Udc;                                                                       \
        v.d0 = (-0.5*v.Ualpha + 0.8660254038*v.Ubeta - 0.25*v.Ugamma)*v.inver_Udc + 0.5;                                    \
        v.da = 0.5*v.d1;                                                                                                    \
        v.db = 0.5*v.d1 + 0.5*v.d2 + 0.5*v.d3;                                                                              \
        v.dc = 0.5*v.d1 + 0.5*v.d2;                                                                                         \
        EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;                                                                                  \
        EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                                \
        EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;                                                                                \
        EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;                                                                                  \
        break;                                                                                                              \
    }                                                                                                                       \
    v.da = 2.0*v.da;                                                                                                        \
    v.db = 2.0*v.db;                                                                                                        \
    v.dc = 2.0*v.dc;                                                                                                        \
    /* Limit v.da,v.db,v.dc,v.dn 0:1 */                                                                                     \
    if (v.da < 0.0) v.da = 0.0; else if(v.da > 0.98) v.da = 0.98;                                                           \
    if (v.db < 0.0) v.db = 0.0; else if(v.db > 0.98) v.db = 0.98;                                                           \
    if (v.dc < 0.0) v.dc = 0.0; else if(v.dc > 0.98) v.dc = 0.98;                                                           \

#define RSPWM_GEN_CLA_INT(v)                                                                                                \
        v.Ualpha = 0;                                                                                                       \
        v.Ugamma = 0;                                                                                                       \
        v.Ubeta = 0;                                                                                                        \
        v.Udc = 0;                                                                                                          \
        v.da = 0;                                                                                                           \
        v.db = 0;                                                                                                           \
        v.dc = 0;                                                                                                           \
        v.d1 = 0;                                                                                                           \
        v.d2 = 0;                                                                                                           \
        v.d3 = 0;                                                                                                           \
        v.d0 = 0;                                                                                                           \
        v.inver_Udc = 0;                                                                                                    \
        v.Prism = 0;

#endif /* RSPWM_CLA_H_ */
