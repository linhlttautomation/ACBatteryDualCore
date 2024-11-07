#ifndef __PARK_CLA_H__
#define __PARK_CLA_H__

typedef struct {
				  float  alpha;			// Input : alpha
				  float  beta;		    // Input : beta
				  float  gamma;         // Input : gamma
				  float  theta; 		// Input : theta
				  float  d;		    	// Output : d 
				  float  q;	    		// Output : q
				  float  z;             // Output : z
				  float  sine;			// Output : sin(theta) 	
				  float  cos;			// Output : cos(theta)
				}PARK_CLA;  
				
#define PARK_CLA_MACRO(v)													\
																			\
	v.d = v.alpha * v.cos + v.beta  * v.sine;								\
	v.q = v.beta  * v.cos - v.alpha * v.sine; 

#define PARK_CLA_V2_MACRO(v)                                                \
                                                                            \
    v.d = v.alpha * v.sine - v.beta  * v.cos;                               \
    v.q = v.alpha  * v.cos + v.beta * v.sine;                               \

#define PARK_CLA_V3_MACRO(v)                                                \
                                                                            \
    v.d = v.alpha * v.sine - v.beta  * v.cos;                               \
    v.q = v.alpha  * v.cos + v.beta * v.sine;                               \
    v.z = v.gamma;                                                          \

#define PARK_CLA_INIT(v)													\
	v.cos=0.0;																\
	v.sine=0.0;																\
	v.d=0.0;																\
	v.q=0.0;																\
	v.z=0.0;                                                                \
	v.theta=0.0;															\
	v.alpha=0.0;															\
	v.beta=0.0;								                                \
	v.gamma=0.0;                                                            \

#endif
