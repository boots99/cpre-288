#include <stdio.h>
#include <stdlib.h>
#include <inc/tm4c123gh6pm.h>
#include "servo.h"
#include "Timer.h"
#include "button.h"


volatile int degree = 0;
volatile int clockwise;

void servo_initc(void){

//    SYSCTL_RCGCGPIO_R |= 0x02;
//    SYSCTL_RCGCTIMER_R |= 0x02;
//
//    while ((SYSCTL_PRGPIO_R & 0x02) != 0x02) {};
//
//    GPIO_PORTB_DIR_R &= ~0x20;
//    GPIO_PORTB_AFSEL_R |= 0x20;
//    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFF7FFFFF) | 0x00700000;
//    GPIO_PORTB_DEN_R |= 0x20;
//
//    TIMER1_CTL_R &= ~0x100;
//    TIMER1_CFG_R = 0x4;
//    TIMER1_TBMR_R = 0x0A;
//    TIMER1_TBPR_R = 0x4;
//    TIMER1_TBILR_R = 0xE200;
//    TIMER1_CTL_R |= 0x100;
//
//
//    //TIMER1_TBMATCHR_R = 160000;
//    //TIMER1_TBPMR_R = 160000 >> 16;

    SYSCTL_RCGCGPIO_R   |= 0b000010;    //Enable GPIO PB Clk
        SYSCTL_RCGCTIMER_R  |= 0b000010;    //Enable Timer 1 Clk

        while((SYSCTL_PRGPIO_R  & 0b0010) == 0){};   //Waiting for the reset to go through
        while((SYSCTL_PRTIMER_R & 0b0010) == 0){};

        GPIO_PORTB_DEN_R    |= 0b00100000;      //Enable Digital Function
        GPIO_PORTB_DIR_R    |= 0b00100000;      //Set PB5 as output
        GPIO_PORTB_AFSEL_R  |= 0b00100000;      //Enable Alternate Function for PB5
        GPIO_PORTB_PCTL_R   &= 0xFF0FFFFF;      //Clear PCTL for PB5
        GPIO_PORTB_PCTL_R   |= 0x00700000;      //Alternate Function set to T1CCP1

        TIMER1_CTL_R        &= 0b011111111;     //Disable Timer1 B (TBEN)
        TIMER1_CFG_R        = 0x00000004;       //16 bit timer config
        TIMER1_TBMR_R       &= 0xFFFFFFF0;      //Clears bits 3:0
        TIMER1_TBMR_R       |= 0b1010;          //PWM Mode, Edge-Count Mode, Periodic Timer Mode
        TIMER1_CTL_R        &= 0xFFFFBFFF;      //Output is not inverted
        //TIMER1_CTL_R        |= 0x4000;          //Output is inverted
        TIMER1_TBPR_R       = 0x04;             //Extension of timer (0b100)
        TIMER1_TBILR_R      = 0b1110001000000000; //0xE200;           //Set first 16 bits of timer period (1110001000000000)
        TIMER1_TBMATCHR_R   = 0x5355;           //283,477 = 0xA380;            //0b1001010001110000000 = 320000 - 16000;   //Initialize servo at 0 degrees
        TIMER1_TBPMR_R      = 0x4;
        TIMER1_CTL_R        |= 0b100000000;     //Re-Enable Timer1 B
}

void servo_movec(float degrees){
    int counts = 151.59 * abs(180 - degrees) + 284366;
    TIMER1_TBMATCHR_R = counts;
    TIMER1_TBPMR_R = counts >> 16;

}

int button_Handler(int ButtonInput){

    switch(ButtonInput){
        case 4:

                servo_movec(degree + 1);
                if(clockwise == 1){
                    degree = degree + 1;
                }
                else{
                    degree = degree - 1;
                }

            break;
        case 3:

                servo_move(degree + 5);
                if(clockwise == 1){
                    degree = degree + 5;
                }
                else{
                    degree = degree - 5;
                }

            break;
        case 2:
            if(clockwise == 1){
                clockwise = 0;
                TIMER1_TBMR_R = 0x1A;
                break;
            }
            clockwise = 1;
            TIMER1_TBMR_R = 0x0A;
            break;
        case 1:

            if(clockwise == 0){
                degree = 180;
                servo_move(180);
            }
            else{
                degree = 0;
                servo_move(0);
            }
            break;
    }
    if(degree > 180){
        degree = 180;
    }
    if (degree < 0){
        degree = 0;
    }
    return degree;

}
