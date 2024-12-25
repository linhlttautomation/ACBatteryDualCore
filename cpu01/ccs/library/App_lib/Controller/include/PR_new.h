/*
 * PR_new.h
 *
 *  Created on: 25 thg 10, 2024
 *      Author: TrungNghia
 */

#ifndef _PR_NEW_H_
#define _PR_NEW_H_
typedef struct
{
    float Kp;
    float Ki;
    float IN_MAX;
    float OUT_MAX;
    float w1;
    float Ts;
    float b0;
    float b1;
    float b2;
    float a0;
    float a1;
    float a2;
    float Ki_dsp;
    float Kp_dsp;
    float den;
}PR_Parameter_new;

typedef struct
{
    float Up     ;
    float Ui     ;
    float Ui_1   ;
    float Ui_2   ;
    float Errork  ;
    float Errork_1;
    float Errork_2;
    float OUT ;
    float OUT1 ;
}PR_Calculator_new;

#define PR_PARAMETER_INT_new(v)             \
        v.Kp = 0;                       \
        v.Ki = 0;                       \
        v.IN_MAX = 0;                   \
        v.OUT_MAX = 0;                  \
        v.w1 = 0;                       \
        v.Ts = 0;                       \
        v.b0 = 0;                       \
        v.b1 = 0;                       \
        v.b2 = 0;                       \
        v.a0 = 0;                       \
        v.a1 = 0;                       \
        v.a2 = 0;                       \
        v.Ki_dsp = 0;                   \
        v.Kp_dsp = 0;                   \
        v.den = 0;
#define PR_CACULATOR_INT_new(v)                     \
        v.Up=0;     \
        v.Ui=0;    \
        v.Ui_1 = 0  ;                         \
        v.Ui_2  = 0 ;                         \
        v.Errork  = 0;                         \
        v.Errork_1 = 0;                        \
        v.Errork_2 = 0;                        \
        v.OUT = 0;    \
        v.OUT1 = 0;
#define PR_CALCULATOR_PARAMETER_bw(v)                                                      \
        v.Kp_dsp      = v.Kp*v.IN_MAX/v.OUT_MAX;                                          \
        v.Ki_dsp      = v.Ki*v.Ts*v.IN_MAX/v.OUT_MAX;                                       \
        v.den =  v.w1*v.w1*v.Ts*v.Ts+1;                                                     \
        v.b2  = -v.den;                       \
        v.b1 = 2;                                                                           \
        v.a2  = -v.Ki_dsp;                                                                        \
        v.a1  = v.Ki_dsp;                                              \
        v.a0  = 0;

#define PR_CALCULATOR_PARAMETER_fw(v)                                                      \
        v.Kp_dsp      = v.Kp*v.IN_MAX/v.OUT_MAX;                                          \
        v.Ki_dsp      = v.Ki*v.Ts*v.IN_MAX/v.OUT_MAX;                                       \
        v.den =  v.w1*v.w1*v.Ts*v.Ts+1;                                                     \
        v.b2  = -1/v.den;                       \
        v.b1 = 2/v.den;                                                                           \
        v.a2  = 0;                                                                        \
        v.a1  = -v.Ki_dsp/v.den;                                              \
        v.a0  = v.Ki_dsp/v.den;

#define PR_CALCULATOR_PARAMETER_fw1(v)                                                      \
        v.Kp_dsp      = v.Kp*v.IN_MAX/v.OUT_MAX;                                          \
        v.Ki_dsp      = v.Ki*v.Ts*v.IN_MAX/v.OUT_MAX;                                       \
        v.den =  v.w1*v.w1*v.Ts*v.Ts+1;                                                     \
        v.b2  = -1/v.den;                       \
        v.b1 = 2/v.den;                                                                           \
        v.a2  = v.Kp_dsp/v.den;                                                                        \
        v.a1  = -(2*v.Kp_dsp+v.Ki_dsp)/v.den;                                              \
        v.a0  = v.Kp_dsp+v.Ki_dsp/v.den;

#define PR_Calcalator_MACRO_new(v1,v2)                                                          \
v1.Up= v1.Errork*v2.Kp_dsp;  \
v1.Ui= v2.a0*v1.Errork+v2.a1*v1.Errork_1+v2.a2*v1.Errork_2+v2.b1*v1.Ui_1+v2.b2*v1.Ui_2;                                  \
v1.OUT =  v1.Ui; \
v1.OUT1 = v1.Ui; \
v1.Errork_2 = v1.Errork_1;                                                                \
v1.Errork_1 = v1.Errork;                                                                  \
v1.Ui_2 = v1.Ui_1;    \
v1.Ui_1 = v1.Ui;                                                                        \
if(v1.OUT > (0.95)) v1.OUT = (0.95);                                                        \
if(v1.OUT < (-0.95)) v1.OUT = (-0.95);


#endif /* 2_LIBRUARY_APP_LIB_CONTROLLER_INCLUDE_PR_NEW_H_ */
