/*
 * CLARKE_3PHASE_CLA.h
 *
 *  Created on: 6 Mar 2017
 *      Author: dinhngock6
 */

#ifndef _CLARKE_3PHASE_CLA_H_
#define _CLARKE_3PHASE_CLA_H_

typedef struct {
	float As;
	float Bs;
	float Cs;
	float Alpha;
	float Beta;
	float Gamma;
}CLARKE_3PHASE_CLA;

#define CLARKE_3PHASE_CLA_MARCO(v)                                              \
		v.Alpha = v.As;    												        \
		v.Beta  = 0.57735*(v.As +2.0*v.Bs);

#define CLARKE_3PHASE_CLA_V2_MARCO(v)                                   \
        v.Alpha=((0.66666666667)*(v.As - ((0.5)*(v.Bs+v.Cs))));         \
        v.Beta =((0.57735026913)*(v.Bs - v.Cs));                        \

#define CLARKE_3PHASE_CLA_V3_MARCO(v)                                   \
        v.Alpha = ((0.66666666667)*(v.As - ((0.5)*(v.Bs+v.Cs))));         \
        v.Beta = ((0.57735026913)*(v.Bs - v.Cs));                        \
        v.Gamma = 0.3333333333*(v.As + v.Bs + v.Cs);                    \

#define CLARKE_3PHASE_CLA_INIT(v)												\
	v.Alpha=0.0;																\
	v.Beta =0.0;																\
	v.Gamma =0.0;                                                                \
	v.As=0.0;																	\
	v.Bs=0.0;																	\
	v.Cs=0.0;


#endif /* _3PHASE_CLA_H_ */
