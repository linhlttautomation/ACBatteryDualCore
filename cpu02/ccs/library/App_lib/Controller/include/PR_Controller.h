/*
 * PR_Controller.h
 *
 *  Created on: 8 May 2017
 *      Author: dinhngock6
 */

#ifndef _PR_CONTROLLER_H_
#define _PR_CONTROLLER_H_


#include "CLAmath.h"

typedef struct
{
	float Kp;
	float Ki;
	float IN_MAX;
	float OUT_MAX;
	float w1;
    float Ts;
    float phi;
    float b0;
    float b1;
    float a0;
    float a1;
    float a2;
    float A0;
    float A1;
    float A2;
    float Ki_dsp;
    float Kp_dsp;
}PR_Parameter;

typedef struct
{
	float Up     ;
	float Ui_k     ;
	float Ui_k_1   ;
	float Ui_k_2   ;
	float Error_k  ;
	float Error_k_1;
	float OUT ;

}PR_Calculator;

//---------------------------------------------------------
// Khoi tao
//---------------------------------------------------------
#define PR_PARAMETER_INT(v)             \
		v.Kp = 0;						\
		v.Ki = 0;						\
		v.IN_MAX = 0;					\
		v.OUT_MAX = 0;					\
		v.w1 = 0;						\
		v.Ts = 0;						\
		v.phi = 0;						\
		v.b0 = 0;						\
		v.b1 = 0;						\
		v.a0 = 0;						\
		v.a1 = 0;						\
		v.a2 = 0;						\
		v.A0 = 0;						\
		v.A1 = 0;						\
		v.A2 = 0;						\
		v.Ki_dsp = 0;					\
		v.Kp_dsp = 0;

#define PR_CACULATOR_INT(v)                     \
		v.Up = 0    ;							\
		v.Ui_k = 0  ;							\
		v.Ui_k_1 = 0  ;							\
		v.Ui_k_2  = 0 ;							\
		v.Error_k  = 0;							\
		v.Error_k_1 = 0;						\
		v.OUT = 0;

//----------------------------------------------------------------------------------
// MARCO
#define PR_CALCULATOR_PARAMETER(v,h)                                                    	\
		v.phi =  h*v.w1*2.0*v.Ts;                                                       	\
		v.b0  =  v.Ts*CLAcos(v.phi) ;                                           	        \
	    v.b1  = -v.Ts*(CLAcos(v.phi) + h*v.w1*v.Ts*CLAsin(v.phi));							\
		v.a0  = 1.0;                                                                    	\
		v.a1  = h*h*v.w1*v.w1*v.Ts*v.Ts - 2.0;                                          	\
		v.a2  = 1.0;                                                                    	\
	   	v.A0          = v.Ki*v.b0*v.IN_MAX/v.a0/v.OUT_MAX;									\
	   	v.A1          = v.Ki*v.b1*v.IN_MAX/v.a0/v.OUT_MAX ;									\
	   	v.A2          = v.a1/v.a0;														    \
		v.Kp_dsp      = v.Kp*v.IN_MAX/v.OUT_MAX;											\
		v.Ki_dsp      = v.Ki*v.Ts*v.IN_MAX/v.OUT_MAX;



//-----------------------------------------------------------------------------------------------
#define PR_Calcalator_MACRO(v1,v2)                              							\
v1.Up = (v2.Kp_dsp*v1.Error_k);																\
v1.Ui_k = (v2.A0*v1.Error_k) + (v2.A1*v1.Error_k_1) - (v2.A2*v1.Ui_k_1) - v1.Ui_k_2 ;		\
v1.OUT = v1.Up + v1.Ui_k;																	\
v1.Error_k_1 = v1.Error_k;																	\
v1.Ui_k_2 = v1.Ui_k_1;																		\
v1.Ui_k_1 = v1.Ui_k;																		\
if(v1.OUT > (0.9)) v1.OUT = (0.9);				    										\
if(v1.OUT < (-0.9)) v1.OUT = (-0.9);

//-----------------------------------------------------------------------------------------------
#define PR_Calcalator_CURR_MACRO(v1,v2)                              						\
v1.Up = (v2.Kp_dsp*v1.Error_k);																\
v1.Ui_k = (v2.A0*v1.Error_k) + (v2.A1*v1.Error_k_1) - (v2.A2*v1.Ui_k_1) - v1.Ui_k_2 ;		\
v1.OUT = v1.Up + v1.Ui_k;																	\
v1.Error_k_1 = v1.Error_k;																	\
v1.Ui_k_2 = v1.Ui_k_1;																		\
v1.Ui_k_1 = v1.Ui_k;																		\
if(v1.OUT > (311.0/Us_max)) v1.OUT = (311.0/Us_max);				    					\
if(v1.OUT < (-311.0/Us_max)) v1.OUT = (-311.0/Us_max);

#endif /* 2_LIBRUARY_INCLUDE_CONTROLLER_PR_CONTROLLER_H_ */
