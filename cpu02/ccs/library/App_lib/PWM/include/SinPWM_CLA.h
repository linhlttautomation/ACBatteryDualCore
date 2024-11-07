/*
 * SinPWM_CLA.h
 *
 *  Created on: 19 Feb 2017
 *      Author: dinhngock6
 */

#ifndef _SINPWM_CLA_H_
#define _SINPWM_CLA_H_

typedef struct 	{ float  Usa; 			// Input: reference alpha-axis phase voltage
				  float  Usb;			// Input: reference beta-axis phase voltage
				  float  Usc;			// Input: reference beta-axis phase voltage
				  float  Udc;				// Input: reference Udc voltage
				  float  Ta;				// Output: reference phase-a switching function (0 , 1)
				  float  Tb;				// Output: reference phase-b switching function (0 , 1)
				  float  Tc;				// Output: reference phase-c switching function (0 , 1)
				  float  T1;				// (-1 , 1)
				  float  T2;				// (-1 , 1)
				  float  T3;                // (-1 , 1)
				} SPWM_CLA;
#define SPWM_CLA_INT(v)		\
		v.Usa = 0;			\
		v.Usb = 0;			\
		v.Usc = 0;			\
		v.Udc = 0;			\
		v.Ta = 0;			\
		v.Tb = 0;			\
		v.Tc = 0;			\
		v.T1 = 0;			\
		v.T2 = 0;			\
		v.T3 = 0;

#define SPWM_CLA_MARCO(v)				\
		v.T1 = v.Usa/(0.50*v.Udc);		\
		v.T2 = v.Usb/(0.50*v.Udc);		\
		v.T3 = v.Usc/(0.50*v.Udc);		\
										\
	    v.Ta = v.T1/2.0 + 0.50;			\
	    v.Tb = v.T2/2.0 + 0.50;			\
	    v.Tc = v.T3/2.0 + 0.50;			\
		 /*Limit*/							\
		if (v.Ta<0.0) v.Ta = 0.0; else if(v.Ta>1.0) v.Ta = 1.0;		\
		if (v.Tb<0.0) v.Tb = 0.0; else if(v.Tb>1.0) v.Tb = 1.0;		\
		if (v.Tc<0.0) v.Tc = 0.0; else if(v.Tc>1.0) v.Tc = 1.0;

#endif /* _SINPWM_CLA_H_ */
