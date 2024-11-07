/*
 * FtoU.h
 *
 *  Created on: Sep 21, 2016
 *      Author: Mr
 */

#ifndef FTOU_H_
#define FTOU_H_



typedef struct {
	float Freq;		// Input: Frequency input
	float Uf ;			// Output: Voltage
	float He_so_K ; 		   // Input : K
	float Uboost;     // Paramater : Uboost
	float U_max;
	float Freq_max;
	float Freq_dm;
	float U_dm;
}FREQTOU_CLA;

#define FtoU_LINE_CLA_MARCO(v)													\
	v.Uf = (1.0*v.He_so_K*v.Freq*v.Freq_max/v.U_max) + v.Uboost;    			\
	if(v.Freq >= v.Freq_dm)														\
       v.Uf = v.U_dm	;


#define FtoU_SQUARE_CLA_MARCO(v)																\
   	v.Uf = (1.0*v.He_so_K*v.Freq_max*v.Freq_max*v.Freq_max*v.Freq_max/v.U_max) + v.Uboost;    \
   	if(v.Freq >= v.Freq_dm)															  			\
          v.Uf = v.U_dm	;

#define FtoU_CLA_INT(v)		\
		v.Freq    = 0;		\
        v.Uf      = 0;		\
        v.He_so_K = 0; 		\
        v.Uboost  = 0;      \
        v.U_max  = 0;		\
        v.Freq_max= 0;		\
        v.Freq_dm = 0;		\
        v.U_dm    = 0;

#endif /* FTOU_H_ */
