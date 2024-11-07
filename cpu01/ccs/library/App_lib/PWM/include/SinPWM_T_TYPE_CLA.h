/*
 * SPWM_CLA.h
 *
 *  Created on: 20 Feb 2017
 *      Author: dinhngock6
 */

#ifndef _SPWM_T_TYPE_CLA_H_
#define _SPWM_T_TYPE_CLA_H_

typedef struct 	{ float  Ua; 			// Input: reference alpha-axis phase voltage
				  float  Ub;			// Input: reference beta-axis phase voltage
				  float  Uc;			// Input: reference beta-axis phase voltage
				  float  Udc;				// Input: reference Udc voltage
				  float  Ta1;				// Output: reference phase-a switching function (0 , 1)
				  float  Tb1;				// Output: reference phase-b switching function (0 , 1)
				  float  Tc1;				// Output: reference phase-c switching function (0 , 1)
				  float  Ta2;				// Output: reference phase-a switching function (0 , 1)
				  float  Tb2;				// Output: reference phase-b switching function (0 , 1)
				  float  Tc2;				// Output: reference phase-c switching function (0 , 1)
				  float  Ta;	// (-1 , 1)
				  float  Tb;	// (-1 , 1)
				  float  Tc;    // (-1 , 1)
				} SPWM_T_TYPE_CLA;
#define SPWM_T_TYPE_CLA_INT(v)		\
		v.Ua = 0;			\
		v.Ub = 0;			\
		v.Uc = 0;			\
		v.Udc = 0;			\
		v.Ta1 = 0;			\
		v.Tb1 = 0;			\
		v.Tc1 = 0;			\
		v.Ta2 = 0;			\
		v.Tb2 = 0;			\
		v.Tc2 = 0;			\
		v.Ta = 0;			\
		v.Tb = 0;			\
		v.Tc = 0;


#define SPWM_T_TYPE_CLA_MARCO(v)									\
		v.Udc = 0.5*v.Udc;									        \
		v.Ta = (1.0*Us_max/Udc_max)*(v.Ua/v.Udc);					\
		v.Tb = (1.0*Us_max/Udc_max)*(v.Ub/v.Udc);					\
		v.Tc = (1.0*Us_max/Udc_max)*(v.Uc/v.Udc);					\
																	\
		v.Ta1 = v.Ta;												\
		v.Tb1 = v.Tb;												\
		v.Tc1 = v.Tc;												\
																	\
		v.Ta2 = -v.Ta;												\
		v.Tb2 = -v.Tb;												\
		v.Tc2 = -v.Tc;												\
	 /*Limit*/														\
	if (v.Ta1<0.0) v.Ta1 = 0.0; else if(v.Ta1>0.99) v.Ta1 = 0.99;		\
	if (v.Tb1<0.0) v.Tb1 = 0.0; else if(v.Tb1>0.99) v.Tb1 = 0.99;		\
	if (v.Tc1<0.0) v.Tc1 = 0.0; else if(v.Tc1>0.99) v.Tc1 = 0.99;       \
	if (v.Ta2<0.0) v.Ta2 = 0.0; else if(v.Ta2>0.99) v.Ta2 = 0.99;		\
	if (v.Tb2<0.0) v.Tb2 = 0.0; else if(v.Tb2>0.99) v.Tb2 = 0.99;		\
	if (v.Tc2<0.0) v.Tc2 = 0.0; else if(v.Tc2>0.99) v.Tc2 = 0.99;


#endif /* _SPWM_CLA_H_ */
