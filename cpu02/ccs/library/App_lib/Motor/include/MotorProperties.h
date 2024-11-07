/*
 * MotorProperties.h
 *
 *  Created on: Sep 22, 2016
 *      Author: Mr
 */

#ifndef MOTORPROPERTIES_H_
#define MOTORPROPERTIES_H_


typedef struct {
		float	power_n_motor;
		float	volate_n_motor;
		float	frequency_n_motor;
		float	current_n_motor;
		float	speed_n_motor;
		float	cosphi_n_motor;
		float	pair_pole_n_motor;

		float  Ramp_Inc_Run;				//change equal rc1.StepRampInc
		float  Ramp_Dec_Run;				//change equal rc1.StepRampDec
		float  Control_mode_Run;
		float  sinphi;
		float 	isdN;
		float 	isqN;
		float 	WrN;
		float	Tr;
		float 	Xo;
		float 	Xh;
		float   Rs;
		float 	Rr;
		float 	Lr;
		float 	o;
		float 	Ls;
		float 	Ts;
		float 	Lm;
       } MOTOR_PROPERTIES;

#define MOTOR_PROPERTIES_CLA_INT(v)		\
		v.power_n_motor = 0;			\
       v.volate_n_motor = 0;			\
       v.frequency_n_motor = 0;			\
       v.current_n_motor = 0;			\
       v.speed_n_motor = 0;				\
       v.cosphi_n_motor = 0;			\
       v.pair_pole_n_motor = 0;			\
       v.Ramp_Inc_Run = 0;				\
       v.Ramp_Dec_Run = 0;				\
       v.Control_mode_Run = 0;			\
       v.sinphi = 0;					\
       v.isdN = 0;						\
       v.isqN = 0;						\
       v.WrN = 0;						\
       v.Tr = 0;						\
       v.Xo = 0;						\
       v.Xh = 0;						\
       v.Rs = 0;						\
       v.Rr = 0;						\
       v.Lr = 0;						\
       v.o = 0;							\
       v.Ls = 0;						\
       v.Ts = 0;						\
       v.Lm = 0;

#endif /* MOTORPROPERTIES_H_ */
