/*
 * SPWM_CLA.h
 *
 *  Created on: 20 Feb 2017
 *      Author: dinhngock6
 */

#ifndef _SPWM_T_TYPE_1Pha_CLA_H_
#define _SPWM_T_TYPE_1Pha_CLA_H_

typedef struct 	{ float  Ua; 			// Input: reference alpha-axis phase voltage
				  float  Udc;				// Input: reference Udc voltage
				  float  Udc1;
				  float  Udc2;
				  float  Ta1;				// Output: reference phase-a switching function (0 , 1)
				  float  Ta2;				// Output: reference phase-b switching function (0 , 1)
				  float  Tb1;				// Output: reference phase-a switching function (0 , 1)
				  float  Tb2;				// Output: reference phase-b switching function (0 , 1)
				  float  Ta;	// (-1 , 1)
				  int32 Count;
				  float  Tmp_Udc1;
				  float  Tmp_Udc2;
				} SPWM_T_TYPE_1Pha_CLA;

#define SPWM_T_TYPE_1Pha_CLA_INT(v)		\
		v.Ua = 0;			\
		v.Udc1 = 0;			\
		v.Udc2 = 0;			\
		v.Udc = 0;			\
		v.Ta1 = 0;			\
		v.Tb1 = 0;			\
		v.Ta2 = 0;			\
		v.Tb2 = 0;			\
		v.Ta = 0;



#define SPWM_T_TYPE_1Pha_CLA_MARCO(v)								\
		v.Udc = 0.5*v.Udc;											\
		v.Ta = (1.0*Us_max/Udc_max)*(v.Ua/v.Udc);					\
																	\
		v.Ta1 = v.Ta;												\
		v.Ta2 = v.Ta + 1.0;											\
																	\
		v.Tb1 = v.Ta + 2.0;											\
		v.Tb2 = v.Ta - 1.0;											\
	 /*Limit*/														\
	if (v.Ta1<0.0) v.Ta1 = 0.0; else if(v.Ta1>1.0) v.Ta1 = 1.0;		\
 	if (v.Tb1<0.0) v.Tb1 = 0.0; else if(v.Tb1>1.0) v.Tb1 = 1.0;		\
	if (v.Ta2<0.0) v.Ta2 = 0.0; else if(v.Ta2>1.0) v.Ta2 = 1.0;		\
	if (v.Tb2<0.0) v.Tb2 = 0.0; else if(v.Tb2>1.0) v.Tb2 = 1.0;

#define SPWM_T_TYPE_1Pha_V2_CLA_MARCO(v)							\
		v.Udc = v.Udc1 + v.Udc2;									\
		v.Udc = 0.5*v.Udc;											\
		v.Ta = (1.0*Us_max/Udc_max)*(v.Ua/v.Udc);					\
																	\
		if(v.Udc1 <= v.Udc2) v.Ta1 = v.Ta - 1.0;					\
		else v.Ta1 = v.Ta;											\
		if(v.Udc1 >= v.Udc2) v.Ta2 = v.Ta + 2.0;					\
		else v.Ta2 = v.Ta + 1.0;									\
																	\
		if(v.Udc1 >= v.Udc2) v.Tb1 = v.Ta + 1.0;					\
		else v.Tb1 = v.Ta + 2.0;									\
		if(v.Udc1 <= v.Udc2) v.Tb2 = v.Ta;							\
		else v.Tb2 = v.Ta - 1.0;									\
	 /*Limit*/														\
	if (v.Ta1<0.0) v.Ta1 = 0.0; else if(v.Ta1>1.0) v.Ta1 = 1.0;		\
 	if (v.Tb1<0.0) v.Tb1 = 0.0; else if(v.Tb1>1.0) v.Tb1 = 1.0;		\
	if (v.Ta2<0.0) v.Ta2 = 0.0; else if(v.Ta2>1.0) v.Ta2 = 1.0;		\
	if (v.Tb2<0.0) v.Tb2 = 0.0; else if(v.Tb2>1.0) v.Tb2 = 1.0;		\
	/* Dao */														\
	v.Ta1 = 1.0 - v.Ta1;											\
	v.Tb2 = 1.0 - v.Tb2;


#define SPWM_T_TYPE_1Pha_V3_CLA_MARCO(v)							\
		v.Udc = v.Udc1 + v.Udc2;									\
		v.Udc = 0.5*v.Udc;											\
		v.Ta = (1.0*Us_max/Udc_max)*(v.Ua/v.Udc);					\
																	\
																	\
	    v.Tmp_Udc1 = v.Udc1;										\
	    v.Tmp_Udc2 = v.Udc2;										\
																	\
		if(v.Tmp_Udc1 <= v.Tmp_Udc2)								\
		{															\
			v.Ta1 = v.Ta - 1.0;										\
			v.Ta2 = v.Ta + 1.0;										\
			v.Tb1 = v.Ta + 2.0;										\
			v.Tb2 = v.Ta;											\
		}															\
		else 													    \
		{															\
			v.Ta1 = v.Ta;											\
			v.Ta2 = v.Ta + 2.0;										\
			v.Tb1 = v.Ta + 1.0;										\
			v.Tb2 = v.Ta - 1.0;										\
		}															\
	 /*Limit*/														\
	if (v.Ta1<0.0) v.Ta1 = 0.0; else if(v.Ta1>1.0) v.Ta1 = 1.0;		\
 	if (v.Tb1<0.0) v.Tb1 = 0.0; else if(v.Tb1>1.0) v.Tb1 = 1.0;		\
	if (v.Ta2<0.0) v.Ta2 = 0.0; else if(v.Ta2>1.0) v.Ta2 = 1.0;		\
	if (v.Tb2<0.0) v.Tb2 = 0.0; else if(v.Tb2>1.0) v.Tb2 = 1.0;		\
	/* Dao */														\
	v.Ta1 = 1.0 - v.Ta1;											\
	v.Tb2 = 1.0 - v.Tb2;											\
	if(v.Ta1 < 0.01) v.Tb2 = v.Tb2;


#define SPWM_T_TYPE_1Pha_V4_CLA_MARCO(v)							\
		v.Udc = v.Udc1 + v.Udc2;									\
		v.Udc = 0.5*v.Udc;											\
		v.Ta = (1.0*Us_max/Udc_max)*(v.Ua/v.Udc);					\
																	\
		v.Count++;													\
		if(v.Count == 2) 											\
		{															\
			v.Count = 0;											\
			v.Ta1 = 0.0;											\
			v.Ta2 = 0.0;											\
			v.Tb1 = 0.2;											\
			v.Tb2 = 0.1;											\
		}															\
		if(v.Count == 1) 											\
		{															\
			v.Ta1 = 0.5;											\
			v.Ta2 = 1.0;											\
			v.Tb1 = 0.6;											\
			v.Tb2 = 1.0;											\
		}															\
	 /*Limit*/														\
	if (v.Ta1<0.0) v.Ta1 = 0.0; else if(v.Ta1>1.0) v.Ta1 = 1.0;		\
 	if (v.Tb1<0.0) v.Tb1 = 0.0; else if(v.Tb1>1.0) v.Tb1 = 1.0;		\
	if (v.Ta2<0.0) v.Ta2 = 0.0; else if(v.Ta2>1.0) v.Ta2 = 1.0;		\
	if (v.Tb2<0.0) v.Tb2 = 0.0; else if(v.Tb2>1.0) v.Tb2 = 1.0;

#define SPWM_T_TYPE_OPEN_CLA_MARCO(v)								\
		v.Udc = 0.5*v.Udc;											\
		v.Ta = (1.0*Us_max/Udc_max)*(v.Ua/v.Udc);					\
																	\
		v.Ta1 = v.Ta;												\
		v.Ta2 = v.Ta + 1.0;											\
																	\
		v.Tb1 = v.Ta + 2.0;											\
		v.Tb2 = v.Ta - 1.0;											\
	 /*Limit*/														\
	if (v.Ta1<0.0) v.Ta1 = 0.0; else if(v.Ta1>1.0) v.Ta1 = 1.0;		\
 	if (v.Tb1<0.0) v.Tb1 = 0.0; else if(v.Tb1>1.0) v.Tb1 = 1.0;		\
	if (v.Ta2<0.0) v.Ta2 = 0.0; else if(v.Ta2>1.0) v.Ta2 = 1.0;		\
	if (v.Tb2<0.0) v.Tb2 = 0.0; else if(v.Tb2>1.0) v.Tb2 = 1.0;		\
	/* Dao */														\
	v.Ta1 = 1.0 - v.Ta1;											\
	v.Tb2 = 1.0 - v.Tb2;

#endif /* _SPWM_CLA_H_ */
