// DEVCFG3:
#pragma config IOL1WAY  = OFF           // Peripheral Pin Select Configuration
// DEVCFG2:
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_16         // PLL Multiplier (24x Multiplier)
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
#pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
#pragma config JTAGEN   = OFF           // JTAG Enable

#define SYSCLK 64000000L
#define FCY          SYSCLK

#include <p32xxxx.h>
#include <plib.h>
#include "animationA.h"
#include "animationB.h"
#include "animationC.h"
#include "animationD.h"
#include "animationE.h"
#include "animationF.h"
#include "animationG.h"
#include "animationH.h"
#include "animationI.h"
#include "animationJ.h"
#include "animationK.h"
#include "animationL.h"
#include "animationM.h"
#include "animationN.h"

#include "pattern.h"

#define GetSystemClock()       (SYSCLK)

#define red1_pin    LATBbits.LATB15
#define blu1_pin    LATAbits.LATA0
#define red2_pin    LATAbits.LATA1
#define blu2_pin    LATAbits.LATA3
#define a_pin       LATBbits.LATB4
#define c_pin       LATAbits.LATA4
#define clock_pin   LATBbits.LATB10
#define oe_pin      LATBbits.LATB9

#define grn1_pin    LATBbits.LATB14
#define grn2_pin    LATBbits.LATB13
#define b_pin       LATBbits.LATB11
#define latch_pin   LATBbits.LATB8


unsigned char r[256];
unsigned char g[256];
unsigned char b[256];

#define TRIS_TX1                TRISBbits.TRISB3
#define TRIS_RX1                TRISBbits.TRISB2
extern void resetAnimation();


int count;
int pCount;
int aCnt;
int frameCount;
int firstReset;

void set_abc(int v) {
    switch (v) {
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
    }
}

void dataset(int d) {
    int i, j;
    j = 3 - d; //3,2,1,0
    //    -------------------------------------------------------
    if (j >= 2) {
        for (i = 0; i < 64; i++) {
            red1_pin = r[i];
            grn1_pin = g[i];
            blu1_pin = b[i];
            red2_pin = r[128 + i];
            grn2_pin = g[128 + i];
            blu2_pin = b[128 + i];

            clock_pin = 0;
            clock_pin = 1;
            clock_pin = 0;
            clock_pin = 1;
        }
    } else {
        for (i = 0; i < 64; i++) {
            red1_pin = r[64 + i];
            grn1_pin = g[64 + i];
            blu1_pin = b[64 + i];
            red2_pin = r[192 + i];
            grn2_pin = g[192 + i];
            blu2_pin = b[192 + i];

            clock_pin = 0;
            clock_pin = 1;
            clock_pin = 0;
            clock_pin = 1;
        }
    }
    oe_pin = 1; //PANEL OFF
    set_abc(d);
    latch_pin = 1; //LATCH OFF
    oe_pin = 0; //PANEL ON
    latch_pin = 0; //LATCH ON

}

void InitUart1() {

    TRIS_TX1 = 0;
    TRIS_RX1 = 1;

    // Create a UART TX/RX Pin
    SYSKEY = 0xAA996655; // Write Key1 to SYSKEY
    SYSKEY = 0x556699AA; // Write Key2 to SYSKEY

    U1RXRbits.U1RXR = 0b0100; //U1RX -- RB2
    RPB3Rbits.RPB3R = 0b0001; // U1TX -- RB3

    SYSKEY = 0; // Locks the pin Configurations

    U1MODE = 0;
    U1STA = 0;

    U1MODEbits.BRGH = 0; //16xbaud
    U1BRG = (FCY / (16 * 9600)) - 1;

    U1MODEbits.RTSMD = 1; //No flow control

    U1MODEbits.PDSEL = 0; //8bit noParuty
    U1MODEbits.STSEL = 0; //Stpbit 1

    U1STAbits.UTXEN = 0; //Tx disnable
    U1STAbits.URXEN = 1; //Rx enable

    U1MODEbits.UEN = 0; //NO CTS  & NO RTS
    U1MODEbits.ON = 1;

    IFS1bits.U1RXIF = 0;
    IEC1bits.U1RXIE = 1;

    IPC8bits.U1IP = 1; //Interrupts Priority

}


unsigned int dataPos;
unsigned char myData[10];
unsigned char lastData;

void __ISR(_UART_1_VECTOR, IPL1) U1RXHandler(void) {
    unsigned char RcvData;

    IFS1bits.U1RXIF = 0;

    RcvData = getcUART1();

    if (RcvData == 255) {
        dataPos = 0;
    } else {
        if (RcvData == 'X' || RcvData == 'G' || RcvData == 'O' || RcvData == 'P'

                || RcvData == 'U' || RcvData == 'D'
                || RcvData == 'L' || RcvData == 'R'

                || RcvData == 'g' //STK-L LEFT
                || RcvData == 'h' //STK-L RIGHT
                || RcvData == 'i' //STK-L UP
                || RcvData == 'j'

                || RcvData == 'k' //STK-R LEFT
                || RcvData == 'l' //STK-R RIGHT
                || RcvData == 'm' //STK-R UP
                || RcvData == 'n' //STK-R DOWN
                ) {
            myData[dataPos] = RcvData;
            if (RcvData == 'k' || RcvData == 'D') {

                firstReset = 0;
            } else {
                if (myData[0] != lastData) {
                    resetAnimation();
                }
            }
            dataPos++;
            if (dataPos >= 10)dataPos = 0;
        }
    }
    lastData = myData[0];
}

void setPicture(const unsigned char * picture) {
    int i;
    for (i = 0; i < 256; i++) {
        r[i] = (((picture[i] >> 4) & 0b11) > count);
        g[i] = (((picture[i] >> 2) & 0b11) > count);
        b[i] = (((picture[i]) & 0b11) > count);
    }
}


//void setAnimation(const unsigned char *animation, const unsigned char * frame) {
//    int i;
//    
//    if (firstReset == 0) {
//        firstReset = 1;
//        frameCount = 0;
//        aCnt = 0;
//    } else {
//        if (frameCount % 200 == 0) {
//            aCnt++;
//            if (aCnt >= sizeof (frame) / sizeof (unsigned char)) {
//                aCnt = 0;
//            }
//        }
//    }
//
//    setPicture(&animation[frame[aCnt]]);
//
//}

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

    InitUart1();

    resetAnimation();

    while (1) {

        count++;
        if (count > 4) {
            count = 0;
        }

        frameCount++;

        switch (myData[0]) {
            default:
                myData[0] = 0;
                resetAnimation();
                break;

                //UP
            case 'U':
                //SHOUT  //2016-02-17
                if (firstReset == 0) {
                    firstReset = 1;
                    frameCount = 0;
                    aCnt = 0;
                } else {
                    if (frameCount % 200 == 0) {
                        aCnt++;
                        if (aCnt >= sizeof (frameK_1) / sizeof (unsigned char)) {
                            aCnt = 0;
                        }
                    }
                }
                setPicture(animationK[frameK_1[aCnt]]);
                
                break;

                //DOWN
            case 'D':
                setPicture(GANBARE_EX);
                break;


                //LEFT
                //NORMAL BLINK
            case 'L':
                if (firstReset == 0) {
                    firstReset = 1;

                    frameCount = 0;
                    aCnt = 0;
                } else {
                    if (frameCount % 1200 == 0) {
                        aCnt++;
                        if (aCnt >= sizeof (frameD_1) / sizeof (unsigned char)) {
                            aCnt = 0;
                        }
                    }
                }
                
                setPicture(animationD[frameD_1[aCnt]]);
                break;

                //RIGHT 
                //BROKEN BLINK
            case 'R':
                if (firstReset == 0) {
                    firstReset = 1;

                    frameCount = 0;
                    aCnt = 0;
                } else {
                    if (frameCount % 200 == 0) {
                        aCnt++;
                        if (aCnt >= sizeof (frameB_1) / sizeof (unsigned char)) {
                            aCnt = 0;
                        }
                    }
                }
                
                setPicture(animationB[frameB_1[aCnt]]);
                break;


                //BATSU
                //DELETE
            case 'X':
                for (i = 0; i < 256; i++) {
                    r[i] = 0;
                    g[i] = 0;
                    b[i] = 0;
                }
                break;

                //SANKAKU
                //BATSU
            case 'G':
                setPicture(batsu);
                break;

                //MARU
                //HATENA?
            case 'O':
                setPicture(hatena);
                break;

                //SIKAKU
                //BREAK HEART
            case 'P':
                setPicture(break_heart);
                break;

                //STK-L LEFT
                //WAVE
            case 'g':
                setPicture(GANBARE_G);
                
                break;
                //STK-L RIGHT
                //ENERGY
            case 'h':
                setPicture(GANBARE_A);

                break;

                //STK-L UP
                //START UP
            case 'i':
                if (firstReset == 0) {
                    firstReset = 1;

                    frameCount = 0;
                    aCnt = 0;
                } else {
                    if (frameCount % 250 == 0) {
                        aCnt++;
                        if (aCnt >= sizeof (frameF_1) / sizeof (unsigned char)) {
                            aCnt = sizeof (frameF_1) / sizeof (unsigned char) - 1;
                            myData[0] = 0;
                        }
                    }
                }
                setPicture(animationF[frameF_1[aCnt]]);
                break;

                //STK-L DOWN
            case 'j':
                setPicture(GANBARE_N);
                break;

                //STK-R LEFT
            case 'k':
                setPicture(GANBARE_B);
                break;

                //STK-R RIGHT
            case 'l':
                setPicture(GANBARE_A);
                break;

                //STK-R UP
            case 'm':
                setPicture(GANBARE_R);
                break;

                //STK-R DOWN
            case 'n':
                setPicture(GANBARE_E);
                break;
        }

        pCount++;
        dataset(pCount % 4);
    }

}

void resetAnimation() {
    int i;
    for (i = 0; i < 256; i++) {
        r[i] = 0;
        g[i] = 0;
        b[i] = 0;
    }
    firstReset = 0;
}
