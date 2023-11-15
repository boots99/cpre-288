/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Describtion: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"



int main (void) {

    char message[50] = "Microcontrollers are lots of fun!";
    //const char *fmt = "Message 288";
	timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the the LCD screen.  This also clears the screen. 

	//timer_waitMillis(300);

	// Clear the LCD screen and print "Hello, world" on the LCD
	//lcd_puts("Hello, world");
	int i = 20;
	int count = 0;
	char dummy[50] = "                    ";
	int count2 = 0;
	int l;
	while(1){
        //dummy = "Microcontrollers are lots of fun!";
        timer_waitMillis(300);
        int j = i;
        count = count2;
        while(dummy[j] != '\0'){
        dummy[j] = message[count];
        j++;
        count++;
        }

        lcd_printf("%s", dummy);
        //printf("%s", dummy);


        if (i == 0){
        count++;
        i = 1;
        count2++;
        }
        i--;
        if(count2 == strlen(message)){
            count2 = 0;
            for (l = 0; l < 20; l ++){
                dummy[l] = ' ';
            }

            i = 20;
        }
	}



	// lcd_puts("Hello, world");// Replace lcd_printf with lcd_puts
                                 // step through in debug mode
                                 // and explain to TA how it
                                 // works


    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
    // NOTE: For time fuctions see Timer.h

	return 0;
}
