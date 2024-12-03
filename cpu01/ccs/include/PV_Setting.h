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

// ---------------------------------------------------
#define BUILDLEVEL    LEVEL4
// ---------------------------------------------------

#define SINGLE_PHASE_MODE       1
#define THREE_PHASE_MODE        3

// ---------------------------------------------------
#define SET_MODE_RUN            THREE_PHASE_MODE
// ---------------------------------------------------

#define READ_VOLTAGE_AC_BEFORE_LPF         1
#define READ_VOLTAGE_AC_AFTER_LPF          2
#define READ_VOLTAGE_DC                    3
#define READ_CURRENT                       4

// ---------------------------------------------------
#define SET_MODE_READ           READ_VOLTAGE_AC_AFTER_LPF
// ---------------------------------------------------

// ---------------------------------------------------
#define TUNNING_ADC              1 // 0: Auto; 1: Manual; 2: No
#define DATA1_VG_RMS             220.0
#define DATA1_IG_RMS             1.0
// ---------------------------------------------------

#define ALLOW_IPC_2CPU          1

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

// Define the base quantities for PU system conversion
#define NORMAL_FREQ     50.0
#define BASE_FREQ       150.0               // Base electrical frequency (Hz)
#define Udc_max         800.0               // Max DC Voltage (V)
#define Us_max          400.0               // Max Phase Voltage (V)
#define Is_max          10.0               // Base Peak Phase Current (A)

// CMPSS FLC Permission
#define CMPSS_PROTECT_UDC_UPPER         1 // Da test co the bao ve duoc, bv ok

#define CMPSS_PROTECT_VaG_UPPER         0 // Da test co the bao ve duoc
#define CMPSS_PROTECT_VaG_LOWER         0 // Da test co the bao ve duoc

#define CMPSS_PROTECT_VbG_UPPER         1 // Da test co the bao ve duoc, bv ok
#define CMPSS_PROTECT_VbG_LOWER         1 // Da test co the bao ve duoc, bv ok

#define CMPSS_PROTECT_Ia_inv_UPPER      0
#define CMPSS_PROTECT_Ia_inv_LOWER      0

#define CMPSS_PROTECT_Ib_inv_UPPER      0
#define CMPSS_PROTECT_Ib_inv_LOWER      0

#define CMPSS_PROTECT_Ic_inv_UPPER      1 // Da test co the bao ve duoc, bv ok
#define CMPSS_PROTECT_Ic_inv_LOWER      1 // Da test co the bao ve duoc, bv ok

// CMPSS FLC Setting
#define CMPSS_Udc_New_Protecion            80.0

#define CMPSS_Udc_Offset_New_Protecion     0.0
#define CMPSS_Vg_Offset_New_Protecion      0.0

#define CMPSS_Ig_inv_New_Protecion         3.0

// CMPSS TPC Setting

#endif /* _PV_SETTING_H_ */
