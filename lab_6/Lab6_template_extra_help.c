/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */



#include "timer.h"
#include "lcd.h"
#include "uart_extra_help.h"
#include <stdio.h>
#include "cyBot_Scan.h"


// Adding global volatile varibles for communcating between 
// your Interupt Service Routine, and your non-interupt code.

volatile  char uart_data;  // Your UART interupt code can place read data here
volatile  char flag;       // Your UART interupt can update this flag
                           // to indicate that it has placed new data
                           // in uart_data                     


void main()
{

    timer_init();
    lcd_init();


    // TODO
    uart_init();
    uart_interrupt_init();
    cyBOT_init_Scan(0b0111);
    right_calibration_value = 85750;
    left_calibration_value = 1487500 ;
    int deg;
    cyBOT_Scan_t scan;
    for(deg = 0; deg <= 180; deg += 1){
        int previousScan = 1;
            // YOUR CODE HERE
            char scanval[5];
            int i = 0;
            char pingScan[5];
            int previousPingScan = 0;
            char calculatedDistance[5];
                timer_waitMillis(300);
                cyBOT_Scan(deg, &scan);
                lcd_clear();


                sprintf(scanval, "%d", (scan.IR_raw_val+previousScan)/2);


                sprintf(pingScan, "%f", (scan.sound_dist+previousPingScan)/2);

                for(i = 0; i< 5; i++){
                    uart_sendChar(scanval[i]);
                }
                uart_sendChar(',');
                uart_sendChar(' ');
                for(i = 0; i< 5; i++){
                    uart_sendChar(pingScan[i]);
                }

                float Stock = pow( ((scan.IR_raw_val+previousScan)/2.0), -1.626);
                float Ham = 4000000 * Stock;
                sprintf(calculatedDistance, "%f", Ham);
                uart_sendChar(',');
                uart_sendChar(' ');
                for(i = 0; i< 5; i++){
                    uart_sendChar(calculatedDistance[i]);

                }

                previousScan = scan.IR_raw_val;
                previousPingScan = scan.sound_dist;


                uart_sendChar('\n');
                uart_sendChar('\r');
    }


    //part 3
//	char Input[20];
//	char counter[2];
//
//	int count = 0;
//	while(1){
//        while(count < 20){
//            while(flag == 0){}
//            if(flag == 1){
//                Input[count] = uart_data;
//                flag = 0;
//
//            }
//            uart_sendChar(uart_data);
//            lcd_clear();
//            if(uart_data == '\r'){
//                break;
//            }
//            count++;
//            sprintf(counter, "%d", count);
//            lcd_putc(counter[0]);
//            lcd_putc(counter[1]);
//
//        }
//        uart_sendChar('\n');
//        uart_sendChar('\r');
//        lcd_clear();
//        lcd_puts(Input);
//        for(count = 0; count < 20; count ++){
//            //uart_sendChar(Input[count]);
//            Input[count] = ' ';
//        }
//        count = 0;
//	}


	//part 4




 


}

