/*
 * LowFilter.h
 *
 *  Created on: 21 Mar 2017
 *      Author: dinhngock6
 */

#ifndef _LOWFILTER_H_
#define _LOWFILTER_H_

typedef struct { float  Heso_in;
		 	     float  Heso_out;
		 	     float  Vin;
		 	     float  Vout_k2;
		 	     float  Vout_k1;
		 	     float  Vout;
	  	  	   } LOWPASSFILTER;


typedef LOWPASSFILTER *LOWPASSFILTER_hanlde;
/*------------------------------------------------------------------------------
      Object Initializers
------------------------------------------------------------------------------*/
#define LOWPASSFILTERDEFAULTS {0,0,0,0,0,0}



// Heso_in = deltaT/(RC +deltaT)
// Heso_out = RC/(RC + deltaT)

// fc = 1/(2*pi*RC)
// RC = 1/(2*pi*fc)

#define LOWPASSFILTER_MACRO(v)												\
	v.Vout = v.Heso_in*v.Vin + v.Heso_out*v.Vout_k1;						\
	v.Vout_k1 = v.Vout;

#define LOWPASSFILTER_INT(v)			\
  v.Heso_in = 0;						\
  v.Heso_out = 0;						\
  v.Vin = 0;							\
  v.Vout_k2 = 0;						\
  v.Vout_k1 = 0;						\
  v.Vout = 0;


#endif /* 2_LIBRUARY_INCLUDE_OTHER_LOWFILTER_H_ */
