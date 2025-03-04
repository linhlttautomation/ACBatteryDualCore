/*
 * CFDAB_Setting.h
 *
 *  Created on: Oct 22, 2024
 *      Author: 84339
 */

#ifndef _CFDAB_SETTING_H_
#define _CFDAB_SETTING_H_

// Define Level
#define LEVEL1          1           // Vong ho discharge
#define LEVEL1          5           // Vong kin discharge

#define BUILDLEVEL      LEVEL1
//
#define PV_INVERTER_ADC 1    // 0 HIL , 1 = Exp
#define ADC_PU_SCALE_FACTOR_16BIT        0.0000152587890625         //1/2^12=0.000244140625
#define ADC_PU_SCALE_FACTOR_12BIT        0.000244140625             //1/2^12=0.000244140625
#define ADC_PU_PPB_SCALE_FACTOR          0.000488281250             //1/2^11
#define SD_PU_SCALE_FACTOR               0.000030517578125

#define DUTY_MAX        1.0

#define Udc_max         1000.0           // Udc_max = 600
#define Ubat_max        200.0           // Ubat_max = 200
#define Ibat_max        50.0             // Base peak phase current (amp)
#define Uc_max          800.0             // Uc_max = 800



#if (PV_INVERTER_ADC == 0)
//
#endif
//
#if (PV_INVERTER_ADC == 1)

#define UDC_HCPL    AdcbResultRegs.ADCRESULT2
#define UBAT_HCPL   AdccResultRegs.ADCRESULT2
#define VC_HCPL     AdccResultRegs.ADCRESULT5
#define IHV_LEM     AdccResultRegs.ADCRESULT3
#define ILV_LEM     AdccResultRegs.ADCRESULT4
#endif

// LEM    1.0pu current ==> 50.0A -> 2048 counts
#define LEM_2(A)     2024-132+(2048.0*A/50.0)
#define LEML_2(A)    2024+(2048.0*A/50.0/1.1)-165

#define MEAUBAT(A)   (A/200.0*4096.0/1.1+43)
#define MEAUC(A)     (A/600.0*4096.0/1.1-100)

// Discharge Close_Loop
#if (BUILDLEVEL == LEVEL5)

//#define KP_VOLT_UDC_LOOP      0.002
//#define KP_VOLT_UDC_LOOP        0.0006
//#define KP_VOLT_UDC_LOOP        0.002
//
////#define KI_VOLT_UDC_LOOP     0.2860
//#define KI_VOLT_UDC_LOOP      0.02
// Vout 100V

//#define KP_VOLT_UDC_LOOP        0.0005
#define KP_VOLT_UDC_LOOP        0.0002

//#define KI_VOLT_UDC_LOOP     0.2860
//#define KI_VOLT_UDC_LOOP      0.01
#define KI_VOLT_UDC_LOOP      0.05


#define KP_VOLT_VC_LOOP     0.000001
//#define KP_VOLT_VC_LOOP     0.00001
#define KI_VOLT_VC_LOOP     0.00003
//#define KI_VOLT_VC_LOOP     0.000015
#define KD_VOLT_VC_LOOP    0

#endif

// SETTING CFDAB

#define CFDAB_Power                    2500
#define CFDAB_Voltage                  400
#define CFDAB_MaxCharge_Current        10
#define CFDAB_MaxDischarge_Current     10

#define CFDAB_UdcRef                   300
#define CFDAB_VcRef                    130

#define CFDAB_UbatRef                  6
#define CFDAB_IbatRef                  5.2

#define CFDAB_Udc_Max                  460
#define CFDAB_Udc_Min                  350

#define CFDAB_Vc_Max                   460
#define CFDAB_Vc_Min                   0

#define CFDAB_Ubat_Max                 160
#define CFDAB_Ubat_Min                 0

#define CFDAB_Ibat_Max                 10

// CMPSS TPC Permission
#define CMPSS_PROTECT_Ubat_UPPER        0
#define CMPSS_PROTECT_Ubat_LOWER        0

#define CMPSS_PROTECT_Ihv_UPPER         0
#define CMPSS_PROTECT_Ihv_LOWER         0

#define CMPSS_PROTECT_Ilv_UPPER         0
#define CMPSS_PROTECT_Ilv_LOWER         0

#define CMPSS_PROTECT_Uc_UPPER          0
#define CMPSS_PROTECT_Uc_LOWER          0

#endif /* _CFDAB_SETTING_H_ */
