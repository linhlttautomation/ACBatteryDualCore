/*
 * PLL_1PHASE.h
 *
 *  Created on: 7 May 2017
 *      Author: dinhngock6
 */

#ifndef _PLL_1PHASE_CLA_H_
#define _PLL_1PHASE_CLA_H_

typedef struct {
float Usq;
float Theta;
float Kp_dsp;
float Ki_dsp;
float Kp;
float Ki;
float Ts;
float wref;
float w1_P;
float w1_I;
float w1;
float w;
float IN_MAX;
float OUT_MAX;

}PLL_1PHASE_CLA;


#define PLL_1PHASE_CLA_MACRO(v)											\
		v.Kp_dsp = v.Kp*v.IN_MAX/v.OUT_MAX;								\
		v.Ki_dsp = v.Ki*v.Ts*v.IN_MAX/v.OUT_MAX;						\
        v.w1_P   = (v.Kp_dsp*v.Usq);                                    \
		v.w1_I   = (v.Ki_dsp*v.Usq)  + v.w1_I;            				\
		v.w1     = v.w1_P + v.w1_I;                                     \
		v.w      = v.w1    + v.wref;                         			\
		v.Theta   = v.Theta + (v.OUT_MAX*v.Ts*v.w);           			\
		if(v.Theta > (1.0)) v.Theta = v.Theta - (1.0);                  \
		if(v.Theta < (0.0)) v.Theta = v.Theta + (1.0);



#define PLL_1PHASE_CLA_INT(v)			\
v.Usq = 0;								\
v.Theta = 0;							\
v.Kp_dsp = 0;							\
v.Ki_dsp = 0;							\
v.Kp = 0;								\
v.Ki = 0;								\
v.Ki = 0;								\
v.w1_P = 0;								\
v.w1_I = 0;								\
v.w1 = 0;								\
v.w = 0;								\
v.wref = 0;								\
v.IN_MAX = 0;							\
v.OUT_MAX = 0;

#endif /* 2_LIBRUARY_INCLUDE_TRANFORM_PLL_1PHASE_H_ */
