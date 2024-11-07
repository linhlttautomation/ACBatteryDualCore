/*
 * PI.h
 *
 *  Created on: 10 Mar 2017
 *      Author: dinhngock6
 */

#ifndef _P_H_
#define _P_H_

typedef struct
{
   float Kp;
   float IN_max;
   float OUT_max;
   float REF;
   float INPUT;
   float ERROR;
   float OUT;
   float LIMIT;
}P_NORMAL_CLA;

#define P_NORMAL_INT(v)                 \
        v.Kp = 0;                       \
        v.IN_max = 0;                   \
        v.OUT_max = 0;                  \
        v.REF = 0;                      \
        v.INPUT = 0;                    \
        v.ERROR = 0;                    \
        v.OUT = 0;


#define P_NORMAL_MACRO(v)                                     \
        v.OUT    = (v.Kp*v.IN_max/v.OUT_max)*v.ERROR;         \
        if(v.OUT > (v.LIMIT)) v.OUT = (v.LIMIT);              \
        if(v.OUT < (-v.LIMIT)) v.OUT = (-v.LIMIT);



#endif /* 2_LIBRUARY_INCLUDE_CONTROLLER_PI_H_ */
