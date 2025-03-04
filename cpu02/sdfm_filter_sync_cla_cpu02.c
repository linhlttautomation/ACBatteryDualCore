// Included Files

#include "F28x_Project.h"
#include "cla_sdfm_filter_sync_shared.h"
#include "F2837xD_sdfm_drivers.h"
#include "F2837xD_struct.h"
#include "CFDAB_Variables.h"
#include "F2837xD_Examples.h"
#include "F2837xD_adc.h"
#include "CFDAB_Setting.h"
#include "F2837xD_CMPSS_defines.h"





typedef struct {
        unsigned int PeriodMax;     // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        float MfuncA1;        // Input: EPWM1 A&B Duty cycle ratio (Q15)
        float MfuncA2;        // Input: EPWM2 A&B Duty cycle ratio (Q15)
        float MfuncB1;        // Input: EPWM3 A&B Duty cycle ratio (Q15)
        float MfuncB2;        // Input: EPWM1 A&B Duty cycle ratio (Q15)
        float MfuncC1;        // Input: EPWM2 A&B Duty cycle ratio (Q15)
        float MfuncC2;        // Input: EPWM3 A&B Duty cycle ratio (Q15)
        } PWMGEN ;

typedef PWMGEN *PWMGEN_handle;

float Datalog1[200],Datalog2[200];
float Vout_Display;
float Vin_Display;
float Vc_Display;

/*------------------------------------------------------------------------------
    Default Initializers for the F280X PWMGEN Object
    bo di 1 cai
------------------------------------------------------------------------------*/
#define F2837X_FC_PWM_GEN    { 10000,  \
                              0.0, \
                              0.0, \
                              0.0, \
                              0.0, \
                              0.0, \
                              0.0, \
                             }

#define PWMGEN_DEFAULTS     F2837X_FC_PWM_GEN

//
// Macro definitions
//
#define WAITSTEP                  asm(" RPT #255 || NOP")

PWMGEN pwm1 = PWMGEN_DEFAULTS;

//
// Global variables
//
#pragma DATA_SECTION(Setting_bat,"RAMGS0");
volatile SETTING_BAT  Setting_bat;

#pragma DATA_SECTION(Task8_Isr,"RAMGS0");
volatile Uint16  Task8_Isr = 0;

#pragma DATA_SECTION(StartFlag,"RAMGS0");
volatile Uint16  StartFlag = 0;

#pragma DATA_SECTION(START,"RAMGS0");
volatile Uint16  START = 0;

#pragma DATA_SECTION(Task1_Isr,"RAMGS0");
volatile Uint16  Task1_Isr = 0;

#pragma DATA_SECTION(Task2_Isr,"RAMGS0");
volatile Uint16  Task2_Isr = 0;

#pragma DATA_SECTION(ChannelAdc,"RAMGS0");
volatile int  ChannelAdc = 0;

// CMPSS parameters for Over Current Protection TPC

#pragma DATA_SECTION(clkPrescale, "RAMGS0");
volatile Uint16 clkPrescale = 2;

#pragma DATA_SECTION(sampwin, "RAMGS0");
volatile Uint16 sampwin = 30;

#pragma DATA_SECTION(thresh, "RAMGS0");
volatile Uint16 thresh = 18;

#pragma DATA_SECTION(LEM_curIlvHi, "RAMGS0");
volatile Uint16 LEM_curIlvHi = LEM_2(15);

#pragma DATA_SECTION(LEM_curIlvLo, "RAMGS0");
volatile Uint16 LEM_curIlvLo = LEML_2(-15);

#pragma DATA_SECTION(LEM_curIhvHi, "RAMGS0");
volatile Uint16 LEM_curIhvHi = LEM_2(15);

#pragma DATA_SECTION(LEM_curIhvLo, "RAMGS0");
volatile Uint16 LEM_curIhvLo = LEML_2(-15);

#pragma DATA_SECTION(MEA_voltUbatHi, "RAMGS0");
volatile Uint16 MEA_voltUbatHi = MEAUBAT(80);

#pragma DATA_SECTION(MEA_voltUbatLo, "RAMGS0");
volatile Uint16 MEA_voltUbatLo = 0;

#pragma DATA_SECTION(MEA_voltUcHi, "RAMGS0");
volatile Uint16 MEA_voltUcHi = MEAUC(30);

#pragma DATA_SECTION(MEA_voltUcLo, "RAMGS0");
volatile Uint16 MEA_voltUcLo = 0;

//
// Function prototypes
//
void Cla_initMemoryMap(void);
void CLA_initCpu2Cla(void);

// Khai bao cac bien share CPU --> CLA
extern volatile CPU_TO_CLA CpuToCLA;

// Khai bao cac bien share CPU --> CLA
extern volatile CLA_TO_CPU ClaToCPU;

// Init ADC C
void Init_ADC_C()
{
    Uint16 i;
    EALLOW;

    //
    //write configurations
    //
    AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    //AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdccRegs.ADCCTL2.bit.RESOLUTION = 0;
    AdccRegs.ADCCTL2.bit.SIGNALMODE = 0;
    //
    //Set pulse positions to late
    //
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //
    //power up the ADC
    //
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //
    //delay for > 1ms to allow ADC time to power up
    //
    for(i = 0; i < 1000; i++)
    {
        asm("   RPT#255 || NOP");
    }
    EDIS;


    EALLOW;

    AdccRegs.ADCSOC4CTL.bit.CHSEL = 4;          //SOC3 will convert pin C4 (67) -> Ilv_adc
    AdccRegs.ADCSOC4CTL.bit.ACQPS = 8;         //sample window is 20 SYSCLK cycles
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = 0x05;     //trigger on ePWM1 SOCA/C
    AdccRegs.ADCPPB1CONFIG.bit.CONFIG = 0;      // PPB is associated with SOC0
    AdccRegs.ADCPPB1OFFCAL.bit.OFFCAL = 0;      // Write zero to this for now till offset ISR is run

    AdccRegs.ADCSOC5CTL.bit.CHSEL = 5;          //SOC5 will convert pin C5 (64) -> Vclamp
    AdccRegs.ADCSOC5CTL.bit.ACQPS = 8;         //sample window is 11 SYSCLK cycles
    AdccRegs.ADCSOC5CTL.bit.TRIGSEL = 0x05;     //trigger on ePWM1 SOCA/C
    AdccRegs.ADCPPB1CONFIG.bit.CONFIG = 0;      // PPB is associated with SOC0
    AdccRegs.ADCPPB1OFFCAL.bit.OFFCAL = 0;      // Write zero to this for now till offset ISR is run

    AdccRegs.ADCSOC3CTL.bit.CHSEL = 3;          //SOC2 will convert pin C3 (24)-> Ihv_adc
    AdccRegs.ADCSOC3CTL.bit.ACQPS = 8;         //sample window is 11 SYSCLK cycles
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = 0x05;     //trigger on ePWM1 SOCA/C
    AdccRegs.ADCPPB2CONFIG.bit.CONFIG = 1;      // PPB is associated with SOC1
    AdccRegs.ADCPPB2OFFCAL.bit.OFFCAL = 0;      // Write zero to this for now till offset ISR is run

    AdccRegs.ADCSOC2CTL.bit.CHSEL = 2;          //SOC4 will convert pin C2 (27) -> Ubat
    AdccRegs.ADCSOC2CTL.bit.ACQPS = 8;         //sample window is 20 SYSCLK cycles
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 0x05;     //trigger on ePWM1 SOCA/C
    AdccRegs.ADCPPB1CONFIG.bit.CONFIG = 0;      // PPB is associated with SOC0
    AdccRegs.ADCPPB1OFFCAL.bit.OFFCAL = 0;      // Write zero to this for now till offset ISR is run

    AdccRegs.ADCINTSOCSEL1.all = 0x0000;          // No ADCInterrupt will trigger SOCx
    AdccRegs.ADCINTSOCSEL2.all = 0x0000;
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 3;      // EOC3 is trigger for ADCINT1
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;        // enable ADC interrupt 1
    AdccRegs.ADCINTSEL1N2.bit.INT1CONT = 1;     // ADCINT1 pulses are generated whenever an EOC pulse is generated irrespective of whether the flag bit is cleared or not.
                                               // 0 No further ADCINT2 pulses are generated until ADCINT2 flag (in ADCINTFLG register) is cleared by user.
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;      //make sure INT1 flag is cleared

   EDIS;
}

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
    EPwm3Regs.TBCTL.bit.PHSDIR = TB_DOWN;

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

void DelayMs(unsigned long ms)
{
    unsigned long count = 0;
    for(count = 0; count < ms ; count++)
    {
        DELAY_US(1000);
    }
}

void DelayS(unsigned long s)
{
    unsigned long count = 0;
    for(count = 0; count < s ; count++)
    {
        DelayMs(1000);
    }
}

void CMPSS_Protection_TPC(void)
{
        EALLOW;
    #if(CMPSS_PROTECT_Ubat_UPPER == 1)
        Cmpss6Regs.COMPCTL.bit.COMPDACE = 1;
        Cmpss6Regs.COMPCTL.bit.COMPHSOURCE = 0;
        Cmpss6Regs.COMPDACCTL.bit.DACSOURCE = 0;
        Cmpss6Regs.COMPDACCTL.bit.SWLOADSEL = 0;

        // Ubat Upper protection
        //Cmpss6Regs.DACHVALS.bit.DACVAL = MEA_voltUbatHi;
        Cmpss6Regs.DACHVALS.bit.DACVAL = 100;
        Cmpss6Regs.COMPCTL.bit.COMPHINV = 0;
        Cmpss6Regs.COMPCTL.bit.CTRIPHSEL = 2;

        Cmpss6Regs.CTRIPHFILCLKCTL.bit.CLKPRESCALE = clkPrescale_2; // Set time between samples, max : 1023
        Cmpss6Regs.CTRIPHFILCTL.bit.SAMPWIN        = sampwin_2; // # Of samples in window, max : 31
        Cmpss6Regs.CTRIPHFILCTL.bit.THRESH         = thresh_2; // Recommended : thresh > sampwin/2
        Cmpss6Regs.CTRIPHFILCTL.bit.FILINIT        = 1; // Init samples to filter input value
        Cmpss6Regs.COMPSTSCLR.bit.HLATCHCLR = 1; // Clear the status register for latched comparator events
    #endif
    //-----------------------------------------------------
    #if(CMPSS_PROTECT_Ubat_LOWER == 1)
        Cmpss6Regs.COMPCTL.bit.COMPDACE = 1;
        Cmpss6Regs.COMPCTL.bit.COMPLSOURCE = 0;
        Cmpss6Regs.COMPDACCTL.bit.DACSOURCE = 0;
        Cmpss6Regs.COMPDACCTL.bit.SWLOADSEL = 0;

        // Ubat Lower protecion
        Cmpss6Regs.DACLVALS.bit.DACVAL = MEA_voltUbatLo;

        Cmpss6Regs.COMPCTL.bit.COMPLINV = 1;
        Cmpss6Regs.COMPCTL.bit.CTRIPLSEL = 2;

        Cmpss6Regs.CTRIPLFILCLKCTL.bit.CLKPRESCALE = clkPrescale_2; // Set time between samples, max : 1023
        Cmpss6Regs.CTRIPLFILCTL.bit.SAMPWIN        = sampwin_2; // # Of samples in window, max : 31
        Cmpss6Regs.CTRIPLFILCTL.bit.THRESH         = thresh_2; // Recommended : thresh > sampwin/2
        Cmpss6Regs.CTRIPLFILCTL.bit.FILINIT        = 1; // Init samples to filter input value
        Cmpss6Regs.COMPSTSCLR.bit.LLATCHCLR = 1; // Clear the status register for latched comparator events
    #endif
    //-----------------------------------------------------
    #if(CMPSS_PROTECT_Ihv_UPPER == 1)
        Cmpss6Regs.COMPCTL.bit.COMPDACE = 1;
        Cmpss6Regs.COMPCTL.bit.COMPHSOURCE = 1;
        Cmpss6Regs.COMPDACCTL.bit.DACSOURCE = 0;
        Cmpss6Regs.COMPDACCTL.bit.SWLOADSEL = 0;

        // Ihv Upper protection
        Cmpss6Regs.DACHVALS.bit.DACVAL = LEM_curIhvHi_2;

        Cmpss6Regs.COMPCTL.bit.COMPHINV = 0;
        Cmpss6Regs.COMPCTL.bit.CTRIPHSEL = 2;

        Cmpss6Regs.CTRIPHFILCLKCTL.bit.CLKPRESCALE = clkPrescale_2; // Set time between samples, max : 1023
        Cmpss6Regs.CTRIPHFILCTL.bit.SAMPWIN        = sampwin_2; // # Of samples in window, max : 31
        Cmpss6Regs.CTRIPHFILCTL.bit.THRESH         = thresh_2; // Recommended : thresh > sampwin/2
        Cmpss6Regs.CTRIPHFILCTL.bit.FILINIT        = 1; // Init samples to filter input value
        Cmpss6Regs.COMPSTSCLR.bit.HLATCHCLR = 1; // Clear the status register for latched comparator events
    #endif
    //-----------------------------------------------------
    #if(CMPSS_PROTECT_Ihv_LOWER == 1)
        Cmpss6Regs.COMPCTL.bit.COMPDACE = 1;
        Cmpss6Regs.COMPCTL.bit.COMPLSOURCE = 1;
        Cmpss6Regs.COMPDACCTL.bit.DACSOURCE = 0;
        Cmpss6Regs.COMPDACCTL.bit.SWLOADSEL = 0;

        // VbG Lower protecion
        Cmpss6Regs.DACLVALS.bit.DACVAL = LEM_curIhvLo_2;

        Cmpss6Regs.COMPCTL.bit.COMPLINV = 1;
        Cmpss6Regs.COMPCTL.bit.CTRIPLSEL = 2;

        Cmpss6Regs.CTRIPLFILCLKCTL.bit.CLKPRESCALE = clkPrescale_2; // Set time between samples, max : 1023
        Cmpss6Regs.CTRIPLFILCTL.bit.SAMPWIN        = sampwin_2; // # Of samples in window, max : 31
        Cmpss6Regs.CTRIPLFILCTL.bit.THRESH         = thresh_2; // Recommended : thresh > sampwin/2
        Cmpss6Regs.CTRIPLFILCTL.bit.FILINIT        = 1; // Init samples to filter input value
        Cmpss6Regs.COMPSTSCLR.bit.LLATCHCLR = 1; // Clear the status register for latched comparator events
    #endif
    //-----------------------------------------------------
    #if(CMPSS_PROTECT_Ilv_UPPER == 1)
        Cmpss5Regs.COMPCTL.bit.COMPDACE = 1;
        Cmpss5Regs.COMPCTL.bit.COMPHSOURCE = 0;
        Cmpss5Regs.COMPDACCTL.bit.DACSOURCE = 0;
        Cmpss5Regs.COMPDACCTL.bit.SWLOADSEL = 0;

        // Ilv Upper protection
        Cmpss5Regs.DACHVALS.bit.DACVAL = LEM_curIlvHi_2;
        Cmpss5Regs.COMPCTL.bit.COMPHINV = 0;
        Cmpss5Regs.COMPCTL.bit.CTRIPHSEL = 2;

        Cmpss5Regs.CTRIPHFILCLKCTL.bit.CLKPRESCALE = clkPrescale_2; // Set time between samples, max : 1023
        Cmpss5Regs.CTRIPHFILCTL.bit.SAMPWIN        = sampwin_2; // # Of samples in window, max : 31
        Cmpss5Regs.CTRIPHFILCTL.bit.THRESH         = thresh_2; // Recommended : thresh > sampwin/2
        Cmpss5Regs.CTRIPHFILCTL.bit.FILINIT        = 1; // Init samples to filter input value
        Cmpss5Regs.COMPSTSCLR.bit.HLATCHCLR = 1; // Clear the status register for latched comparator events
    #endif
    //-----------------------------------------------------
    #if(CMPSS_PROTECT_Ilv_LOWER == 1)
        Cmpss5Regs.COMPCTL.bit.COMPDACE = 1;
        Cmpss5Regs.COMPCTL.bit.COMPLSOURCE = 0;
        Cmpss5Regs.COMPDACCTL.bit.DACSOURCE = 0;
        Cmpss5Regs.COMPDACCTL.bit.SWLOADSEL = 0;

        // Ilv Lower protecion
        Cmpss5Regs.DACLVALS.bit.DACVAL = LEM_curIlvLo_2;
        Cmpss5Regs.COMPCTL.bit.COMPLINV = 1;
        Cmpss5Regs.COMPCTL.bit.CTRIPLSEL = 2;

        Cmpss5Regs.CTRIPLFILCLKCTL.bit.CLKPRESCALE = clkPrescale_2; // Set time between samples, max : 1023
        Cmpss5Regs.CTRIPLFILCTL.bit.SAMPWIN        = sampwin_2; // # Of samples in window, max : 31
        Cmpss5Regs.CTRIPLFILCTL.bit.THRESH         = thresh_2; // Recommended : thresh > sampwin/2
        Cmpss5Regs.CTRIPLFILCTL.bit.FILINIT        = 1; // Init samples to filter input value
        Cmpss5Regs.COMPSTSCLR.bit.LLATCHCLR = 1; // Clear the status register for latched comparator events
    #endif
    //-----------------------------------------------------
    #if(CMPSS_PROTECT_Uc_UPPER == 1)
        Cmpss5Regs.COMPCTL.bit.COMPDACE = 1;
        Cmpss5Regs.COMPCTL.bit.COMPHSOURCE = 1;
        Cmpss5Regs.COMPDACCTL.bit.DACSOURCE = 0;
        Cmpss5Regs.COMPDACCTL.bit.SWLOADSEL = 0;

        // Uc Upper protection
        //Cmpss5Regs.DACHVALS.bit.DACVAL = MEA_voltUcHi;
        Cmpss5Regs.DACHVALS.bit.DACVAL = 100;
        Cmpss5Regs.COMPCTL.bit.COMPHINV = 0;
        Cmpss5Regs.COMPCTL.bit.CTRIPHSEL = 2;

        // High protect
        Cmpss5Regs.CTRIPHFILCLKCTL.bit.CLKPRESCALE = clkPrescale_2; // Set time between samples, max : 1023
        Cmpss5Regs.CTRIPHFILCTL.bit.SAMPWIN        = sampwin_2; // # Of samples in window, max : 31
        Cmpss5Regs.CTRIPHFILCTL.bit.THRESH         = thresh_2; // Recommended : thresh > sampwin/2
        Cmpss5Regs.CTRIPHFILCTL.bit.FILINIT        = 1; // Init samples to filter input value
        Cmpss5Regs.COMPSTSCLR.bit.HLATCHCLR = 1; // Clear the status register for latched comparator events
    #endif
    //-----------------------------------------------------
    #if(CMPSS_PROTECT_Uc_LOWER == 1)
        Cmpss5Regs.COMPCTL.bit.COMPDACE = 1;
        Cmpss5Regs.COMPCTL.bit.COMPLSOURCE = 1;
        Cmpss5Regs.COMPDACCTL.bit.DACSOURCE = 0;
        Cmpss5Regs.COMPDACCTL.bit.SWLOADSEL = 0;

        // Uc Lower protecion
        Cmpss5Regs.DACLVALS.bit.DACVAL = MEA_voltUcLo;
        Cmpss5Regs.COMPCTL.bit.COMPLINV = 1;
        Cmpss5Regs.COMPCTL.bit.CTRIPLSEL = 2;

        Cmpss5Regs.CTRIPLFILCLKCTL.bit.CLKPRESCALE = clkPrescale_2; // Set time between samples, max : 1023
        Cmpss5Regs.CTRIPLFILCTL.bit.SAMPWIN        = sampwin_2; // # Of samples in window, max : 31
        Cmpss5Regs.CTRIPLFILCTL.bit.THRESH         = thresh_2; // Recommended : thresh > sampwin/2
        Cmpss5Regs.CTRIPLFILCTL.bit.FILINIT        = 1; // Init samples to filter input value
        Cmpss5Regs.COMPSTSCLR.bit.LLATCHCLR = 1; // Clear the status register for latched comparator events
    #endif
    //-----------------------------------------------------

//    // DC Trip select tripin6
//    EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 6 ; // Tripin6
//    EPwm1Regs.TZDCSEL.bit.DCAEVT1 = 4 ; // DCAL high , DCAH don't care
//    EPwm1Regs.DCTRIPSEL.bit.DCALCOMPSEL = 6 ; // Tripin6
//
//    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 6 ; // Tripin6
//    EPwm2Regs.TZDCSEL.bit.DCAEVT1 = 4 ; // DCAL high , DCAH don't care
//    EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL = 6 ; // Tripin6
//
//    EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 6 ; // Tripin6
//    EPwm3Regs.TZDCSEL.bit.DCAEVT1 = 4 ; // DCAL high , DCAH don't care
//    EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL = 6 ; // Tripin6
//
//    EPwm10Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 6 ; // Tripin6
//    EPwm10Regs.TZDCSEL.bit.DCAEVT1 = 4 ; // DCAL high , DCAH don't care
//    EPwm10Regs.DCTRIPSEL.bit.DCALCOMPSEL = 6 ; // Tripin6
//
//    // DC Trip select tripin5
//    EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 4 ; // Tripin5
//    EPwm1Regs.TZDCSEL.bit.DCAEVT1 = 4 ; // DCAL high , DCAH don't care
//    EPwm1Regs.DCTRIPSEL.bit.DCALCOMPSEL = 4 ; // Tripin5
//
//    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 4 ; // Tripin5
//    EPwm2Regs.TZDCSEL.bit.DCAEVT1 = 4 ; // DCAL high , DCAH don't care
//    EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL = 4 ; // Tripin5
//
//    EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 4 ; // Tripin5
//    EPwm3Regs.TZDCSEL.bit.DCAEVT1 = 4 ; // DCAL high , DCAH don't care
//    EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL = 4 ; // Tripin5
//
//    EPwm10Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 4 ; // Tripin5
//    EPwm10Regs.TZDCSEL.bit.DCAEVT1 = 4 ; // DCAL high , DCAH don't care
//    EPwm10Regs.DCTRIPSEL.bit.DCALCOMPSEL = 4 ; // Tripin5
//
//    // Tripzone Select
//    EPwm1Regs.TZSEL.bit.DCAEVT1 = 1;
//    EPwm2Regs.TZSEL.bit.DCAEVT1 = 1;
//    EPwm3Regs.TZSEL.bit.DCAEVT1 = 1;
//    EPwm10Regs.TZSEL.bit.DCAEVT1 = 1;
//
//    EPwm1Regs.TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // EPWMxA will go low
//    EPwm1Regs.TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // EPWMxB will go low
//
//    EPwm2Regs.TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // EPWMxA will go low
//    EPwm2Regs.TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // EPWMxB will go low
//
//    EPwm3Regs.TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // EPWMxA will go low
//    EPwm3Regs.TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // EPWMxB will go low
//
//    EPwm10Regs.TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // EPWMxA will go low
//    EPwm10Regs.TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // EPWMxB will go low
//
//    // Clear any spurious OV trip
//    EPwm1Regs.TZCLR.bit.DCAEVT1 = 1;
//    EPwm2Regs.TZCLR.bit.DCAEVT1 = 1;
//    EPwm3Regs.TZCLR.bit.DCAEVT1 = 1;
//    EPwm10Regs.TZCLR.bit.DCAEVT1 = 1;


    EDIS;
}


//
// Main
//
int main(void)
{
//
// Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//
   InitSysCtrl();

   EALLOW;

   CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
   CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
   CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
   CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;

   CpuSysRegs.PCLKCR13.bit.ADC_C = 1;  // Bật clock cho ADC

   CpuSysRegs.PCLKCR14.bit.CMPSS5 = 1;
   CpuSysRegs.PCLKCR14.bit.CMPSS6 = 1;

   EDIS;

   Init_ADC_C();

   EALLOW;
   CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   PWM_CFDAB(2000,30);

   //CMPSS_Protection_TPC();
   CMPSS_Protection();

   EALLOW;
   CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;



//
// Clear all __interrupts and initialize PIE vector table:
// Disable CPU __interrupts
//
    DINT;

//
// Initialize PIE control registers to their default state.
// The default state is all PIE __interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
   InitPieCtrl();

//
// Disable CPU __interrupts and clear all CPU __interrupt flags:
//
    IER = 0x0000;
    IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the __interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_SysCtrl.c.
// This function is found in F2837xD_SysCtrl.c.
//
   InitPieVectTable();

//
// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
//
   EALLOW;
//   PieVectTable.SD1_INT = &Sdfm1_ISR;
//   PieVectTable.SD2_INT = &Sdfm2_ISR;
   EDIS;

   EALLOW;
//
// Enable CPU INT5 which is connected to SDFM INT
//
   IER |= M_INT11;

//
// Enable SDFM INTn in the PIE: Group 5 __interrupt 9-10
//
   PieCtrlRegs.PIEIER5.bit.INTx9 = 1;  // SDFM1 interrupt enabled
   PieCtrlRegs.PIEIER5.bit.INTx10 = 1; // SDFM2 interrupt enabled
   EINT;

//
// Configure the CLA memory spaces
//
    Cla_initMemoryMap();

//
// Configure the CLA task vectors for CPU2
//
    CLA_initCpu2Cla();

    Cla1ForceTask8andWait();
    WAITSTEP;

    EALLOW;

//
// Trigger Source for TASK1 of CLA1 = SDFM1
//
    //DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1=CLA_TRIG_SD1INT;
    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1 = 11;

//
// Trigger Source for TASK1 of CLA1 = SDFM2
//
    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK2 = 11;

//
// Lock CLA1TASKSRCSEL1 register
//
    DmaClaSrcSelRegs.CLA1TASKSRCSELLOCK.bit.CLA1TASKSRCSEL1 = 1;
    EDIS;
    // khoi tao luong dat Cpu cho CLA
    CpuToCLA.EnableADC = 0;
    DelayMs(1000);

    CpuToCLA.EnableADC = 1;
    CpuToCLA.EnableFlag = 0;
    CpuToCLA.UdcTesting = 600.0/Udc_max;

    CpuToCLA.DutyTesting = 0.64;
    CpuToCLA.DutyStart = 0.001;

    CpuToCLA.PhiTesting = 0.084;
    CpuToCLA.PhiStart = 0.001;

    DelayMs(1000);
    //------------------------------------------------------------------------------
    // khoi tao tham so ban dau cho CFDAB
    Setting_bat.Power  = CFDAB_Power;
    Setting_bat.Voltage = CFDAB_Voltage;
    Setting_bat.ChargeCurrentMax = CFDAB_MaxCharge_Current;
    Setting_bat.DisChargeCurrentMax = CFDAB_MaxDischarge_Current;

    Setting_bat.UdcRef = CFDAB_UdcRef;
    Setting_bat.VcRef  = CFDAB_VcRef;
    Setting_bat.UbatRef = CFDAB_UbatRef;
    Setting_bat.IbatRef = CFDAB_IbatRef;

    Setting_bat.UdcMax = CFDAB_Udc_Max;
    Setting_bat.UdcMin = CFDAB_Udc_Min;
    Setting_bat.VcMax = CFDAB_Vc_Max;
    Setting_bat.VcMin = CFDAB_Vc_Min;
    Setting_bat.UbatMax = CFDAB_Ubat_Max;
    Setting_bat.UbatMin = CFDAB_Ubat_Min;
    Setting_bat.IbatMax = CFDAB_Ibat_Max;

    // DelayMs(1000);

    while(1)
    {
        if(START == 1)
        {
            // LEVEL1
            #if(BUILDLEVEL == LEVEL1)
                CpuToCLA.EnableFlag = 1;
            #endif
            // LEVEL5
            #if(BUILDLEVEL == LEVEL5)
            // if(Setting_bat.UdcRef > BAT_UDC_REF) Setting_bat.UdcRef = BAT_UDC_REF;
            if(Setting_bat.UdcRef > 330) Setting_bat.UdcRef = 330;
            // if(Setting_bat.VcRef > BAT_VC_REF) Setting_bat.VcRef = BAT_VC_REF;
            if(Setting_bat.VcRef > 170) Setting_bat.VcRef = 170;

            CpuToCLA.UdcRef = Setting_bat.UdcRef/Udc_max;
            CpuToCLA.VcRef = Setting_bat.VcRef/Uc_max;
            CpuToCLA.EnableFlag = 1;
            #endif
        }

        else
        {
            CpuToCLA.EnableFlag = 0;
        }

        if(EPwm1Regs.TZFLG.bit.OST == 1)
        {
            START = 0;
        }

        //asm(" NOP");
    }
}

//
// Cla_initMemoryMap - Initialize CLA memory
//
void Cla_initMemoryMap(void)
{
    EALLOW;

    //
    // Initialize and wait for CLA1ToCPUMsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    //
    // Initialize and wait for CPUToCLA1MsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    //
    // Select LS1 and LS2 RAM to be the programming space for the CLA
    // Select LS5 to be data RAM for the CLA
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;

//    MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
//    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 0;
//
//    //
//    // Filter1 and Filter2 data memory LS0
//    //
//    MemCfgRegs.LSxMSEL.bit.MSEL_LS2 = 1;      // LS2RAM is shared between
//                                              // CPU and CLA
//    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS2 = 0;  // LS2RAM is configured as
//                                              // data memory
//
//    //
//    // Filter3 and Filter4 data memory LS3
//    //
//    MemCfgRegs.LSxMSEL.bit.MSEL_LS3 = 1;      // LS3RAM is shared between
//                                              // CPU and CLA
//    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS3 = 0;  // LS3RAM is configured as
//                                              // data memory
//
//    MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;      // LS5RAM is shared between
//                                              // CPU and CLA
//    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;  // LS5RAM is configured as
//                                              // program memory

    MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 1;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS2 = 1; //LS2RAM is shared between CPU and CLA
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS2 = 1; //LS2RAM setup as data memory

    MemCfgRegs.LSxMSEL.bit.MSEL_LS3 = 1; //LS3RAM is shared between CPU and CLA
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS3 = 1; //LS3RAM setup as data memory

    MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1; //LS4RAM is shared between CPU and CLA
//    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 0; //LS4RAM setup as data memory
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 1; //LS4RAM setup as data memory

    MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1; //LS5RAM is shared between CPU and CLA
//    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1; //LS5RAM setup as data memory
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1; //LS5RAM setup as data memory

    EDIS;
}

//
// CLA_initCpu2Cla - Initialize CLA task vectors and end of task interrupts
//
void CLA_initCpu2Cla(void)
{
    //
    // Compute all CLA task vectors
    // on soprano the the VECT register has 16 bits so no need to subtract
    // an offset
    //
    EALLOW;
    Cla1Regs.MVECT1 = (Uint16)(&Cla1Task1);
    Cla1Regs.MVECT2 = (Uint16)(&Cla1Task2);
    Cla1Regs.MVECT3 = (Uint16)(&Cla1Task3);
    Cla1Regs.MVECT4 = (Uint16)(&Cla1Task4);
    Cla1Regs.MVECT5 = (Uint16)(&Cla1Task5);
    Cla1Regs.MVECT6 = (Uint16)(&Cla1Task6);
    Cla1Regs.MVECT7 = (Uint16)(&Cla1Task7);
    Cla1Regs.MVECT8 = (Uint16)(&Cla1Task8);

    //
    // Enable IACK instruction to start a task on CLA
    // and for all  the CLA tasks
    //
    asm("   RPT #3 || NOP");
    Cla1Regs.MCTL.bit.IACKE = 1;
//    Cla1Regs.MIER.all = 0x0003;
    Cla1Regs.MIER.all = 0x0083;
    //
    // Enable CLA interrupts at the group and subgroup levels
    //
    PieVectTable.CLA1_1_INT = &cla1Isr1;
    PieVectTable.CLA1_2_INT = &cla1Isr2;
    PieVectTable.CLA1_3_INT = &cla1Isr3;
    PieVectTable.CLA1_4_INT = &cla1Isr4;
    PieVectTable.CLA1_5_INT = &cla1Isr5;
    PieVectTable.CLA1_6_INT = &cla1Isr6;
    PieVectTable.CLA1_7_INT = &cla1Isr7;
    PieVectTable.CLA1_8_INT = &cla1Isr8;

    //
    // Enable CLA interrupts at the group and subgroup levels
    //
    PieCtrlRegs.PIEIER11.all = 0xFFFF;
    IER |= (M_INT11 );
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
    EDIS;
}

//
// cla1Isr1 - CLA1 ISR 1
//
interrupt void cla1Isr1 ()
{
    Task1_Isr++;
    static Uint16 i = 0;
    // hien thi
    Vout_Display = 800.0 * ClaToCPU.ADC_CPU.Udc_CFDAB;
    Vc_Display   = 600.0 * ClaToCPU.ADC_CPU.Vc;
    Vin_Display  = 200.0 * ClaToCPU.ADC_CPU.Ubat;

    if(i == 200) i =0;
    switch(ChannelAdc)
    {
        case 0:
            Datalog1[i] = ClaToCPU.ADC_CPU.Udc_CFDAB;
            Datalog2[i] = ClaToCPU.ADC_CPU.Vc;
            break;
        case 1:
            Datalog1[i] = ClaToCPU.ADC_CPU.Ubat;
            Datalog2[i] = ClaToCPU.ADC_CPU.Ilv;
            break;
        case 2:
            Datalog1[i] = ClaToCPU.MEASUARE_CPU.duty;
            Datalog2[i] = ClaToCPU.MEASUARE_CPU.theta1;
            break;
    }
    i++;
    if(i == 200) i =0;
    //asm(" ESTOP0");
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    PieCtrlRegs.PIEACK.all = (PIEACK_GROUP1 | PIEACK_GROUP11);
    PieCtrlRegs.PIEACK.all = M_INT11;
}

//
// cla1Isr2 - CLA1 ISR 2
//
interrupt void cla1Isr2 ()
{
     //asm(" ESTOP0");
     PieCtrlRegs.PIEACK.all = M_INT11;
}

//
// cla1Isr3 - CLA1 ISR 3
//
interrupt void cla1Isr3 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr4 - CLA1 ISR 4
//
interrupt void cla1Isr4 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr5 - CLA1 ISR 5
//
interrupt void cla1Isr5 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr6 - CLA1 ISR 6
//
interrupt void cla1Isr6 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr7 - CLA1 ISR 7
//
interrupt void cla1Isr7 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr8 - CLA1 ISR 8
//
interrupt void cla1Isr8 ()
{
    // asm(" ESTOP0");
    Task8_Isr++;
    PieCtrlRegs.PIEACK.all = M_INT11;
}

void cmpssConfig(volatile struct CMPSS_REGS *v, int16 Hi, int16 Lo)
{

    // Set up COMPCTL register
    EALLOW;
    v->COMPCTL.bit.COMPDACE    = 1;             // Enable CMPSS
    v->COMPCTL.bit.COMPLSOURCE = NEGIN_DAC;     // NEG signal from DAC for COMP-L
    v->COMPCTL.bit.COMPHSOURCE = NEGIN_DAC;     // NEG signal from DAC for COMP-H
    v->COMPCTL.bit.COMPHINV    = 0;             // COMP-H output is NOT inverted
    v->COMPCTL.bit.COMPLINV    = 1;             // COMP-L output is inverted
    v->COMPCTL.bit.ASYNCHEN    = 0;             // Disable aynch COMP-H ouput
    v->COMPCTL.bit.ASYNCLEN    = 0;             // Disable aynch COMP-L ouput
    v->COMPCTL.bit.CTRIPHSEL    = CTRIP_FILTER; // Dig filter output ==> CTRIPH
    v->COMPCTL.bit.CTRIPOUTHSEL = CTRIP_FILTER; // Dig filter output ==> CTRIPOUTH
    v->COMPCTL.bit.CTRIPLSEL    = CTRIP_FILTER; // Dig filter output ==> CTRIPL
    v->COMPCTL.bit.CTRIPOUTLSEL = CTRIP_FILTER; // Dig filter output ==> CTRIPOUTL

    // Set up COMPHYSCTL register
    v->COMPHYSCTL.bit.COMPHYS   = 2; // COMP hysteresis set to 2x typical value

    // set up COMPDACCTL register
    v->COMPDACCTL.bit.SELREF    = REFERENCE_VDDA_CMPSS; // VDDA is REF for CMPSS DACs
    v->COMPDACCTL.bit.SWLOADSEL = 0; // DAC updated on sysclock
    v->COMPDACCTL.bit.DACSOURCE = 0; // Ramp bypassed

    // Load DACs - High and Low
    v->DACHVALS.bit.DACVAL = Hi;     // Set DAC-H to allowed MAX +ve current
    v->DACLVALS.bit.DACVAL = Lo;     // Set DAC-L to allowed MAX -ve current

    // digital filter settings - HIGH side
    v->CTRIPHFILCLKCTL.bit.CLKPRESCALE = clkPrescale; // set time between samples, max : 1023
    v->CTRIPHFILCTL.bit.SAMPWIN        = sampwin;     // # of samples in window, max : 31
    v->CTRIPHFILCTL.bit.THRESH         = thresh;      // recommended : thresh > sampwin/2
    v->CTRIPHFILCTL.bit.FILINIT        = 1;           // Init samples to filter input value

    // digital filter settings - LOW side
    v->CTRIPLFILCLKCTL.bit.CLKPRESCALE = clkPrescale; // set time between samples, max : 1023
    v->CTRIPLFILCTL.bit.SAMPWIN        = sampwin;     // # of samples in window, max : 31
    v->CTRIPLFILCTL.bit.THRESH         = thresh;      // recommended : thresh > sampwin/2
    v->CTRIPLFILCTL.bit.FILINIT        = 1;           // Init samples to filter input value

    // Clear the status register for latched comparator events
    v->COMPSTSCLR.bit.HLATCHCLR = 1;
    v->COMPSTSCLR.bit.LLATCHCLR = 1;


    EDIS;
    return;
}
//
void CMPSS_Protection(void)
{
    cmpssConfig(&Cmpss6Regs,MEA_voltUbatHi, MEA_voltUbatLo);  //Enable CMPSS6 - BAT VOLTAGE - 6P
    //    cmpssConfig(&Cmpss5Regs, LEM_curHi, LEM_curLo);  //Enable CMPS5 - LEM CURRENT  for ADCINC4
    //    cmpssConfig(&Cmpss7Regs,MEA_voltUcHi,MEA_voltUcLo);  //Enable CMPSS7 - Vclamp -7P
    //cmpssConfig(&Cmpss3Regs,MEA_voltUdcHi,MEA_voltUdcLo);  //Enable CMPSS3 - Vdc - 3P

    EALLOW;
    // Configure TRIP 4 to OR the High and Low trips from both comparator 1 & 3
    // Clear everything first
//    EPwmXbarRegs.TRIP4MUX0TO15CFG.all  = 0x0000;
//    EPwmXbarRegs.TRIP4MUX16TO31CFG.all = 0x0000;
//        EPwmXbarRegs.TRIP5MUX0TO15CFG.all  = 0x0000;
//        EPwmXbarRegs.TRIP5MUX16TO31CFG.all = 0x0000;
//    // Enable Muxes for ored input of CMPSS1H and 1L, i.e. .1 mux for Mux0
//    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX10  = 0;
//    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX12  = 0;
//    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX4  = 0;
//    //    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX10  = 0;  //cmpss2 - tripH ubat 6P
//    //    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX4  = 0;  //cmpss3 - tripH
//    //    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX6  = 0;  //cmpss4 - tripH
//    //    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX8  = 1;  //cmpss5 - tripH or TripL
//
//
//    // Disable all the muxes first
//    EPwmXbarRegs.TRIP4MUXENABLE.all = 0x0000;
//    //    EPwmXbarRegs.TRIP5MUXENABLE.all = 0x0000;
//    // Enable Mux 2,4,6,8 to generate TRIP4
//    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX10  = 1;
//    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX12  = 1;
//    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX4  = 1;
////    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX4  = 1;
////    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX6  = 1;
////    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX8  = 1;


//    CMPSS6_CTRL_REG.bit.HI_TRIP = 1;

    EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 4; //Trip 4 is the input to the DCAHCOMPSEL
    EPwm1Regs.TZDCSEL.bit.DCAEVT1       = TZ_DCAH_HI;
    EPwm1Regs.DCACTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm1Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm1Regs.TZSEL.bit.DCAEVT1         = 1;           // 1/0 - Enable/Disable One Shot Mode

    EPwm1Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 4; //Trip 4 is the input to the DCBHCOMPSEL
    EPwm1Regs.TZDCSEL.bit.DCBEVT1       = TZ_DCBH_HI;
    EPwm1Regs.DCBCTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm1Regs.DCBCTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm1Regs.TZSEL.bit.DCBEVT1         = 1;           // 1/0 - Enable/Disable One Shot Mode

    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 4; //Trip 4 is the input to the DCAHCOMPSEL
    EPwm2Regs.TZDCSEL.bit.DCAEVT1       = TZ_DCAH_HI;
    EPwm2Regs.DCACTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm2Regs.TZSEL.bit.DCAEVT1         = 1;

    EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 4; //Trip 4 is the input to the DCBHCOMPSEL
    EPwm2Regs.TZDCSEL.bit.DCBEVT1       = TZ_DCBH_HI;
    EPwm2Regs.DCBCTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm2Regs.DCBCTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm2Regs.TZSEL.bit.DCBEVT1         = 1;           // 1/0 - Enable/Disable One Shot Mode

    EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 4; //Trip 4 is the input to the DCAHCOMPSEL
    EPwm3Regs.TZDCSEL.bit.DCAEVT1       = TZ_DCAH_HI;
    EPwm3Regs.DCACTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm3Regs.TZSEL.bit.DCAEVT1         = 1;

    EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 4; //Trip 4 is the input to the DCBHCOMPSEL
    EPwm3Regs.TZDCSEL.bit.DCBEVT1       = TZ_DCBH_HI;
    EPwm3Regs.DCBCTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm3Regs.DCBCTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm3Regs.TZSEL.bit.DCBEVT1         = 1;           // 1/0 - Enable/Disable One Shot Mode

    EPwm10Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 4; //Trip 4 is the input to the DCAHCOMPSEL
    EPwm10Regs.TZDCSEL.bit.DCAEVT1       = TZ_DCAH_HI;
    EPwm10Regs.DCACTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm10Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm10Regs.TZSEL.bit.DCAEVT1         = 1;

    EPwm10Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 4; //Trip 4 is the input to the DCBHCOMPSEL
    EPwm10Regs.TZDCSEL.bit.DCBEVT1       = TZ_DCBH_HI;
    EPwm10Regs.DCBCTL.bit.EVT1SRCSEL     = DC_EVT1;
    EPwm10Regs.DCBCTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm10Regs.TZSEL.bit.DCBEVT1         = 1;           // 1/0 - Enable/Disable One Shot Mode

    // What do we want the DCAEVT1 events to do?
    // TZA events can force EPWMxA
    // TZB events can force EPWMxB

    EPwm1Regs.TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // EPWMxA will go low
    EPwm1Regs.TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // EPWMxB will go low

    EPwm2Regs.TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // EPWMxA will go low
    EPwm2Regs.TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // EPWMxB will go low

    EPwm3Regs.TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // EPWMxA will go low
    EPwm3Regs.TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // EPWMxB will go low

    EPwm10Regs.TZCTL.bit.DCAEVT1 = TZ_FORCE_LO; // EPWMxA will go low
    EPwm10Regs.TZCTL.bit.DCBEVT1 = TZ_FORCE_LO; // EPWMxB will go low


    // Clear any spurious OV trip
    EPwm1Regs.TZCLR.bit.DCAEVT1 = 1;
    EPwm2Regs.TZCLR.bit.DCAEVT1 = 1;
    EPwm3Regs.TZCLR.bit.DCAEVT1 = 1;
    EPwm10Regs.TZCLR.bit.DCAEVT1 = 1;

    // Clear any spurious OV trip
    EPwm1Regs.TZCLR.bit.DCBEVT1 = 1;
    EPwm2Regs.TZCLR.bit.DCBEVT1 = 1;
    EPwm3Regs.TZCLR.bit.DCBEVT1 = 1;
    EPwm10Regs.TZCLR.bit.DCBEVT1 = 1;

    EDIS;

}



//
// End of file
//
