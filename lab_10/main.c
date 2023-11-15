#include <stdio.h>
#include <stdlib.h>
#include "Timer.h"
#include "servo.h"
#include "lcd.h"
#include "button.h"
#include <inc/tm4c123gh6pm.h>


/**
 * main.c
 */
void main(void)
{
    timer_init();
    lcd_init();
    button_init();
    init_button_interrupts();
    servo_init();

    extern volatile int button_event;
    extern volatile int button_num;
    extern volatile int clockwise;
	//return 0;

    servo_move(90);
    while(1){
        timer_waitMillis(100);
        lcd_printf("%d \n%d", abs(180 - button_Handler(button_num)), clockwise);

    }
}

