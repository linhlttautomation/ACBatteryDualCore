/*
 * PLL_1PHASE.h
 *
 *  Created on: 7 May 2017
 *      Author: dinhngock6
 */

#ifndef _PARK_1PHASE_CLA_H_
#define _PARK_1PHASE_CLA_H_

typedef struct {
float Vs_k;
float Vs_k_1;
float Vs_k_2;
float Ualpha_k;
float Ualpha_k_1;
float Ualpha_k_2;
float Ubeta_k;
float Ubeta_k_1;
float Ubeta_k_2;
float Urms;
float Ts;
float wn;
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

float b0a0_1;
float b1a0_1 ;
float b2a0_1;
float a1a0_1;
float a2a0_1;
float b0a0_2;
float b1a0_2;
float b2a0_2;
float a1a0_2;
float a2a0_2;

}PARK_1PHASE_CLA;


#define PARK_1PHASE_CLA_MACRO(v)																					 \
v.Ualpha_k = v.b0a0_1*v.Vs_k + v.b1a0_1*v.Vs_k_1 + v.b2a0_1*v.Vs_k_2- v.a1a0_1*v.Ualpha_k_1 - v.a2a0_1*v.Ualpha_k_2; \
v.Ubeta_k = v.b0a0_2*v.Vs_k + v.b1a0_2*v.Vs_k_1 + v.b2a0_2*v.Vs_k_2 - v.a1a0_2*v.Ubeta_k_1 - v.a2a0_2*v.Ubeta_k_2;   \
v.Urms = CLAsqrt(v.Ualpha_k*v.Ualpha_k + v.Ubeta_k*v.Ubeta_k)/can2;                         \
v.Ualpha_k_2 = v.Ualpha_k_1;																\
v.Ualpha_k_1 = v.Ualpha_k;																	\
v.Ubeta_k_2 = v.Ubeta_k_1;																	\
v.Ubeta_k_1 = v.Ubeta_k;																	\
v.Vs_k_2 = v.Vs_k_1;																		\
v.Vs_k_1 = v.Vs_k;


#define PARK_1PHASE_CLA_PARA(v)							\
v.b0_1 = 2*0.707*v.wn*v.Ts;								\
v.b1_1 = -2*0.707*v.wn*v.Ts;							\
v.b2_1 = 0;												\
v.a0_1 = 1 + 2*0.707*v.wn*v.Ts + v.wn*v.wn*v.Ts*v.Ts;	\
v.a1_1 = -2 - 2*0.707*v.wn*v.Ts;						\
v.a2_1 = 1;												\
														\
v.b0_2 = 2*0.707*v.wn*v.Ts*v.wn*v.Ts;					\
v.b1_2 = 0;												\
v.b2_2 = 0;												\
v.a0_2 = 1 + 2*0.707*v.wn*v.Ts + v.wn*v.wn*v.Ts*v.Ts;	\
v.a1_2 = -2 - 2*0.707*v.wn*v.Ts;						\
v.a2_2 = 1;												\
														\
v.b0a0_1 = v.b0_1/v.a0_1;								\
v.b1a0_1 = v.b1_1/v.a0_1;								\
v.b2a0_1 = v.b2_1/v.a0_1;								\
v.a1a0_1 = v.a1_1/v.a0_1;								\
v.a2a0_1 = v.a2_1/v.a0_1;								\
														\
v.b0a0_2 = v.b0_2/v.a0_2;								\
v.b1a0_2 = v.b1_2/v.a0_2;								\
v.b2a0_2 = v.b2_2/v.a0_2;								\
v.a1a0_2 = v.a1_2/v.a0_2;								\
v.a2a0_2 = v.a2_2/v.a0_2;


#define PARK_1PHASE_CLA_INT(v)			\
v.Vs_k = 0;								\
v.Vs_k_1 = 0;							\
v.Vs_k_2 = 0;							\
v.Ualpha_k = 0;							\
v.Ualpha_k_1 = 0;						\
v.Ualpha_k_2 = 0;						\
v.Ubeta_k = 0;							\
v.Ubeta_k_1 = 0;						\
v.Ubeta_k_2 = 0;						\
v.Urms = 0;                             \
v.Ts = 0;								\
v.wn = 0;								\
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
v.a2_2 = 0;								\
v.b0a0_1 = 0;								\
v.b1a0_1 = 0;								\
v.b2a0_1 = 0;								\
v.a1a0_1 = 0;								\
v.a2a0_1 = 0;								\
v.b0a0_2 = 0;								\
v.b1a0_2 = 0;								\
v.b2a0_2 = 0;								\
v.a1a0_2 = 0;								\
v.a2a0_2 = 0;


#endif /* 2_LIBRUARY_INCLUDE_TRANFORM_PLL_1PHASE_H_ */
