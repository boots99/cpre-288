

/**
 * main.c
 */
#include "open_interface.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "lcd.h"
#include "cyBot_Scan.h"
int main(void)
{
     lcd_init();
     oi_t *sensor_data = oi_alloc();
     oi_init(sensor_data);
     cyBOT_init_Scan(0b0111);
         right_calibration_value = 190750;
         left_calibration_value = 1267000;






//
     cyBot_uart_init();

     char my_data;       // Variable to get bytes from Client
       char command[100];  // Buffer to store command from Client
       int index = 0;      // Index position within the command buffer
       int i= 0;
       int deg = 0;
       cyBOT_Scan_t scan;
       char pingScan[5];
       char degreeNum[5];

       // Write to LCD so that we know the program is running
       lcd_printf("Running");

       while(1)
       {

         index = 0;  // Set index to the beginning of the command buffer
         my_data = cyBot_getByte(); // Get first byte of the command from the Client

         // Get the rest of the command until a newline byte (i.e., '\n') received
         while(my_data != '\n' )
         {
           command[index] = my_data;  // Place byte into the command buffer
           index++;
           my_data = cyBot_getByte(); // Get the next byte of the command
         }

         command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
         command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

         lcd_printf("Got: %s", command);  // Print received command to the LCD screen

         // Send a response to the Client (Starter Client expects the response to end with \n)
         // In this case I am just sending back the first byte of the command received and a '\n'
         //cyBot_sendByte(command[0]);

             if(command[0] == 'w'){
                 lcd_printf("Moving Forward");
                 move_forward(sensor_data, 50);
             }
             if(command[0] == 'a'){
                 lcd_printf("Moving left");
                 turn_cclockwise(sensor_data, 250);

             }
             if(command[0] == 's'){
                 lcd_printf("Moving backwards");
                 move_backwards(sensor_data, 50);

             }
             if(command[0] == 'd'){
                 lcd_printf("Moving right");
                 turn_clockwise(sensor_data, 250);

             }
             if(command[0] == 'm'){
                 cyBOT_Scan(0, &scan);
                 timer_waitMillis(1000);
                 for(i = 0; i < 5; i ++){
                     degreeNum[i] = ' ';
                 }
                 for(deg = 0; deg <= 180; deg += 4){
                     cyBOT_Scan(deg, &scan);

                     sprintf(pingScan, "%f", scan.sound_dist/100);

                     sprintf(degreeNum, "%d", deg);
                     for(i = 0; i < 5; i ++){
                         if(degreeNum[i] != '\0'){
                             cyBot_sendByte(degreeNum[i]);
                         }
                     }
                     for(i = 0; i < 5; i ++){
                         cyBot_sendByte(' ');
                     }
                     for(i = 0; i < 5; i ++){
                         cyBot_sendByte(pingScan[i]);
                     }
                     cyBot_sendByte('\n');
//                     cyBot_sendByte('\r');

                 }
             }

         // Only send a '\n' if the first byte of the command is not a '\n',
         // to avoid sending back-to-back '\n' to the client
         if(command[0] != '\n')
         {
           cyBot_sendByte('\n');
         }

       }
//     while(1){
//         char data = cyBot_getByte();
//         lcd_printf("%c", (data));
//         if(data == 'w'){
//             lcd_printf("Moving Forward");
//             oi_setWheels(250, 250);
//         }
//         if(data == 'a'){
//             lcd_printf("Moving left");
//             oi_setWheels(250, -250);
//
//         }
//         if(data == 's'){
//             lcd_printf("Moving backwards");
//             oi_setWheels(-250, -250);
//
//         }
//         if(data == 'd'){
//             lcd_printf("Moving right");
//             oi_setWheels(-250, 250);
//
//         }
//     }
     //Drive in a square
//     move_forward(sensor_data, 2000);
//     turn_clockwise(sensor_data, 250);
//     move_backwards(sensor_data, 500);
//     turn_cclockwise(sensor_data, 250);
//     move_forward(sensor_data, 500);
//     turn_clockwise(sensor_data, 250);
//     move_forward(sensor_data, 500);
//     turn_clockwise(sensor_data, 250);
//     move_forward(sensor_data, 500);
//     turn_clockwise(sensor_data, 250);


     oi_free(sensor_data);
	return 0;
}

//
//
//#include "Timer.h"
//#include "lcd.h"
//#include "cyBot_uart.h"
//#include "open_interface.h"
//#include "movement.h"
//
//int main (void) {
//
//  timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called
//                  // and enables time functions (e.g. timer_waitMillis)
//  lcd_init();   // Initialize the the LCD screen.  This also clears the screen.
//  cyBot_uart_init();  // Initialize UART
//  oi_t *sensor_data = oi_alloc();
//  oi_init(sensor_data);
//
//
//  char my_data;       // Variable to get bytes from Client
//  char command[100];  // Buffer to store command from Client
//  int index = 0;      // Index position within the command buffer
//
//  // Write to LCD so that we know the program is running
//  lcd_printf("Running");
//
//  while(1)
//  {
//
//    index = 0;  // Set index to the beginning of the command buffer
//    my_data = cyBot_getByte(); // Get first byte of the command from the Client
//
//    // Get the rest of the command until a newline byte (i.e., '\n') received
//    while(my_data != '\n' )
//    {
//      command[index] = my_data;  // Place byte into the command buffer
//      index++;
//      my_data = cyBot_getByte(); // Get the next byte of the command
//    }
//
//    command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
//    command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing
//
//    lcd_printf("Got: %s", command);  // Print received command to the LCD screen
//
//    // Send a response to the Client (Starter Client expects the response to end with \n)
//    // In this case I am just sending back the first byte of the command received and a '\n'
//    cyBot_sendByte(command[0]);
//
//    //    if(command[0] == 'w'){
//    //        lcd_printf("Moving Forward");
//    //        move_forward(sensor_data, 2000);
//    //    }
//    //    if(command[0] == 'a'){
//    //        lcd_printf("Moving left");
//    //        turn_cclockwise(sensor_data, 250);
//    //
//    //    }
//    //    if(command[0] == 's'){
//    //        lcd_printf("Moving backwards");
//    //        move_backwards(sensor_data, 2000);
//    //
//    //    }
//    //    if(command[0] == 'd'){
//    //        lcd_printf("Moving right");
//    //        turn_clockwise(sensor_data, 250);
//    //
//    //    }
//
//    // Only send a '\n' if the first byte of the command is not a '\n',
//    // to avoid sending back-to-back '\n' to the client
//    if(command[0] != '\n')
//    {
//      cyBot_sendByte('\n');
//    }
//
//  }
//
//  return 0;
//}
