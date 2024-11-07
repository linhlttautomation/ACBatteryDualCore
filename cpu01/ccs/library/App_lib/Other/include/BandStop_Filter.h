/*
 * LowFilter.h
 *
 *  Created on: 21 Mar 2017
 *      Author: dinhngock6
 */

#ifndef _BANDSTOP_FILTER_H_
#define _BANDSTOP_FILTER_H_

typedef struct {
				 float  a0;
		 	     float  a1;
		 	     float  a2;
		 	     float  b0;
		 	     float  b1;
		 	     float  b2;
		 	     float  b0a0;
		 	     float  b1a0;
		 	     float  b2a0;
		 	     float  a1a0;
		 	     float  a2a0;
		 	     float  w0;
		 	     float  wc;
		 	     float  Ts;
		 	     float  In_k;
		 	     float  In_k_1;
		 	     float  In_k_2;
		 	     float  Out_k;
		 	     float  Out_k_1;
		 	     float  Out_k_2;
	  	  	   } BANDSTOP_FILTER;


typedef BANDSTOP_FILTER *BANDSTOP_FILTER_hanlde;

// w0: the central rejected frequency
// wc: the width of the rejected band


#define BANDSTOP_FILTER_INT(v)				\
	v.a0 = 0;								\
	v.a1 = 0;								\
	v.a2 = 0;								\
	v.b0 = 0;								\
	v.b1 = 0;								\
	v.b2 = 0;								\
	v.a1a0 = 0;								\
	v.a2a0 = 0;								\
	v.b0a0 = 0;								\
	v.b1a0 = 0;								\
	v.b2a0 = 0;								\
	v.w0 = 0;								\
	v.wc = 0;								\
	v.In_k = 0;								\
	v.In_k_1 = 0;							\
	v.In_k_2 = 0;							\
	v.Out_k = 0;							\
	v.Out_k_1 = 0;							\
	v.Out_k_2 = 0;

#define BANDSTOP_FILTER_PARA(v)					\
v.b0 = 1 + v.w0*v.w0*v.Ts*v.Ts;					\
v.b1 = -2.0;									\
v.b2 = 1.0;										\
v.a0 = 1 + v.Ts*v.wc + v.Ts*v.Ts*v.w0*v.w0;		\
v.a1 = -2 - v.Ts*v.wc;							\
v.a2 = 1.0;										\
v.a1a0 = v.a1/v.a0;								\
v.a2a0 = v.a2/v.a0;								\
v.b0a0 = v.b0/v.a0;								\
v.b1a0 = v.b1/v.a0;								\
v.b2a0 = v.b2/v.a0;

#define BANDSTOP_FILTER_MACRO(v)																   \
v.Out_k = v.b0a0*v.In_k + v.b1a0*v.In_k_1 + v.b2a0*v.In_k_2 - v.a1a0*v.Out_k_1 - v.a2a0*v.Out_k_2; \
v.Out_k_2 = v.Out_k_1;							\
v.Out_k_1 = v.Out_k;							\
v.In_k_2 = v.In_k_1;							\
v.In_k_1 = v.In_k;


#endif /* 2_LIBRUARY_INCLUDE_OTHER_BANDSTOP_FILTER_H_ */
