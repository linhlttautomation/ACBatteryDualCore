/*
 * Calculator_RMS.h
 *
 *  Created on: 5 Mar 2018
 *      Author: dinhngoc
 */

#ifndef _CALCULATOR_RMS_PV_H_
#define _CALCULATOR_RMS_PV_H_

//*********** Structure Definition ********//
typedef struct {

    float32 VabG;                // Input: Sine Signal
    float32 VcbG;                // Input: Sine Signal

    float32 VabL;                // Input: Sine Signal
    float32 VcbL;                // Input: Sine Signal

	float32	VaG;   				// Input: Sine Signal
    float32 VbG;                // Input: Sine Signal
    float32 VcG;                // Input: Sine Signal

    float32 VaL;                // Input: Sine Signal
    float32 VbL;                // Input: Sine Signal
    float32 VcL;                // Input: Sine Signal

    float32 IaInv;                // Input: Sine Signal
    float32 IbInv;                // Input: Sine Signal
    float32 IcInv;                // Input: Sine Signal

    float32 IaL;                // Input: Sine Signal
    float32 IbL;                // Input: Sine Signal
    float32 IcL;                // Input: Sine Signal

    float32 VabG_rms;                // Output: RMS Value
    float32 VcbG_rms;                 // Output: RMS Value

    float32 VabL_rms;                // Output: RMS Value
    float32 VcbL_rms;                 // Output: RMS Value

    float32 VaG_rms;   			    // Output: RMS Value
    float32 VbG_rms;                 // Output: RMS Value
    float32 VcG_rms;                 // Output: RMS Value

    float32 VaL_rms;                 // Output: RMS Value
    float32 VbL_rms;                 // Output: RMS Value
    float32 VcL_rms;                 // Output: RMS Value

    float32 IaInv_rms;                 // Output: RMS Value
    float32 IbInv_rms;                 // Output: RMS Value
    float32 IcInv_rms;                 // Output: RMS Value

    float32 IaL_rms;                 // Output: RMS Value
    float32 IbL_rms;                 // Output: RMS Value
    float32 IcL_rms;                 // Output: RMS Value

    float32 VabG_sum_rms;                 // Output: RMS Value
    float32 VcbG_sum_rms;                 // Output: RMS Value
    float32 VabL_sum_rms;                 // Output: RMS Value
    float32 VcbL_sum_rms;                 // Output: RMS Value

    float32 VaG_sum_rms;                 // Output: RMS Value
    float32 VbG_sum_rms;                 // Output: RMS Value
    float32 VcG_sum_rms;                 // Output: RMS Value

    float32 VaL_sum_rms;                 // Output: RMS Value
    float32 VbL_sum_rms;                 // Output: RMS Value
    float32 VcL_sum_rms;                 // Output: RMS Value

    float32 IaInv_sum_rms;                 // Output: RMS Value
    float32 IbInv_sum_rms;                 // Output: RMS Value
    float32 IcInv_sum_rms;                 // Output: RMS Value

    float32 IaL_sum_rms;                 // Output: RMS Value
    float32 IbL_sum_rms;                 // Output: RMS Value
    float32 IcL_sum_rms;                 // Output: RMS Value

	int32	nsamples;			// Internal: No of samples in one cycle of the sine wave
	int32   nsamplesTB;			// Internal: Number samples signals
	float32 inv_sqrt_nsamples;	// Internal: sqrt(1/( No of samples in one cycle of the sine wave))

} CALCULATOR_RMS_PV_CLA;


//*********** Structure Init Macro *******//
#define CALCULATOR_RMS_PV_CLA_INIT(v)	    \
        v.VabG_rms = 0;           \
        v.VcbG_rms = 0;           \
        v.VabL_rms = 0;           \
        v.VcbL_rms= 0;            \
        v.VaG_rms = 0;             \
        v.VbG_rms = 0;             \
        v.VcG_rms = 0;             \
        v.VaL_rms = 0;             \
        v.VbL_rms = 0;             \
        v.VcL_rms = 0;             \
                                  \
        v.IaInv_rms = 0;           \
        v.IbInv_rms = 0;           \
        v.IcInv_rms = 0;           \
                                  \
        v.IaL_rms = 0;             \
        v.IbL_rms = 0;             \
        v.IcL_rms = 0;             \
		                          \
        v.VaG_sum_rms = 0;         \
        v.VbG_sum_rms = 0;         \
        v.VcG_sum_rms = 0;         \
                                  \
        v.VaL_sum_rms = 0;         \
        v.VbL_sum_rms = 0;         \
        v.VcL_sum_rms = 0;         \
                                  \
        v.IaInv_sum_rms = 0;       \
        v.IbInv_sum_rms = 0;       \
        v.IcInv_sum_rms = 0;       \
                                  \
        v.IaL_sum_rms = 0;         \
        v.IbL_sum_rms = 0;         \
        v.IcL_sum_rms = 0;         \
                                   \
		v.nsamples= 0;						\
		v.nsamplesTB= 0;				    \
		v.inv_sqrt_nsamples= 0;				\
//*********** Macro Definition ***********//
#define CALCULATOR_RMS_PV_CLA_MARCO(v)											    \
	if(v.nsamples >= v.nsamplesTB)													\
	{																				\
		v.inv_sqrt_nsamples = __meisqrtf32(v.nsamples);								\
        v.VabG_rms = __meinvf32(__meisqrtf32(v.VabG_sum_rms))*v.inv_sqrt_nsamples;            \
        v.VcbG_rms = __meinvf32(__meisqrtf32(v.VcbG_sum_rms))*v.inv_sqrt_nsamples;            \
        /*v.VabL_rms = __meinvf32(__meisqrtf32(v.VabL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        /*v.VcbL_rms = __meinvf32(__meisqrtf32(v.VcbL_sum_rms))*v.inv_sqrt_nsamples;*/            \
		v.VaG_rms = __meinvf32(__meisqrtf32(v.VaG_sum_rms))*v.inv_sqrt_nsamples;		    \
        v.VbG_rms = __meinvf32(__meisqrtf32(v.VbG_sum_rms))*v.inv_sqrt_nsamples;            \
        v.VcG_rms = __meinvf32(__meisqrtf32(v.VcG_sum_rms))*v.inv_sqrt_nsamples;            \
        /*v.VaL_rms = __meinvf32(__meisqrtf32(v.VaL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        /*v.VbL_rms = __meinvf32(__meisqrtf32(v.VbL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        /*v.VcL_rms = __meinvf32(__meisqrtf32(v.VcL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        v.IaInv_rms = __meinvf32(__meisqrtf32(v.IaInv_sum_rms))*v.inv_sqrt_nsamples;       \
        v.IbInv_rms = __meinvf32(__meisqrtf32(v.IbInv_sum_rms))*v.inv_sqrt_nsamples;       \
        v.IcInv_rms = __meinvf32(__meisqrtf32(v.IcInv_sum_rms))*v.inv_sqrt_nsamples;       \
        /*v.IaL_rms = __meinvf32(__meisqrtf32(v.IaL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        /*v.IbL_rms = __meinvf32(__meisqrtf32(v.IbL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        /*v.IcL_rms = __meinvf32(__meisqrtf32(v.IcL_sum_rms))*v.inv_sqrt_nsamples;*/            \
                                                                                             \
        v.VabG_sum_rms = 0;                                 \
        v.VcbG_sum_rms = 0;                                 \
        /*v.VabL_sum_rms = 0;*/                                 \
        /*v.VcbL_sum_rms = 0;*/                                 \
        v.VaG_sum_rms = 0;                                 \
        v.VbG_sum_rms = 0;                                 \
        v.VcG_sum_rms = 0;                                 \
        /*v.VaL_sum_rms = 0;*/                                 \
        /*v.VbL_sum_rms = 0;*/                                 \
        /*v.VcL_sum_rms = 0;*/                                 \
        v.IaInv_sum_rms = 0;                              \
        v.IbInv_sum_rms = 0;                              \
        v.IcInv_sum_rms = 0;                              \
        /*v.IaL_sum_rms = 0;*/                                 \
        /*v.IbL_sum_rms = 0;*/                                 \
        /*v.IcL_sum_rms = 0;*/                                 \
		v.nsamples=0;																\
	  }																			    \
	else																			\
	{																				\
		v.nsamples++;																\
		v.VabG_sum_rms = v.VabG_sum_rms+(v.VabG*v.VabG);                                 \
		v.VcbG_sum_rms = v.VcbG_sum_rms+(v.VcbG*v.VcbG);                                 \
		/*v.VabL_sum_rms = v.VabL_sum_rms+(v.VabL*v.VabL);*/                                 \
		/*v.VcbL_sum_rms = v.VcbG_sum_rms+(v.VcbL*v.VcbL);*/                                 \
		v.VaG_sum_rms = v.VaG_sum_rms+(v.VaG*v.VaG);			                     \
        v.VbG_sum_rms = v.VbG_sum_rms+(v.VbG*v.VbG);                                 \
        v.VcG_sum_rms = v.VcG_sum_rms+(v.VcG*v.VcG);                                 \
        /*v.VaL_sum_rms = v.VaL_sum_rms+(v.VaL*v.VaL);*/                                 \
        /*v.VbL_sum_rms = v.VbL_sum_rms+(v.VbL*v.VbL);*/                                 \
        /*v.VcL_sum_rms = v.VcL_sum_rms+(v.VcL*v.VcL);*/                                 \
        v.IaInv_sum_rms = v.IaInv_sum_rms+(v.IaInv*v.IaInv);                         \
        v.IbInv_sum_rms = v.IbInv_sum_rms+(v.IbInv*v.IbInv);                         \
        v.IcInv_sum_rms = v.IcInv_sum_rms+(v.IcInv*v.IcInv);                         \
        /*v.IaL_sum_rms = v.IaL_sum_rms+(v.IaL*v.IaL);*/                                 \
        /*v.IbL_sum_rms = v.IbL_sum_rms+(v.IbL*v.IbL);*/                                 \
        /*v.IcL_sum_rms = v.IcL_sum_rms+(v.IcL*v.IcL);*/                                 \
	}

//-----------------------------------------
#define CALCULATOR_RMS_PV_CLA_V2_MARCO(v)                                                   \
    if(v.nsamples >= v.nsamplesTB)                                                          \
    {                                                                                       \
        v.inv_sqrt_nsamples = __meisqrtf32(v.nsamples);                                     \
        v.VabG_rms = __meinvf32(__meisqrtf32(v.VabG_sum_rms))*v.inv_sqrt_nsamples;          \
        v.VcbG_rms = __meinvf32(__meisqrtf32(v.VcbG_sum_rms))*v.inv_sqrt_nsamples;          \
        /*v.VabL_rms = __meinvf32(__meisqrtf32(v.VabL_sum_rms))*v.inv_sqrt_nsamples;*/          \
        /*v.VcbL_rms = __meinvf32(__meisqrtf32(v.VcbL_sum_rms))*v.inv_sqrt_nsamples;*/          \
                                                                                            \
        /*v.VaL_rms = __meinvf32(__meisqrtf32(v.VaL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        /*v.VbL_rms = __meinvf32(__meisqrtf32(v.VbL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        /*v.VcL_rms = __meinvf32(__meisqrtf32(v.VcL_sum_rms))*v.inv_sqrt_nsamples;*/            \
        v.IaInv_rms = __meinvf32(__meisqrtf32(v.IaInv_sum_rms))*v.inv_sqrt_nsamples;        \
        v.IbInv_rms = __meinvf32(__meisqrtf32(v.IbInv_sum_rms))*v.inv_sqrt_nsamples;        \
        v.IcInv_rms = __meinvf32(__meisqrtf32(v.IcInv_sum_rms))*v.inv_sqrt_nsamples;        \
        v.VaG_rms = __meinvf32(__meisqrtf32(v.VaG_sum_rms))*v.inv_sqrt_nsamples;            \
        v.VbG_rms = __meinvf32(__meisqrtf32(v.VbG_sum_rms))*v.inv_sqrt_nsamples;            \
        v.VcG_rms = __meinvf32(__meisqrtf32(v.VcG_sum_rms))*v.inv_sqrt_nsamples;            \
                                                                                            \
        v.VabG_sum_rms = 0;                                 \
        v.VcbG_sum_rms = 0;                                 \
        /*v.VabL_sum_rms = 0;*/                                 \
        /*v.VcbL_sum_rms = 0;*/                                 \
        /*v.VaL_sum_rms = 0;*/                                 \
        /*v.VbL_sum_rms = 0;*/                                 \
        /*v.VcL_sum_rms = 0;*/                                 \
        v.IaInv_sum_rms = 0;                              \
        v.IbInv_sum_rms = 0;                              \
        v.IcInv_sum_rms = 0;                              \
        v.VaG_sum_rms = 0;                                 \
        v.VbG_sum_rms = 0;                                 \
        v.VcG_sum_rms = 0;                                 \
        v.nsamples=0;                                                               \
      }                                                                             \
    else                                                                            \
    {                                                                               \
        v.nsamples++;                                                                \
        v.VabG_sum_rms = v.VabG_sum_rms+(v.VabG*v.VabG);                                 \
        v.VcbG_sum_rms = v.VcbG_sum_rms+(v.VcbG*v.VcbG);                                 \
        /*v.VabL_sum_rms = v.VabL_sum_rms+(v.VabL*v.VabL);*/                                 \
        /*v.VcbL_sum_rms = v.VcbL_sum_rms+(v.VcbL*v.VcbL);*/                                 \
        /*v.VaL_sum_rms = v.VaL_sum_rms+(v.VaL*v.VaL);*/                                 \
        /*v.VbL_sum_rms = v.VbL_sum_rms+(v.VbL*v.VbL);*/                                 \
        /*v.VcL_sum_rms = v.VcL_sum_rms+(v.VcL*v.VcL);*/                                 \
        v.IaInv_sum_rms = v.IaInv_sum_rms+(v.IaInv*v.IaInv);                         \
        v.IbInv_sum_rms = v.IbInv_sum_rms+(v.IbInv*v.IbInv);                         \
        v.IcInv_sum_rms = v.IcInv_sum_rms+(v.IcInv*v.IcInv);                         \
        v.VaG_sum_rms = v.VaG_sum_rms+(v.VaG*v.VaG);                                 \
        v.VbG_sum_rms = v.VbG_sum_rms+(v.VbG*v.VbG);                                 \
        v.VcG_sum_rms = v.VcG_sum_rms+(v.VcG*v.VcG);                                 \
    }


#endif /* _CALCULATOR_RMS_H_ */
