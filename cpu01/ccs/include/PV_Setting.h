/*
 * PV_Setting.h
 *
 *  Created on: Oct 22, 2024
 *      Author: 84339
 */

#ifndef _PV_SETTING_H_
#define _PV_SETTING_H_

// Define Level
#define LEVEL1        1           // Vong ho
#define LEVEL3        3           // Vong dong
#define LEVEL4        4           // Vong ap doc lap

#define BUILDLEVEL    LEVEL4

// Define toán học
#define can2 1.414213562
#define can3 1.732050808
#define can6 2.449489743

// Define đọc ADC
#define UDC_HCPL        AdcbResultRegs.ADCRESULT2

#define VaG_HCPL        AdcaResultRegs.ADCRESULT5
#define VbG_HCPL        AdcaResultRegs.ADCRESULT2
#define VcG_HCPL        AdcbResultRegs.ADCRESULT5

#define IA_INV_LEM      AdcaResultRegs.ADCRESULT1
#define IB_INV_LEM      AdcaResultRegs.ADCRESULT0
#define IC_INV_LEM      AdcaResultRegs.ADCRESULT4
#define IZ_INV_LEM      AdcbResultRegs.ADCRESULT4

#define T_Us             0.002 // Time sample voltage
#define Ti               0.00002 // Time sample current 0.0000154

#define LEM_1(A)     (2048.0*A/81.3)

// LEM    1.0pu current ==> 50.0A -> 2048 counts
#define LEM_2(A)     2024-132+(2048.0*A/50.0)
#define LEML_2(A)    2024+(2048.0*A/50.0/1.1)-165

#define MEAUBAT(A)   (A/800.0*4096.0+43)
#define MEAUDC(A)    (4096.0*A/1000.0/1.09+65)
#define MEAUC(A)     (A/800.0*4096.0/1.1+75)

#if (BUILDLEVEL == LEVEL3)

    #define KP_CURR_LOOP            3.0 // L filter 330uH: 3.3
    #define KI_CURR_LOOP            1500.0 // L filter 330uH: 132000.0

    #define KP_CURR_LOOP_Z          3.0 // L filter 330uH: 13.2
    #define KI_CURR_LOOP_Z          1500.0 // L filter 330uH: 528000.0

#endif
#if(BUILDLEVEL == LEVEL4)

    #define KP_CURR_LOOP_1            2.0 // Thong so moi vong ap khong tai
    #define KI_CURR_LOOP_1            0.18

    #define KP_VOLT_US_LOOP           0.00001
    #define KI_VOLT_US_LOOP           0.1

#endif


// CMPSS FLC Permission
#define CMPSS_PROTECT_UDC_UPPER         0 //

#define CMPSS_PROTECT_VaG_UPPER         0 //
#define CMPSS_PROTECT_VaG_LOWER         0 //

#define CMPSS_PROTECT_VbG_UPPER         0 //
#define CMPSS_PROTECT_VbG_LOWER         0 //

#define CMPSS_PROTECT_Ia_inv_UPPER      0
#define CMPSS_PROTECT_Ia_inv_LOWER      0

#define CMPSS_PROTECT_Ib_inv_UPPER      0
#define CMPSS_PROTECT_Ib_inv_LOWER      0

#define CMPSS_PROTECT_Ic_inv_UPPER      0 //
#define CMPSS_PROTECT_Ic_inv_LOWER      0 //

// CMPSS TPC Permission

#define CMPSS_PROTECT_Ubat_UPPER        0 //
#define CMPSS_PROTECT_Ubat_LOWER        0 //

#define CMPSS_PROTECT_Ihv_UPPER         0 //
#define CMPSS_PROTECT_Ihv_LOWER         0 //

#define CMPSS_PROTECT_Ilv_UPPER         0 //
#define CMPSS_PROTECT_Ilv_LOWER         0 //

#define CMPSS_PROTECT_Uc_UPPER          0 //
#define CMPSS_PROTECT_Uc_LOWER          0 //

// CMPSS FLC Setting
#define CMPSS_Udc_New_Protecion            300.0

#define CMPSS_Udc_Offset_New_Protecion     0.0
#define CMPSS_Vg_Offset_New_Protecion      0.0

#define CMPSS_Ig_inv_New_Protecion         15.0

// CMPSS TPC Setting

#endif /* _PV_SETTING_H_ */
