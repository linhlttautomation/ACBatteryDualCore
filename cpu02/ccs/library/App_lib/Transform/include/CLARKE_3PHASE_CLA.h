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
}CLARKE_3PHASE_CLA;

#define CLARKE_3PHASE_CLA_MARCO(v)                                              \
		v.Alpha = v.As;    												\
		v.Beta  = 0.57735*(v.As +2.0*v.Bs);        						\

#define CLARKE_3PHASE_CLA_INIT(v)												\
	v.Alpha=0.0;																\
	v.Beta =0.0;																\
	v.As=0.0;																	\
	v.Bs=0.0;																	\
	v.Cs=0.0;


#endif /* _3PHASE_CLA_H_ */
