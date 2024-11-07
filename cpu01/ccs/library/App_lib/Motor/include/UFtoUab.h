/*
 * UFtoUab.h
 *
 *  Created on: Sep 21, 2016
 *      Author: Mr
 */

#ifndef UFTOUAB_H_
#define UFTOUAB_H_

typedef struct
{
	float  Uf;  		// Input: U  variable
	float  Alpha;		// Output: U Alpha variable
	float  Beta;		// Output: U Beta variable
	float  Sine;		// Input: Sine  variable
	float  Cosine;		// Input: Cosine  variable
} UFTOUAB_CLA;



#define UFTOUAB_CLA_MACRO(v)							\
v.Alpha = (v.Uf*v.Cosine);								\
v.Beta =  (v.Uf*v.Sine);

#define UFTOUAB_CLA_INT(v)			\
v.Uf = 0;  							\
v.Alpha = 0;						\
v.Beta  = 0;						\
v.Sine  = 0;						\
v.Cosine= 0;

#endif /* UFTOUAB_H_ */
