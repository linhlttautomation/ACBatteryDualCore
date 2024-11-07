/*
 * PLL_3PHASE_CLA.h
 *
 *  Created on: 6 Mar 2017
 *      Author: dinhngock6
 */

#ifndef _PLL_3PHASE_CLA_H_
#define _PLL_3PHASE_CLA_H_

typedef struct {
	float Usd;
	float Usq;
	float w2_P;
	float w2_I;
	float w2;
	float w;
	float Kp_dsp;
	float Ki_dsp;
	float step;
	float Teta;
	float CheckPhase;
	float EnableCheckPhase;
}PLL_3PHASE_CLA;


#define PLL_3PHASE_CLA_MACRO(v)                                                          	\
		v.w2_P   = v.Kp_dsp*v.Usq;															\
		v.w2_I   = v.Ki_dsp*v.Usq + v.w2_I;                         						\
		v.w2     = v.w2_P + v.w2_I;                                                  		\
		v.w      = v.w2    + (100.0*PI/Us_max);                                         	\
		v.Teta   = v.Teta +  (v.step*v.w);                                          	    \
		if(v.Teta >= 1.0) v.Teta =   v.Teta - (1.0);                                  		\
		if(v.Teta <= 0.0) v.Teta = v.Teta + (1.0);

#define PLL_3PHASE_CLA_V2_MACRO(v)                                                          \
		v.w2_P   = v.Kp_dsp*v.Usq;															\
		v.w2_I   = v.Ki_dsp*v.Usq + v.w2_I;                         						\
		v.w2     = v.w2_P + v.w2_I;                                                  		\
		v.w      = v.w2    + (100.0*PI/Us_max);                                         	\
		v.Teta   = v.Teta +  (v.step*v.w);                                          	    \
		if(v.Teta >= 1.0) { v.Teta = v.Teta - (1.0); v.CheckPhase = 1.0;} /*TRUE*/          \
		if(v.Teta <= 0.0) { v.Teta = v.Teta + (1.0); v.CheckPhase = -1.0;}/*FALSE*/



#define PLL_3PHASE_CLA_INT(v)		\
v.Usd=0;							\
v.Usq=0;							\
v.w2_P=0;							\
v.w2_I=0;							\
v.w2=0;								\
v.w=0;								\
v.Kp_dsp=0;							\
v.Ki_dsp=0;							\
v.step=0;							\
v.Teta=0;							\
v.EnableCheckPhase = 0;				\
v.CheckPhase = 0;

#endif /* _PLL_3PHASE_CLA_H_ */
