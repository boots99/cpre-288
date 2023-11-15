/**
 * lab5_template.c
 * 
 * Template file for CprE 288 lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include <math.h>
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#include "cyBot_Scan.h"  // For scan sensors 


#warning "Possible unimplemented functions"
#define REPLACEME 0


// Defined in button.c : Used to communicate information between the
// the interupt handeler and main.
extern volatile int button_event;
extern volatile int button_num;


int main(void) {
	button_init();
	lcd_init();
	init_button_interrupts();
    timer_init();
    cyBOT_init_Scan(0b0111);
    cyBOT_Scan_t scan;


	            // Don't forget to initialze the cyBot UART before trying to use it
	 cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

      SYSCTL_RCGCGPIO_R |= 0x02;
      timer_waitMillis(1);            // Small delay before accessing device after turning on clock
      GPIO_PORTB_AFSEL_R |= 0b00000011;
      GPIO_PORTB_PCTL_R &= 0x00000011;     // Force 0's in the disired locations
      GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the disired locations
      GPIO_PORTB_DEN_R |= 0b00000011;
      GPIO_PORTB_DIR_R &= 0b11111111;;      // Force 0's in the disired locations
      GPIO_PORTB_DIR_R |= 0b00000011;      // Force 1's in the disired locataions
    
     cyBot_uart_init_last_half();  // Complete the UART device initialization part of configuration
	
	// Initialze scan sensors
    // cyBOT_init_Scan();p


    int previousScan = 1;
	// YOUR CODE HERE
    char scanval[5];
    int i = 0;
    char pingScan[5];
    int previousPingScan = 0;
    char calculatedDistance[5];
	while(1)
	{
	    timer_waitMillis(300);
	    cyBOT_Scan(0, &scan);
	    lcd_clear();


	    sprintf(scanval, "%d", (scan.IR_raw_val+previousScan)/2);
	    lcd_puts(scanval);

	    sprintf(pingScan, "%f", (scan.sound_dist+previousPingScan)/2);

        for(i = 0; i< 5; i++){
            cyBot_sendByte(scanval[i]);
        }
        cyBot_sendByte(',');
        cyBot_sendByte(' ');
        for(i = 0; i< 5; i++){
            cyBot_sendByte(pingScan[i]);
        }

        float Stock = pow( ((scan.IR_raw_val+previousScan)/2.0), -1.626);
        float Ham = 4000000 * Stock;
        sprintf(calculatedDistance, "%f", Ham);
        cyBot_sendByte(',');
        cyBot_sendByte(' ');
        for(i = 0; i< 5; i++){
            cyBot_sendByte(calculatedDistance[i]);

        }

	    previousScan = scan.IR_raw_val;
	    previousPingScan = scan.sound_dist;


	    cyBot_sendByte('\n');
	    cyBot_sendByte('\r');

	                 switch(button_num){
	                 case 1:
	                     cyBot_getByte_blocking();
	                             break;
	                         case 2:
	                             cyBot_getByte_blocking();
	                             break;
	                         case 3:
	                             cyBot_getByte_blocking();
	                             break;
	                         case 4:
	                             cyBot_getByte_blocking();
	                             break;
	                         case 0:

	                             break;
	                         }


//	    lcd_puts("hello");
//	           switch(button_num){
//	           case 1:
//	                       lcd_puts("button 4 pressed");
//	                       cyBot_sendByte('4');
//	                       cyBot_sendByte('\n');
//	                       cyBot_sendByte('\r');
//	                       break;
//	                   case 2:
//	                       lcd_puts("button 3 pressed");
//	                       cyBot_sendByte('3');
//	                       cyBot_sendByte('\n');
//	                       cyBot_sendByte('\r');
//	                       break;
//	                   case 3:
//	                       lcd_puts("button 2 pressed");
//	                       cyBot_sendByte('2');
//	                       cyBot_sendByte('\n');
//	                       cyBot_sendByte('\r');
//	                       break;
//	                   case 4:
//	                       lcd_puts("button 1 pressed");
//	                       cyBot_sendByte('1');
//	                       cyBot_sendByte('\n');
//	                       cyBot_sendByte('\r');
//	                       break;
//	                   case 0:
//	                       lcd_puts("no button pressed");
//	                       break;
//	                   }

	
	}
	
}
