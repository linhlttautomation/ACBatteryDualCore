/*
 * PLL_1PHASE.h
 *
 *  Created on: 7 May 2017
 *      Author: dinhngock6
 */

#ifndef _PLL_1PHASE2_CLA_H_
#define _PLL_1PHASE2_CLA_H_

typedef struct {
float k;
float Vs_k;
float Vs_k_1;
float Vs_k_2;
float Ualpha_k;
float Ualpha_k_1;
float Ualpha_k_2;
float Ubeta_k;
float Ubeta_k_1;
float Ubeta_k_2;
float Error_In;
float Error_Out;
float Error_Out_k_1;
float Usd;
float Usq;
float Usq_k_1;
float Heso_in_LPF;
float Heso_out_LPF;
float Theta;
float Kp_dsp;
float Ki_dsp;
float step;
float Urms;
float w2_P;
float w2_I;
float w2_I_k_1;
float w2;
float w;
float w_k_1;
float b0_1;
float b1_1 ;
float b2_1;
float a0_1;
float a1_1;
float a2_1;
float b0_2;
float b1_2;
float b2_2;
float a0_2;
float a1_2;
float a2_2;

}PLL_1PHASE_CLA;


#define PLL_1PHASE_CLA_MACRO(v)																								\
		v.Ualpha_k =(v.b0_1*v.Vs_k) + (v.b2_1*v.Vs_k_2) + (v.a1_1*v.Ualpha_k_1) + (v.a2_1*v.Ualpha_k_2);  					\
		v.Ubeta_k =(v.b0_2*v.Vs_k) + (v.b1_2*v.Vs_k_1) + (v.b2_2*v.Vs_k_2) + (v.a1_1*v.Ubeta_k_1) + (v.a2_2*v.Ubeta_k_2); 	\
		v.Urms = CLAsqrt(v.Ualpha_k*v.Ualpha_k + v.Ubeta_k*v.Ubeta_k)/can2;                         \
		v.Error_In = (v.k*(v.Vs_k - v.Ualpha_k ));                                            		\
		v.Error_Out= (v.Heso_in_LPF*v.Error_In)+ (v.Heso_out_LPF*v.Error_Out_k_1);    				\
		v.Error_Out_k_1 =v.Error_Out;  													          	\
		v.Ubeta_k = - v.Error_Out + v.Ubeta_k;														\
																									\
		v.Usd = (v.Ualpha_k*CLAcosPU(v.Theta)) +  (v.Ubeta_k*CLAsinPU(v.Theta)) ;     				\
		v.Usq = (v.Ubeta_k*CLAcosPU(v.Theta)) - (v.Ualpha_k*CLAsinPU(v.Theta));       				\
																									\
        v.w2_P   = (v.Kp_dsp*v.Usq);                                                    			\
		v.w2_I   = (v.Ki_dsp*v.Usq) + (v.Ki_dsp*v.Usq_k_1) + v.w2_I_k_1;            				\
		v.w2     = v.w2_P + v.w2_I;                                                         		\
		v.w      = v.w2    + (100.0*PI/Us_max);                                          			\
		v.Theta   = v.Theta + (v.step*v.w)+ (v.step*v.w_k_1);                  						\
		if(v.Theta > (1.0)) v.Theta = v.Theta - (1.0);                                				\
		if(v.Theta < (0.0)) v.Theta = v.Theta + (1.0);												\
		v.Ualpha_k_2 = v.Ualpha_k_1;																\
		v.Ualpha_k_1 = v.Ualpha_k;																	\
		v.Ubeta_k_2 = v.Ubeta_k_1;																	\
		v.Ubeta_k_1 = v.Ubeta_k;																	\
		v.Vs_k_2 = v.Vs_k_1;																		\
		v.Vs_k_1 = v.Vs_k;                        													\
		v.Usq_k_1 = v.Usq;                         													\
        v.w_k_1 = v.w;																				\
        v.w2_I_k_1 = v.w2_I;


#define PLL_1PHASE_CLA_INT(v)			\
v.k = 0;								\
v.Vs_k = 0;								\
v.Vs_k_1 = 0;							\
v.Vs_k_2 = 0;							\
v.Ualpha_k = 0;							\
v.Ualpha_k_1 = 0;						\
v.Ualpha_k_2 = 0;						\
v.Ubeta_k = 0;							\
v.Ubeta_k_1 = 0;						\
v.Ubeta_k_2 = 0;						\
v.Error_In = 0;							\
v.Error_Out = 0;						\
v.Error_Out_k_1 = 0;					\
v.Usd = 0;								\
v.Usq = 0;								\
v.Usq_k_1 = 0;							\
v.Heso_in_LPF = 0;						\
v.Heso_out_LPF = 0;						\
v.Theta = 0;							\
v.Kp_dsp = 0;							\
v.Ki_dsp = 0;							\
v.step = 0;								\
v.Urms = 0;								\
v.w2_P = 0;								\
v.w2_I = 0;								\
v.w2_I_k_1 = 0;							\
v.w2 = 0;								\
v.w = 0;								\
v.w_k_1 = 0;							\
v.b0_1 = 0;								\
v.b1_1 = 0;								\
v.b2_1 = 0;								\
v.a0_1 = 0;								\
v.a1_1 = 0;								\
v.a2_1 = 0;								\
v.b0_2 = 0;								\
v.b1_2 = 0;								\
v.b2_2 = 0;								\
v.a0_2 = 0;								\
v.a1_2 = 0;								\
v.a2_2 = 0;

#endif /* 2_LIBRUARY_INCLUDE_TRANFORM_PLL_1PHASE_H_ */
