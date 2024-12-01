/*
 * AVC_Variables.h
 *
 *  Created on: 5 Mar 2017
 *      Author: dinhngock6
 */

#ifndef _PV_VARIABLES_H_
#define _PV_VARIABLES_H_

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
    float Power;
    float Voltage;
    float ChargeCurrentMax;
    float DisChargeCurrentMax;

    float  UdcRef;
    float  VcRef;
    float  UbatRef;
    float  IbatRef;

    float  UdcMax;
    float  UdcMin;
    float  VcMax;
    float  VcMin;
    float  UbatMax;
    float  UbatMin;
    float  IbatMax;

}SETTING_BAT;

// CLA ---> CPU

typedef struct
{
    float Udc_CFDAB;
    float Ubat;
    float Vc;
    float Ilv;
    float Ihv;
}ADC_VALUE;


typedef struct
{
    float theta1;
    float theta2;
    float duty;
}MEASUREMENT_VAULE;

typedef struct
{
    ADC_VALUE ADC_CPU;
    MEASUREMENT_VAULE MEASUARE_CPU;
    int32 flagRun;
    int32 flagErr;
    int32 count;
}CLA_TO_CPU;

//----------------------------------------------------------------------------------------

typedef struct
{
    float UdcTesting;
    float UbatTesting;
    float UcTesting;

    float DutyTesting;
    float DutyStart;

    float PhiTesting;
    float PhiStart;

    float UbatRef;
    float IbatRef;
    float UdcRef;
    float VcRef;

    float Theta1;
    float Theta2;

    short EnableFlag;
    short EnableADC;


}CPU_TO_CLA;

#endif /* _PV_VARIABLES_H_ */
