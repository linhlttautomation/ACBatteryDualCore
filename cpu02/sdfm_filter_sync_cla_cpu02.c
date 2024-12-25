// Included Files

#include "F28x_Project.h"
#include "cla_sdfm_filter_sync_shared.h"
#include "F2837xD_sdfm_drivers.h"
#include "F2837xD_struct.h"
#include "CFDAB_Variables.h"
#include "F2837xD_Examples.h"
#include "CFDAB_Setting.h"
#include "F2837xD_CMPSS_defines.h"
#include "Pwm_CFDAB.h"
#include "ADC_CFDAB.h"

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

//PWMGEN pwm1 = PWMGEN_DEFAULTS;

//
// Global variables
//
SETTING_BAT Setting_bat;
Uint16 Task8_Isr = 0;
Uint16 StartFlag = 0;
Uint16 START = 0;
Uint16 Task1_Isr,Task2_Isr = 0;
int ChannelAdc = 0;
uint16_t CLear_TZ = 1;

// CMPSS parameters for Over Current Protection TPC
Uint16  clkPrescale = 2,
        sampwin     = 30,
        thresh      = 18,
        MEA_voltUbatHi= MEAUBAT(5),
        MEA_voltUbatLo= 0,
        MEA_voltUdcHi = MEAUDC(60),
        MEA_voltUdcLo = 0,
        MEA_voltUcHi=MEAUC(10),
        MEA_voltUcLo=0;

//
// Function prototypes
//
void Cla_initMemoryMap(void);
void CLA_initCpu2Cla(void);

// Khai bao cac bien share CPU --> CLA
extern volatile CPU_TO_CLA CpuToCLA;

// Khai bao cac bien share CPU --> CLA
extern volatile CLA_TO_CPU ClaToCPU;

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
   CpuSysRegs.PCLKCR14.bit.CMPSS7 = 1;

   EDIS;

   Cla1ForceTask8();
   Init_ADC_C();

   EALLOW;
   CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   PWM_CFDAB(2000,30);

   CMPSS_Protection();

   EALLOW;
   CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;

   DINT;

   InitPieCtrl();

//
// Disable CPU __interrupts and clear all CPU __interrupt flags:
//
    IER = 0x0000;
    IFR = 0x0000;

   InitPieVectTable();

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
    Setting_bat.VcRef  = 6.67;
    Setting_bat.UbatRef = CFDAB_UbatRef;
    Setting_bat.IbatRef = CFDAB_IbatRef;

    Setting_bat.UdcMax = CFDAB_Udc_Max;
    Setting_bat.UdcMin = CFDAB_Udc_Min;
    Setting_bat.VcMax = CFDAB_Vc_Max;
    Setting_bat.VcMin = CFDAB_Vc_Min;
    Setting_bat.UbatMax = CFDAB_Ubat_Max;
    Setting_bat.UbatMin = CFDAB_Ubat_Min;
    Setting_bat.IbatMax = CFDAB_Ibat_Max;

     DelayMs(1000);

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

//        if(CLear_TZ == 1) {
//            // Xóa cờ One-shot Trip (OST) của EPWM4
//            EPwm1Regs.TZCLR.bit.OST = 1;
//            EPwm2Regs.TZCLR.bit.OST = 1;
//            EPwm3Regs.TZCLR.bit.OST = 1;
//            EPwm10Regs.TZCLR.bit.OST = 1;
//            // Đặt lại biến CLear_TZ
//            CLear_TZ = 0;
//        }


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
//    Task1_Isr++;
//    static Uint16 i = 0;
//    // hien thi
//    Vout_Display = 800.0 * ClaToCPU.ADC_CPU.Udc_CFDAB;
//    Vc_Display   = 600.0 * ClaToCPU.ADC_CPU.Vc;
//    Vin_Display  = 200.0 * ClaToCPU.ADC_CPU.Ubat;
//
//    if(i == 200) i =0;
//    switch(ChannelAdc)
//    {
//        case 0:
//            Datalog1[i] = ClaToCPU.ADC_CPU.Udc_CFDAB;
//            Datalog2[i] = ClaToCPU.ADC_CPU.Vc;
//            break;
//        case 1:
//            Datalog1[i] = ClaToCPU.ADC_CPU.Ubat;
//            Datalog2[i] = ClaToCPU.ADC_CPU.Ilv;
//            break;
//        case 2:
//            Datalog1[i] = ClaToCPU.MEASUARE_CPU.duty;
//            Datalog2[i] = ClaToCPU.MEASUARE_CPU.theta1;
//            break;
//    }
//    i++;
//    if(i == 200) i =0;
//    //asm(" ESTOP0");
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
//    PieCtrlRegs.PIEACK.all = (PIEACK_GROUP1 | PIEACK_GROUP11);
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
