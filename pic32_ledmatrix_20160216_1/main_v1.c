// DEVCFG3:
#pragma config IOL1WAY  = OFF           // Peripheral Pin Select Configuration
// DEVCFG2:
// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_15         // PLL Multiplier (24x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
// DEVCFG1:
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = OFF           // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = FRCPLL        // Oscillator Selection
// DEVCFG0:
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = ON            // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx3      // ICE/ICD Comm Channel Select
#pragma config JTAGEN   = OFF           // JTAG Enable

#define SYSCLK 60000000L

#include <p32xxxx.h>
#include <plib.h>

#define GetSystemClock()       (SYSCLK)

#define red1_pin      LATBbits.LATB8
#define green1_pin    LATBbits.LATB6
#define blue1_pin     LATBbits.LATB9
#define red2_pin      LATBbits.LATB10
#define green2_pin    LATBbits.LATB7
#define blue2_pin     LATBbits.LATB11

#define clock_pin     LATBbits.LATB12
#define latch_pin     LATBbits.LATB13
#define oe_pin        LATBbits.LATB14

#define a_pin         LATAbits.LATA4
#define b_pin         LATBbits.LATB4
#define c_pin         LATAbits.LATA3


unsigned char pattern[32][32] = {
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
};

void set_abc(int v) {
    switch (v % 8) {
        case 0:
            a_pin = 1;
            b_pin = 1;
            c_pin = 1;
            break;
        case 1:
            a_pin = 0;
            b_pin = 1;
            c_pin = 1;
            break;
        case 2:
            a_pin = 1;
            b_pin = 0;
            c_pin = 1;
            break;
        case 3:
            a_pin = 0;
            b_pin = 0;
            c_pin = 1;
            break;
        case 4:
            a_pin = 1;
            b_pin = 1;
            c_pin = 0;
            break;
        case 5:
            a_pin = 0;
            b_pin = 1;
            c_pin = 0;
            break;
        case 6:
            a_pin = 1;
            b_pin = 0;
            c_pin = 0;
            break;
        case 7:
            a_pin = 0;
            b_pin = 0;
            c_pin = 0;
            break;
    }
}

void dataset(int d) {
    int i, j;

    j = d;

    for (i = 0; i < 8; i++) {
        //NO0
        red1_pin = 0; //NO0-top-down
        green1_pin = 0;
        blue1_pin = pattern[j + 8][i];

        red2_pin = 0; //NO0-btm-down
        green2_pin = 0;
        blue2_pin = pattern[j][i]; //ok

        clock_pin = 0;
        clock_pin = 1;
    }

    for (i = 0; i < 8; i++) {
        red1_pin = 0; //NO0-top-up
        green1_pin = 0;
        blue1_pin = pattern[j + 4 + 8][i];

        red2_pin = 0; //NO0-btm-up
        green2_pin = 0;
        blue2_pin = pattern[j + 4][i];

        clock_pin = 0;
        clock_pin = 1;
    }

    //
    for (i = 0; i < 8; i++) {
        //NO1
        red1_pin = 0; //NO1-top-down
        green1_pin = 0;
        blue1_pin = pattern[j + 8][i + 8];

        red2_pin = 0; //NO1-btm-down
        green2_pin = 0;
        blue2_pin = pattern[j][i + 8]; //ok

        clock_pin = 0;
        clock_pin = 1;
    }
    for (i = 0; i < 8; i++) {

        red1_pin = 0; //NO1-top-up
        green1_pin = 0;
        blue1_pin = pattern[j + 4 + 8][i + 8];

        red2_pin = 0; //NO1-btm-up
        green2_pin = 0;
        blue2_pin = pattern[j + 4 ][i + 8];

        clock_pin = 0;
        clock_pin = 1;
    }


    /////
    for (i = 0; i < 8; i++) {
        //NO2
        red1_pin = 0; //NO2-top-down
        green1_pin = 0;
        blue1_pin = pattern[j + 8][i + 16];

        red2_pin = 0; //NO2-btm-down
        green2_pin = 0;
        blue2_pin = pattern[j][i + 16]; //ok

        clock_pin = 0;
        clock_pin = 1;
    }
    for (i = 0; i < 8; i++) {
        red1_pin = 0; //NO2-top-up
        green1_pin = 0;
        blue1_pin = pattern[j + 4 + 8][i + 16];

        red2_pin = 0; //NO2-btm-up
        green2_pin = 0;
        blue2_pin = pattern[j + 4 ][i + 16];

        clock_pin = 0;
        clock_pin = 1;
    }

    for (i = 0; i < 8; i++) {
        //NO3 bottom-down
        red1_pin = 0; //NO3-top-down
        green1_pin = 0;
        blue1_pin = pattern[j + 8][i + 24];

        red2_pin = 0; //NO3-btm-down
        green2_pin = 0;
        blue2_pin = pattern[j][i + 24]; //ok

        clock_pin = 0;
        clock_pin = 1;
    }
    for (i = 0; i < 8; i++) {
        //NO3 top-up
        red1_pin = 0; //NO3-top-up
        green1_pin = 0;
        blue1_pin = pattern[j + 4 + 8][i + 24];

        red2_pin = 0; //NO3-btm-up
        green2_pin = 0;
        blue2_pin = pattern[j + 4][i + 24];

        clock_pin = 0;
        clock_pin = 1;
    }


    //-------------------------------------------------------

    for (i = 0; i < 8; i++) {
        //NO0
        red1_pin = 0; //NO0-top-down
        green1_pin = 0;
        blue1_pin = pattern[16+j + 8][i];

        red2_pin = 0; //NO0-btm-down
        green2_pin = 0;
        blue2_pin = pattern[16+j][i]; //ok

        clock_pin = 0;
        clock_pin = 1;
    }

    for (i = 0; i < 8; i++) {
        red1_pin = 0; //NO0-top-up
        green1_pin = 0;
        blue1_pin = pattern[16+j + 4 + 8][i];

        red2_pin = 0; //NO0-btm-up
        green2_pin = 0;
        blue2_pin = pattern[16+j + 4][i];

        clock_pin = 0;
        clock_pin = 1;
    }

    //
    for (i = 0; i < 8; i++) {
        //NO1
        red1_pin = 0; //NO1-top-down
        green1_pin = 0;
        blue1_pin = pattern[16+j + 8][i + 8];

        red2_pin = 0; //NO1-btm-down
        green2_pin = 0;
        blue2_pin = pattern[16+j][i + 8]; //ok

        clock_pin = 0;
        clock_pin = 1;
    }
    for (i = 0; i < 8; i++) {

        red1_pin = 0; //NO1-top-up
        green1_pin = 0;
        blue1_pin = pattern[16+j + 4 + 8][i + 8];

        red2_pin = 0; //NO1-btm-up
        green2_pin = 0;
        blue2_pin = pattern[16+j + 4 ][i + 8];

        clock_pin = 0;
        clock_pin = 1;
    }


    /////
    for (i = 0; i < 8; i++) {
        //NO2
        red1_pin = 0; //NO2-top-down
        green1_pin = 0;
        blue1_pin = pattern[16+j + 8][i + 16];

        red2_pin = 0; //NO2-btm-down
        green2_pin = 0;
        blue2_pin = pattern[16+j][i + 16]; //ok
        
        clock_pin = 0;
        clock_pin = 1;
    }
    for (i = 0; i < 8; i++) {
        red1_pin = 0; //NO2-top-up
        green1_pin = 0;
        blue1_pin = pattern[16+j + 4 + 8][i + 16];

        red2_pin = 0; //NO2-btm-up
        green2_pin = 0;
        blue2_pin = pattern[16+j + 4 ][i + 16];

        clock_pin = 0;
        clock_pin = 1;
    }

    for (i = 0; i < 8; i++) {
        //NO3 bottom-down
        red1_pin = 0; //NO3-top-down
        green1_pin = 0;
        blue1_pin = pattern[16+j + 8][i + 24];
        
        red2_pin = 0; //NO3-btm-down
        green2_pin = 0;
        blue2_pin = pattern[16+j][i + 24]; //ok

        clock_pin = 0;
        clock_pin = 1;
    }
    for (i = 0; i < 8; i++) {
        //NO3 top-up
        red1_pin = 0; //NO3-top-up
        green1_pin = 0;
        blue1_pin = pattern[16+j + 4 + 8][i + 24];

        red2_pin = 0; //NO3-btm-up
        green2_pin = 0;
        blue2_pin = pattern[16+j + 4][i + 24];

        clock_pin = 0;
        clock_pin = 1;
    }


        oe_pin = 1; //PANEL OFF
        set_abc(d);
        latch_pin = 1; //LATCH OFF
        oe_pin = 0; //PANEL ON
        latch_pin = 0; //LATCH ON

}

void DelayMs(WORD delay) {
    unsigned int int_status;
    while (delay--) {
        int_status = INTDisableInterrupts();
        OpenCoreTimer(GetSystemClock() / 2000);
        INTRestoreInterrupts(int_status);
        mCTClearIntFlag();
        while (!mCTGetIntFlag());
    }
    mCTClearIntFlag();
}


int count;
int toggle;

int main(void) {
    int i;
    int j;

    SYSTEMConfigPerformance(SYSCLK);
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

    ANSELA = 0x0000; // all digital pins
    ANSELB = 0x0000;

    TRISB = 0;
    TRISA = 0;

    LATBbits.LATB15 = 1;
    DelayMs(100);
    LATBbits.LATB15 = 0;
    DelayMs(100);
    LATBbits.LATB15 = 1;
    DelayMs(100);
    LATBbits.LATB15 = 0;
    DelayMs(100);
    LATBbits.LATB15 = 1;

    while (1) {

        count++;
        if (count % 100 == 0) {

            if (toggle == 1) {
                toggle = 0;
            } else {
                toggle = 1;
            }

            LATBbits.LATB15 = toggle;
        }

        j = count % 4;
        dataset(j);

//        DelayMs(1);

    }
}


