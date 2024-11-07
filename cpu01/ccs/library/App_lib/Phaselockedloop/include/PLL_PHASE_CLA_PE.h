/*
 * PLL_1PHASE.h
 *
 *  Created on: 7 May 2017
 *      Author: dinhngock6
 */

#ifndef _PLL_PHASE_CLA_H_
#define _PLL_PHASE_CLA_H_

typedef struct {
float Usq;
float Theta_har_1st;
float Theta_har_3rd;
float Kp_dsp;
float Ki_dsp;
float Kp;
float Ki;
float Ts;
float wref1;
float wref3;
float w_P;
float w_I;
float w;
float w1;
float w3;
short CheckPhase;
float IN_MAX;
float OUT_MAX;

}PLL_PHASE_CLA;


#define PLL_PHASE_CLA_MACRO(v)											\
		v.Kp_dsp = v.Kp*v.IN_MAX/v.OUT_MAX;								\
		v.Ki_dsp = v.Ki*v.Ts*v.IN_MAX/v.OUT_MAX;						\
        v.w_P   = (v.Kp_dsp*v.Usq);                                     \
		v.w_I   = (v.Ki_dsp*v.Usq)  + v.w_I;            				\
		v.w     = v.w_P + v.w_I;                                        \
		v.w1      = v.w    + v.wref1;                         			\
        v.w3      = 3.0*v.w    + v.wref3;                                    \
		v.Theta_har_1st   = v.Theta_har_1st + (v.OUT_MAX*v.Ts*v.w1);           			\
        v.Theta_har_3rd   = v.Theta_har_3rd + (v.OUT_MAX*v.Ts*v.w3);                    \
		if(v.Theta_har_1st > (1.0)) v.Theta_har_1st = v.Theta_har_1st - (1.0);          \
		if(v.Theta_har_1st < (0.0)) v.Theta_har_1st = v.Theta_har_1st + (1.0);          \
        if(v.Theta_har_3rd > (1.0)) v.Theta_har_3rd = v.Theta_har_3rd - (1.0);          \
        if(v.Theta_har_3rd < (0.0)) v.Theta_har_3rd = v.Theta_har_3rd + (1.0);

#define PLL_PHASE_CLA_V2_MACRO(v)                                          \
        v.Kp_dsp = v.Kp*v.IN_MAX/v.OUT_MAX;                             \
        v.Ki_dsp = v.Ki*v.Ts*v.IN_MAX/v.OUT_MAX;                        \
        v.w_P   = (v.Kp_dsp*v.Usq);                                     \
        v.w_I   = (v.Ki_dsp*v.Usq)  + v.w_I;                            \
        v.w     = v.w_P + v.w_I;                                        \
        v.w1      = v.w    + v.wref1;                                   \
        v.w3      = 3.0*v.w    + v.wref3;                                               \
        v.Theta_har_1st   = v.Theta_har_1st + (v.OUT_MAX*v.Ts*v.w1);                    \
        v.Theta_har_3rd   = v.Theta_har_3rd + (v.OUT_MAX*v.Ts*v.w3);                    \
        if(v.Theta_har_1st > (1.0)) {v.Theta_har_1st = v.Theta_har_1st -(1.0);v.CheckPhase = 1;} /*TRUE*/           \
        if(v.Theta_har_1st < (0.0)) {v.Theta_har_1st = v.Theta_har_1st +(1.0);v.CheckPhase = 0;}  /*FALSE*/         \
        if(v.Theta_har_3rd > (1.0)) v.Theta_har_3rd = v.Theta_har_3rd - (1.0);          \
        if(v.Theta_har_3rd < (0.0)) v.Theta_har_3rd = v.Theta_har_3rd + (1.0);

#define PLL_PHASE_CLA_V3_MACRO(v)                                       \
        v.Kp_dsp = v.Kp*v.IN_MAX/v.OUT_MAX;                             \
        v.Ki_dsp = v.Ki*v.Ts*v.IN_MAX/v.OUT_MAX;                        \
        v.w_P   = (v.Kp_dsp*v.Usq);                                     \
        v.w_I   = (v.Ki_dsp*v.Usq)  + v.w_I;                            \
        v.w     = v.w_P + v.w_I;                                        \
        v.w1      = v.w    + v.wref1;                                   \
        v.Theta_har_1st   = v.Theta_har_1st + (v.OUT_MAX*v.Ts*v.w1);    \
        if(v.Theta_har_1st > (1.0000)) {v.Theta_har_1st = v.Theta_har_1st -(1.000);v.CheckPhase = 1;}  \
        if(v.Theta_har_1st < (0.0000)) {v.Theta_har_1st = v.Theta_har_1st +(1.000);v.CheckPhase = 0;}  \
        v.Theta_har_3rd   = 3.000*v.Theta_har_1st;                                                     \
        if(v.Theta_har_3rd > 2.0000)   v.Theta_har_3rd = v.Theta_har_3rd - (2.0000);                   \
        else if (v.Theta_har_3rd > 1.0000)v.Theta_har_3rd = v.Theta_har_3rd - (1.0000);


#define PLL_PHASE_CLA_INT(v)			\
v.Usq = 0;								\
v.Theta_har_1st = 0;					\
v.Theta_har_3rd = 0;                    \
v.Kp_dsp = 0;							\
v.Ki_dsp = 0;							\
v.Kp = 0;								\
v.Ki = 0;								\
v.w_P = 0;								\
v.w_I = 0;								\
v.w = 0;								\
v.w1 = 0;								\
v.w3 = 0;                               \
v.wref1 = 0;							\
v.wref3 = 0;                            \
v.CheckPhase = 0;                       \
v.IN_MAX = 0;							\
v.OUT_MAX = 0;

#endif /* 2_LIBRUARY_INCLUDE_TRANFORM_PLL_1PHASE_H_ */
