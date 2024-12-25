/*
 * ADC_CFDAB.c
 *
 *  Created on: Dec 18, 2024
 *      Author: maytinh
 */

#include "CFDAB_Setting.h"
#include "ADC_CFDAB.h"



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
