/**
 * lab4_template.c
 * 
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
	button_init();
	lcd_init();
	cyBot_uart_init();
	            // Don't forget to initialze the cyBot UART before trying to use it
	
	// YOUR CODE HERE
	int buttonPressed = 0;
	cyBot_getByte();
	cyBot_sendByte('D');
	while(1)
	{
	
      // YOUR CODE HERE
  

        buttonPressed = button_getButton();
        switch(buttonPressed){
        case 1:
            lcd_puts("button 4 pressed");
            cyBot_sendByte('4');
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');
            break;
        case 2:
            lcd_puts("button 3 pressed");
            cyBot_sendByte('3');
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');
            break;
        case 3:
            lcd_puts("button 2 pressed");
            cyBot_sendByte('2');
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');
            break;
        case 4:
            lcd_puts("button 1 pressed");
            cyBot_sendByte('1');
            cyBot_sendByte('\n');
            cyBot_sendByte('\r');
            break;
        case 0:
            lcd_puts("no button pressed");
            break;
        }

        lcd_clear();
	
	}
	
}
