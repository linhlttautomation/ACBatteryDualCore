/*
 * UabtoUabc.h
 *
 *  Created on: 20 Feb 2017
 *      Author: dinhngock6
 */

#ifndef _UABTOUABC_H_
#define _UABTOUABC_H_

typedef struct {
				  float  Ualpha;			// Input : alpha
				  float  Ubeta;		        // Input : beta
				  float  Ugamma;            // Input : gamma
				  float  Ua;		    	// Output : d
				  float  Ub;	    		// Output : q
				  float  Uc;			    // Output : sin(theta)
				}UABtoUabc_CLA;

#define UABtoUabc_CLA_MACRO(v)							                    \
	v.Ua = v.Ualpha;									                    \
	v.Ub = (-0.50)*v.Ualpha  + v.Ubeta*0.8660254038;                        \
	v.Uc = (-0.50)*v.Ualpha  - v.Ubeta*0.8660254038;                        \

#define UABtoUabc_CLA_V2_MACRO(v)                                           \
    v.Ua = v.Ualpha + v.Ugamma;                                             \
    v.Ub = (-0.50)*v.Ualpha + v.Ubeta*0.8660254038 + v.Ugamma;              \
    v.Uc = (-0.50)*v.Ualpha - v.Ubeta*0.8660254038 + v.Ugamma;              \

#define UABtoUabc_CLA_INIT(v)												\
	v.Ualpha = 0.0;															\
	v.Ubeta = 0.0;															\
	v.Ugamma = 0.0;                                                         \
	v.Ua = 0.0;																\
	v.Ub = 0.0;																\
	v.Uc = 0.0;                                                             \

#endif /* _UABTOUABC_H_ */
