/*
 * Pwm_CFDAB.c
 *
 *  Created on: Dec 18, 2024
 *      Author: maytinh
 */

#include "Pwm_CFDAB.h"

void PWM_CFDAB(int period, int deadtime)
{
    EALLOW;

// Van S1

    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;        // set shadow load
    EPwm1Regs.TBPRD = period;
    EPwm1Regs.CMPA.bit.CMPA = period;             // Fix duty at 100%
    EPwm1Regs.TBPHS.bit.TBPHS = 0;           // Phase = 180 deg
    EPwm1Regs.TBCTR = 0;
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;                   // Free run

    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;           // Slave module
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;       // Sync "flow through" mode
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;            // Count DOWN on sync (=180 deg) // chi dung khi up-down

    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;

    EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;

    // activate shadow mode for DBCTL
    EPwm1Regs.DBCTL2.bit.SHDWDBCTLMODE = 0x1;
    // reload on CTR = 0
    EPwm1Regs.DBCTL2.bit.LOADDBCTLMODE = 0x0;

    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;        // Active Hi Complimentary
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm1Regs.DBRED.bit.DBRED = deadtime;                            // dummy value for now
    EPwm1Regs.DBFED.bit.DBFED = deadtime;                            // dummy value for now

    EPwm1Regs.ETSEL.bit.SOCAEN   = 1;
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;         // CTR = 0//
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;               // Generate pulse on 1st event
    EPwm1Regs.ETCLR.bit.SOCA = 1;
    EPwm1Regs.ETPS.bit.SOCACNT = ET_1ST ;              // Generate INT on 1st event


    // Enable CNT_zero interrupt using EPWM1 Time-base
    EPwm1Regs.ETSEL.bit.INTEN = 1;                      // enable EPWM1INT generation
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;           // enable interrupt CNT_zero event
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;                 // generate interrupt on the 1st event
    EPwm1Regs.ETPS.bit.INTCNT = ET_1ST;
    EPwm1Regs.ETCLR.bit.INT = 1;
    //-----------------------------------------------------
    // Q2a   // ePWM(n+1) init.  EPWM(n+1) is a slave
    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;        // set Immediate load
    EPwm2Regs.TBPRD = period;
    EPwm2Regs.CMPA.bit.CMPA = period;             // Fix duty at 100%
    EPwm2Regs.TBPHS.bit.TBPHS = period/2;           // Phase = 180 deg
    EPwm2Regs.TBCTR = 0;
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 3;                   // Free run

    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;           // Slave module
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;       // Sync "flow through" mode
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;            // Count DOWN on sync (=180 deg)

    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    EPwm2Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;

    EPwm2Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;

    // activate shadow mode for DBCTL
    EPwm2Regs.DBCTL2.bit.SHDWDBCTLMODE = 0x1;
    // reload on CTR = 0
    EPwm2Regs.DBCTL2.bit.LOADDBCTLMODE = 0x0;

    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;        // Active Hi Complimentary
    EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm2Regs.DBRED.bit.DBRED = deadtime;                            // dummy value for now
    EPwm2Regs.DBFED.bit.DBFED = deadtime;                            // dummy value for now

    // ePWM(n+1) init.  EPWM(n+1) is a slave
    EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;             // set Immediate load
    EPwm3Regs.TBPRD = period;
    EPwm3Regs.CMPA.bit.CMPA = period;
    EPwm3Regs.TBPHS.bit.TBPHS = period/2;
    EPwm3Regs.TBCTR = 0;
    EPwm3Regs.TBCTL.bit.FREE_SOFT = 3;                 // Free run

    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;         // COUNTER_UP
    EPwm3Regs.TBCTL.bit.PHSEN   = TB_ENABLE;          // Master module
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;        //used to sync EPWM(n+1) "down-stream"
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;

    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;      // load on CTR=Zero
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;      // load on CTR=Zero
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    EPwm3Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
    EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;

    EPwm3Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;

    // activate shadow mode for DBCTL
    EPwm3Regs.DBCTL2.bit.SHDWDBCTLMODE = 0x1;
    // reload on CTR = 0
    EPwm3Regs.DBCTL2.bit.LOADDBCTLMODE = 0x0;

    EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;     // enable Dead-band module
    EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;          // Active Hi Complimentary
    EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm3Regs.DBRED.bit.DBRED = deadtime;              // dummy value for now
    EPwm3Regs.DBFED.bit.DBFED = deadtime;              // dummy value for now


    //-----------------------------------------------------

    // ePWM(n+1) init.  EPWM(n+1) is a slave
    EPwm10Regs.TBCTL.bit.PRDLD = TB_SHADOW;        // set shadow load
    EPwm10Regs.TBPRD = period;
    EPwm10Regs.CMPA.bit.CMPA = 0;             // Fix duty at 100%
    EPwm10Regs.CMPB.bit.CMPB = period;             // Fix duty at 100%
    EPwm10Regs.TBPHS.bit.TBPHS = 0;           // Phase = 180 deg
    EPwm10Regs.TBCTR = 0;
    EPwm10Regs.TBCTL.bit.FREE_SOFT = 3;                   // Free run

    EPwm10Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
    EPwm10Regs.TBCTL.bit.PHSEN = TB_ENABLE;           // Slave module
    EPwm10Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;       // Sync "flow through" mode
    EPwm10Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm10Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm10Regs.TBCTL.bit.PHSDIR = TB_UP;            // Count DOWN on sync (=180 deg)

    EPwm10Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm10Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm10Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm10Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    EPwm10Regs.AQCTLA.bit.ZRO = AQ_SET;                                                     // tao them 1 phan xung S2
    EPwm10Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm10Regs.AQCTLA.bit.CBU = AQ_SET;

    EPwm10Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm10Regs.AQCTLB.bit.CAU = AQ_CLEAR;
    EPwm10Regs.AQCTLB.bit.CBU = AQ_SET;

    // activate shadow mode for DBCTL
    EPwm10Regs.DBCTL2.bit.SHDWDBCTLMODE = 0x1;
    // reload on CTR = 0
    EPwm10Regs.DBCTL2.bit.LOADDBCTLMODE = 0x0;

    EPwm10Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm10Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;        // Active Hi Complimentary
    EPwm10Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm10Regs.DBRED.bit.DBRED = deadtime;                            // dummy value for now
    EPwm10Regs.DBFED.bit.DBFED = deadtime;                            // dummy value for now

    EDIS;
}
