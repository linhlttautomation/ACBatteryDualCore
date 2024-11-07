/*
 * PLL.h
 *
 *  Created on: Sep 16, 2015
 *      Author: Ngoc
 */

#ifndef _PLL_2LINE_PE_H_
#define _PLL_2LINE_PE_H_
typedef struct {
	float Uab;
	float Ubc;
	float Ualpha;
	float Ubeta;
	float Usd;
	float Usq;
	float w2_P;
	float w2_I;
	float w2;
	float w;
	float kp;
	float ki;
	float step;
	float Teta;
}PLL_2LINE_CLA;



#define PLL_2LINE_CLA_MACRO(v)                                                          	\
		v.Ualpha = 0.66667*v.Uab + 0.33333*v.Ubc;                						\
		v.Ubeta  = 0.57735*v.Ubc;                                             			\
		v.Usd    = v.Ualpha*CLAcosPU(v.Teta)) + v.Ubeta*CLAsinPU(v.Teta)) ; 			\
		v.Usq    = -(v.Ualpha*CLAsinPU(v.Teta)) + (v.Ubeta,CLAcosPU(v.Teta)) ; 			\
		v.w2_P   = (v.kp*v.Usq);                                                   		\
		v.w2_I   = (v.ki*T*v.Usq) + v.w2_I;                                          		\
		v.w2     = v.w2_P + v.w2_I;                                                  	\
		v.w      = v.w2    + (100.0*PI/Usmax);                                          \
		v.Teta   = v.Teta + (v.step*v.w);                                          		\
		if(v.Teta > _IQ(1.0)) v.Teta = v.Teta - (1.0);                                  \
		if(v.Teta < _IQ(0.0)) v.Teta = v.Teta + (1.0);                                  \

#define PLL_2LINE_CLA_INT(v)		\
v.Uab=0;							\
v.Ubc=0;							\
v.Ualpha=0;							\
v.Ubeta=0;							\
v.Usd=0;							\
v.Usq=0;							\
v.w2_P=0;							\
v.w2_I=0;							\
v.w2=0;								\
v.w=0;								\
v.kp=0;								\
v.ki=0;								\
v.step=0;							\
v.Teta=0;


#endif /* PLL_H_ */
