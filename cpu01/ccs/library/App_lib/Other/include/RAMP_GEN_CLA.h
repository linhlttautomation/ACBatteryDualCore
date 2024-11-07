#ifndef __RAMP_GEN_CLA_H__
#define __RAMP_GEN_CLA_H__

typedef struct { float  Freq; 			// Input: Ramp frequency (pu) 	
		 	     float  StepAngleMax;	// Parameter: Maximum step angle (pu)		
	 	 	     float  Angle;			// Variable: Step angle (pu)					  
			     float  Gain;			// Input: Ramp gain (pu)
			     float  Out;  	 		// Output: Ramp signal (pu) 	
			     float  Offset;			// Input: Ramp offset (pu)
			     float  Udc;
	  	  	   } RAMP_GEN_CLA;	            

/*------------------------------------------------------------------------------
	RAMP(Sawtooh) Generator Macro Definition
------------------------------------------------------------------------------*/                                               

#define RAMP_GEN_CLA_MACRO(v)						\
													\
/* Compute the angle rate */						\
    v.Angle = v.Angle + v.StepAngleMax*v.Freq;	    \
													\
/* Saturate the angle rate within (-1,1) */			\
	if (v.Angle > 1.0)								\
		v.Angle = v.Angle - (1.0);					\
	else if (v.Angle<(-1.0))						\
		v.Angle +=(1.0);							\
	v.Out=v.Angle;                                  \

#define RAMP_GEN_CLA_V2_MACRO(v)                    \
                                                    \
	/* Compute the angle rate */                    \
    v.Out = v.Out + 0.00001;                        \
    if (v.Out > 0.1) v.Out = 0.1;                   \

#define RAMP_GEN_CLA_INIT(v)		\
	v.Angle=0.0;					\
	v.Freq=0;						\
	v.StepAngleMax=0;				\
	v.Out=0.0;					    \
	v.Udc = 0.0;                    \
	
#endif // __RAMPGEN_H__
