/*
 * Var.h
 *
 *  Created on: 10 Oct 2017
 *      Author: dinhngock6
 */

#ifndef _PV_VAR_H_
#define _PV_VAR_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	unsigned int ID;
	unsigned int WORD_ONE;
	unsigned int WORD_SEC;
	unsigned int WORD_THREE;
	unsigned int WORD_FOUR;
}MsgDataCan;

typedef struct
{
    unsigned int  Voltage;
    unsigned int  Frequency;
    unsigned int  Power;
    unsigned int  HeSo;
    int           Current;
    unsigned int  Udc;

    unsigned int  ControlMode;

    unsigned int  FrequencyMax;
    unsigned int  FrequencyMin;

    unsigned int  VLoadMin;
    unsigned int  VLoadMax;
    unsigned int  ILoadMax;

    unsigned int  VgridMin;
    unsigned int  VgridMax;
    unsigned int  IgridMax;

    unsigned int  UdcMin;
    unsigned int  UdcMax;
    unsigned int  TemperatureMax;
}SETTING_PV;

typedef struct
{

    float  FrequencyMax;
    float  FrequencyMin;

    float  VLoadMin;
    float  VLoadMax;
    float  ILoadMax;

    float  VgridMin;
    float  VgridMax;
    float  IgridMax;

    float  UdcMin;
    float  UdcMax;

    float  UcMax;
    float  UcMin;

    float  Ipeak;

    float  TemperatureMax;

}LIMIT_PV;


#endif /* PV_VAR_H_ */
