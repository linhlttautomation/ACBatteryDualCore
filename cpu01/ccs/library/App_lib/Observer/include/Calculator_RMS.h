/*
 * Calculator_RMS.h
 *
 *  Created on: 5 Mar 2018
 *      Author: dinhngoc
 */

#ifndef _CALCULATOR_RMS_H_
#define _CALCULATOR_RMS_H_

//*********** Structure Definition ********//
typedef struct {
	float32	in;   				// Input: Sine Signal
	float32 rms;   			    // Output: RMS Value
	float32 sum_rms;      	// Internal : running sum for vacc_rms calculation over one sine cycle
	int32	nsamples;			// Internal: No of samples in one cycle of the sine wave
	int32   nsamplesTB;			// Internal: Number samples signals
	float32 inv_sqrt_nsamples;	// Internal: sqrt(1/( No of samples in one cycle of the sine wave))
} CALCULATOR_RMS_CLA;


//*********** Structure Init Macro *******//
#define CALCULATOR_RMS_CLA_INIT(v)	        \
		v.in=0;							    \
		v.rms=0;							\
		v.sum_rms=0;					    \
		v.nsamples=0;						\
		v.nsamplesTB=0;						\
		v.inv_sqrt_nsamples=0;				\

#define CALCULATOR_RMS_V2_CLA_INIT(v)       \
        v.sum_rms=0;                        \
        v.nsamples=0;                       \

//*********** Macro Definition ***********//

#define CALCULATOR_RMS_CLA_MARCO(v)													\
	if(v.nsamples >= v.nsamplesTB)													\
	{																				\
		v.inv_sqrt_nsamples = __meisqrtf32(v.nsamples);								\
		v.rms = __meinvf32(__meisqrtf32(v.sum_rms))*v.inv_sqrt_nsamples;		    \
		v.sum_rms = 0;															    \
		v.nsamples=0;																\
	  }																			    \
	else																			\
	{																				\
		v.nsamples++;																\
		v.sum_rms = v.sum_rms+(v.in*v.in);			                                \
	}


#endif /* _CALCULATOR_RMS_H_ */
