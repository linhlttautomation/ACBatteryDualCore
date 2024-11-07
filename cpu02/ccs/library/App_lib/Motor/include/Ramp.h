/*
 * Ramp.h
 *
 *  Created on: Sep 21, 2016
 *      Author: Mr
 */

#ifndef RAMP_H_
#define RAMP_H_

typedef struct {
float   TargetValue; 	// Input: Target input (pu)
float 	StepRampInc;	// Parameter: Maximum delay rate (Q0)
float   StepRampDec;	// Parameter: Minimum limit (pu)
float   SetpointValue;	// Output: Target output (pu)
float 	EqualFlag;		// Output: Flag output (Q0)
float  	rc_tmp_inc;
float  	rc_tmp_dec;
} RC_CLA;


#define RC_CLA_MACRO(v)										\
	v.rc_tmp_inc = v.TargetValue - v.SetpointValue;			\
	v.rc_tmp_dec = v.SetpointValue - v.TargetValue;			\
if(v.rc_tmp_inc > v.StepRampInc)							\
	if (v.TargetValue >= v.SetpointValue)					\
	{														\
		v.SetpointValue += v.StepRampInc;					\
		if(v.SetpointValue > (1.0))							\
			v.SetpointValue = (1.0);						\
	}														\
else v.EqualFlag = 0x7FFFFFFF;								\
if(v.rc_tmp_dec > v.StepRampDec)							\
	if (v.TargetValue <= v.SetpointValue)					\
	{														\
		v.SetpointValue -= v.StepRampDec;					\
		if(v.SetpointValue < (-1.0))						\
			v.SetpointValue =(-1.0);						\
	}														\
else v.EqualFlag = 0x7FFFFFFF;

#define RC_INT_MACRO(v)				    \
v.TargetValue = 0;						\
v.StepRampInc = 0;						\
v.StepRampDec = 0;						\
v.SetpointValue = 0;					\
v.EqualFlag  = 0;						\
v.rc_tmp_inc = 0;						\
v.rc_tmp_dec = 0;

#endif /* RAMP_H_ */
