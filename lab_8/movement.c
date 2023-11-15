#include "open_interface.h"
#include "Timer.h"
//#include "cyBot_Scan.h"
#include <stdio.h>
#include <math.h>
#include "movement.h"
#include "uart_extra_help.h"
#include "adc.h"
#include "servo.h"
#include "ping.h"

//volatile  char uart_data;  // Your UART interupt code can place read data here
//volatile  char flag;       // Your UART interupt can update this flag

typedef struct autostruct
{
    int minAngle;
    float objDist;
}autostruct_t;

typedef struct{
    float sound_dist;  // Distance from Ping Sensor (cyBOT_Scan returns -1.0 if PING is not enabled)
    int IR_raw_val;    // Raw ADC value from IR sensor (cyBOT_Scan return -1 if IR is not enabled)
} cyBOT_Scan_t;


void move_forward(oi_t *sensor, int millimeters){

     double sum = 0;
     oi_setWheels(250, 250); // move forward; full speed

     while (sum < millimeters) {
     oi_update(sensor);
     sum += sensor->distance;
     if(sensor->bumpLeft){
         move_backwards(sensor, 150);
         sum -= 30;
         turn_clockwise(sensor, 250);
         move_forward(sensor, 250);
         turn_cclockwise(sensor, 250);
         oi_setWheels(250,250);
         }
     if(sensor->bumpRight){
         move_backwards(sensor, 150);
         sum -= 30;
         turn_cclockwise(sensor, 250);
         move_forward(sensor, 250);
         turn_clockwise(sensor, 250);
         oi_setWheels(250,250);
         }
     }
     oi_setWheels(0, 0); // stop
}
void move_backwards(oi_t *sensor, int millimeters){

     double sum = 0;
     oi_setWheels(-250, -250); // move back; full speed

     while (sum > -millimeters) {
     oi_update(sensor);
     sum += sensor->distance;
     }
     oi_setWheels(0, 0); // stop
}

void turn_clockwise(oi_t *sensor, int degrees){

    double angle = 0;
    oi_setWheels(-250, 250);
    while (angle > -degrees) {
    oi_update(sensor);
    angle += (sensor->angle / 0.324056);
    }
    oi_setWheels(0, 0); // stop
}
void turn_cclockwise(oi_t *sensor, int degrees){

    double angle = 0;
    oi_setWheels(250, -250);
    while (angle < degrees) {
    oi_update(sensor);
    angle += (sensor->angle / 0.324056);
    }
    oi_setWheels(0, 0); // stop
}

void sendString(char string[], int length)
{
    int i = 0;
    for(i = 0; i<length; i++)
    {
        uart_sendChar(string[i]);
    }
}


float toRadians(int degrees)
{
    return M_PI / 180.0 * degrees;
}

float est_distance(int IR_sensor_val)
{
    // y = 2E+07x^(-1.791) Bot 00
//    return 2*pow(10, 7)*pow(IR_sensor_val, -1.791);

    return 6*pow(10, 6)*pow(IR_sensor_val, -1.795); // Bot 02 y = 6E+06x-1.795
}





int main()
{
    timer_init();
    lcd_init();
    lcd_putc('a');

    oi_t *sensor_data = oi_alloc();
    lcd_putc('d');
    oi_init_noupdate();
    //oi_init(sensor_data);
    lcd_putc('c');
    uart_init();
    lcd_putc('b');

    adc_init();
    servo_initc();
    ping_init();

    servo_move(90);

    //cyBOT_init_Scan(0b0111);
//
//        right_calibration_value = 17500;
//        left_calibration_value = 1335250;

            lcd_putc('c');

            int previousScan = 1;

            int objectStartDegree;
            int objectEndDegree;
            char angle[4];
            char distance[4];
            char width[4];
            int shortestObjectIndex = 0;
            char linearWidth[4];
            typedef struct{
                int degree;
                int distance;
                int width;
                int linearWidth;
            }Object;
            Object objectList[4];
               int objectCount = 0;
               // YOUR CODE HERE
               char scanval[5];
               int i = 0;
               char pingScan[5];
               char degre[3];
               int previousPingScan = 0;
               char calculatedDistance[5];
               int deg;
               char readVal;
               int toggle = 0;
               cyBOT_Scan_t scan;
               cyBOT_Scan_t tempscan;
               uart_sendChar('h');
               uart_receive();
               while(1){
                   readVal = uart_receive();
                   if(readVal == 'h' && toggle == 0){

                       for(deg = 0; deg <= 180; deg += 2){
                           if((char)(UART1_DR_R & 0xFF) == 't'){
                               for( i = 0; i < 20; i ++){
                                   uart_sendChar('-');
                               }
                               uart_sendChar('\n');
                               uart_sendChar('\r');
                               toggle = 1;
                               break;
                           }
                           //cyBOT_Scan(deg, &scan);

                           servo_movec(deg);
                           timer_waitMillis(200);
                           scan.sound_dist = ping_getDistance();

                           lcd_clear();


                           sprintf(scanval, "%d", (adc_read()+previousScan)/2);


                           sprintf(pingScan, "%f", (scan.sound_dist+previousPingScan)/2);

                           sprintf(degre, "%d", deg);
                           for(i = 0; i< 3; i++){
                               uart_sendChar(degre[i]);
                           }
                           uart_sendChar(',');
                           uart_sendChar(' ');
                           for(i = 0; i< 5; i++){
                               uart_sendChar(scanval[i]);
                           }
                           uart_sendChar(',');
                           uart_sendChar(' ');
                           for(i = 0; i< 5; i++){
                               uart_sendChar(pingScan[i]);
                           }

                           float Stock = pow( ((adc_read()+previousScan)/2.0), -1.406);
                           float Ham = 688283 * Stock;

                           float calculatedIRVAL = 688283 * pow(adc_read(), -1.406);
                           float calculatedPREVIOUSVAL = 688283 * pow(previousScan, -1.406);

                           sprintf(calculatedDistance, "%f", calculatedIRVAL);
                           uart_sendChar(',');
                           uart_sendChar(' ');
                           for(i = 0; i< 5; i++){
                               uart_sendChar(calculatedDistance[i]);

                           }



                           if(calculatedIRVAL - calculatedPREVIOUSVAL < -10){
                                       //Start object
                                       objectStartDegree = deg;
                                       uart_sendChar('\n');
                                       uart_sendChar('\r');
                                       uart_sendChar('s');
                                       lcd_putc('s');
                                   }
                           if(calculatedPREVIOUSVAL - calculatedIRVAL < -10){
                               //end object
                               objectEndDegree = deg;

                               if(objectEndDegree-objectStartDegree > 3 ){
                              //     if(((objectEndDegree+objectStartDegree)/2) > (objectList[objectCount--].degree + 3)){
                                       //object is valid
                                       uart_sendChar('\n');
                                       uart_sendChar('\r');
                                       uart_sendChar('o');
                                       uart_sendChar('b');
                                       uart_sendChar('j');
                                       uart_sendChar('\n');
                                       uart_sendChar('\r');
                                       lcd_clear();
                                       //save object
                                       objectList[objectCount].degree = (objectEndDegree + objectStartDegree )/ 2;
                    //                   cyBOT_Scan(objectList[objectCount].degree, &tempscan);
                    //                   objectList[objectCount].distance = tempscan.sound_dist;
                                       objectList[objectCount].distance = scan.sound_dist;
                                       objectList[objectCount].linearWidth = abs(2*((sin((objectEndDegree - objectStartDegree)/2))*objectList[objectCount].distance));
                                       objectList[objectCount].width = objectEndDegree-objectStartDegree;

                                       objectCount++;
                             //      }
                               }

                           }

                           previousScan = adc_read();
                           previousPingScan = scan.sound_dist;


                           uart_sendChar('\n');
                           uart_sendChar('\r');
                       }

                       uart_sendChar('O');
                       uart_sendChar('b');
                       uart_sendChar('j');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar('A');
                       uart_sendChar('n');
                       uart_sendChar('g');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar('D');
                       uart_sendChar('i');
                       uart_sendChar('s');
                       uart_sendChar('t');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar('W');
                       uart_sendChar('i');
                       uart_sendChar('d');
                       uart_sendChar('t');
                       uart_sendChar('h');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar(' ');
                       uart_sendChar('L');
                       uart_sendChar('W');
                       uart_sendChar('i');
                       uart_sendChar('d');
                       uart_sendChar('t');
                       uart_sendChar('h');
                       uart_sendChar('\n');
                       uart_sendChar('\r');
                       for(i = 0; i < objectCount; i ++){
                           uart_sendChar(i + 48);
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           sprintf(angle, " %d", objectList[i].degree);
                           sprintf(distance, " %d", objectList[i].distance);
                           sprintf(width, " %d", objectList[i].width);
                           sprintf(linearWidth, " %d", objectList[i].linearWidth);
                           uart_sendChar(angle[0]);
                           uart_sendChar(angle[1]);
                           uart_sendChar(angle[2]);
                           uart_sendChar(angle[3]);
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(distance[0]);
                           uart_sendChar(distance[1]);
                           uart_sendChar(distance[2]);
                           uart_sendChar(distance[3]);
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(width[0]);
                           uart_sendChar(width[1]);
                           uart_sendChar(width[2]);
                           uart_sendChar(width[3]);
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(linearWidth[0]);
                           uart_sendChar(linearWidth[1]);
                           uart_sendChar(linearWidth[2]);
                           uart_sendChar(linearWidth[3]);
                           uart_sendChar('\n');
                           uart_sendChar('\r');

                       }


                       int shortestWidth = objectList[0].width;
                       int shortestObjectIndex = 0;
                       for(i = 0; i < objectCount; i ++){


                           if(objectList[i].width < shortestWidth){
                               shortestWidth = objectList[i].width;
                               shortestObjectIndex = i;
                           }
                       }

                       timer_waitMillis(500);
                       //cyBOT_Scan(objectList[shortestObjectIndex].degree, &tempscan);
                       servo_movec(objectList[shortestObjectIndex].degree);
                       tempscan.sound_dist = ping_getDistance();
                       objectList[shortestObjectIndex].distance = tempscan.sound_dist;
                       timer_waitMillis(200);

                       uart_sendChar('\n');
                       uart_sendChar('\r');
                       sprintf(angle, " %d", objectList[shortestObjectIndex].degree);
                       uart_sendChar(angle[0]);
                       uart_sendChar(angle[1]);
                       uart_sendChar(angle[2]);
                       uart_sendChar(angle[3]);

                       readVal = '0';
                      readVal = uart_receive();
                      if(readVal == 'h'){

                          if(objectList[shortestObjectIndex].degree > 90){
                              //turn counter clockwise
                                   turn_cclockwise(sensor_data, (objectList[shortestObjectIndex].degree - 90)*2.77777778);
                                   move_forward(sensor_data, objectList[shortestObjectIndex].distance*6);


                          }
                          else{
                              //turn clockwise
                              turn_clockwise(sensor_data, (90-objectList[shortestObjectIndex].degree)*2.77777778);
                              move_forward(sensor_data, objectList[shortestObjectIndex].distance*6);

                          }
                      }
                   }

                   if(toggle == 1){
                       readVal = uart_receive();
                       if(readVal == 'w'){
                           move_forward(sensor_data, 100);
                       }
                       else if(readVal == 's'){
                           move_backwards(sensor_data, 100);
                       }
                       else if(readVal == 'a'){
                           turn_cclockwise(sensor_data, 25);
                       }
                       else if(readVal == 'd'){
                           turn_clockwise(sensor_data, 25);
                       }
                       else if(readVal == 'm'){
                           for(deg = 0; deg <= 180; deg += 2){
                               if((char)(UART1_DR_R & 0xFF) == 't'){
                                   for( i = 0; i < 20; i ++){
                                       uart_sendChar('-');
                                   }
                                   uart_sendChar('\n');
                                   uart_sendChar('\r');
                                   toggle = 1;
                                   break;
                               }
                               //cyBOT_Scan(deg, &scan);
                               servo_movec(deg);
                               scan.sound_dist = ping_getDistance();
                               lcd_clear();


                               sprintf(scanval, "%d", (adc_read()+previousScan)/2);


                               sprintf(pingScan, "%f", (scan.sound_dist+previousPingScan)/2);

                               sprintf(degre, "%d", deg);
                               for(i = 0; i< 3; i++){
                                   uart_sendChar(degre[i]);
                               }
                               uart_sendChar(',');
                               uart_sendChar(' ');
                               for(i = 0; i< 5; i++){
                                   uart_sendChar(scanval[i]);
                               }
                               uart_sendChar(',');
                               uart_sendChar(' ');
                               for(i = 0; i< 5; i++){
                                   uart_sendChar(pingScan[i]);
                               }

                               float Stock = pow( ((adc_read()+previousScan)/2.0), -1.626);
                               float Ham = 4000000 * Stock;

                               float calculatedIRVAL = 10000000 * pow(adc_read(), -1.867);
                               float calculatedPREVIOUSVAL = 10000000 * pow(previousScan, -1.867);

                               sprintf(calculatedDistance, "%f", calculatedIRVAL);
                               uart_sendChar(',');
                               uart_sendChar(' ');
                               for(i = 0; i< 5; i++){
                                   uart_sendChar(calculatedDistance[i]);

                               }



                               if(calculatedIRVAL - calculatedPREVIOUSVAL < -5){
                                           //Start object
                                           objectStartDegree = deg;
                                           uart_sendChar('\n');
                                           uart_sendChar('\r');
                                           uart_sendChar('s');
                                           lcd_putc('s');
                                       }
                               if(calculatedPREVIOUSVAL - calculatedIRVAL < -5){
                                   //end object
                                   objectEndDegree = deg;

                                   if(objectEndDegree-objectStartDegree > 3 ){
                                  //     if(((objectEndDegree+objectStartDegree)/2) > (objectList[objectCount--].degree + 3)){
                                           //object is valid
                                           uart_sendChar('\n');
                                           uart_sendChar('\r');
                                           uart_sendChar('o');
                                           uart_sendChar('b');
                                           uart_sendChar('j');
                                           uart_sendChar('\n');
                                           uart_sendChar('\r');
                                           lcd_clear();
                                           //save object
                                           objectList[objectCount].degree = (objectEndDegree + objectStartDegree )/ 2;
                        //                   cyBOT_Scan(objectList[objectCount].degree, &tempscan);
                        //                   objectList[objectCount].distance = tempscan.sound_dist;
                                           objectList[objectCount].distance = scan.sound_dist;
                                           objectList[objectCount].linearWidth = abs(2*((sin((objectEndDegree - objectStartDegree)/2))*objectList[objectCount].distance));
                                           objectList[objectCount].width = objectEndDegree-objectStartDegree;

                                           objectCount++;
                                 //      }
                                   }

                               }

                               previousScan = adc_read();
                               previousPingScan = scan.sound_dist;


                               uart_sendChar('\n');
                               uart_sendChar('\r');
                           }

                           uart_sendChar('O');
                           uart_sendChar('b');
                           uart_sendChar('j');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar('A');
                           uart_sendChar('n');
                           uart_sendChar('g');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar('D');
                           uart_sendChar('i');
                           uart_sendChar('s');
                           uart_sendChar('t');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar('W');
                           uart_sendChar('i');
                           uart_sendChar('d');
                           uart_sendChar('t');
                           uart_sendChar('h');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar(' ');
                           uart_sendChar('L');
                           uart_sendChar('W');
                           uart_sendChar('i');
                           uart_sendChar('d');
                           uart_sendChar('t');
                           uart_sendChar('h');
                           uart_sendChar('\n');
                           uart_sendChar('\r');
                           for(i = 0; i < objectCount; i ++){
                               uart_sendChar(i + 48);
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               sprintf(angle, " %d", objectList[i].degree);
                               sprintf(distance, " %d", objectList[i].distance);
                               sprintf(width, " %d", objectList[i].width);
                               sprintf(linearWidth, " %d", objectList[i].linearWidth);
                               uart_sendChar(angle[0]);
                               uart_sendChar(angle[1]);
                               uart_sendChar(angle[2]);
                               uart_sendChar(angle[3]);
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(distance[0]);
                               uart_sendChar(distance[1]);
                               uart_sendChar(distance[2]);
                               uart_sendChar(distance[3]);
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(width[0]);
                               uart_sendChar(width[1]);
                               uart_sendChar(width[2]);
                               uart_sendChar(width[3]);
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(' ');
                               uart_sendChar(linearWidth[0]);
                               uart_sendChar(linearWidth[1]);
                               uart_sendChar(linearWidth[2]);
                               uart_sendChar(linearWidth[3]);
                               uart_sendChar('\n');
                               uart_sendChar('\r');

                           }
                       }

                   }
                }
}
//    timer_init();
//    lcd_init();
////    uart_init(115200);
//
//
//    uart_init();
//    oi_t *sensor = oi_alloc();
//    oi_init(sensor);
//    cyBOT_init_Scan(0b0111);
//    uart_interrupt_init();
//    adc_init();
//    uart_sendChar('n');
//    cyBOT_Scan_t scan;
//
//    int s = 0, i = 0;
//    char array[15];
//    autostruct_t* min_vals = NULL;
//    char adcRead[4];
////    char pingScan[4];
//    char calculatedIRDist[4];
//    while(1){
////        cyBOT_Scan(90, &scan);
//        sprintf(adcRead, "%d", adc_read());
////        sprintf(pingScan, "%f", scan.sound_dist);
//        sprintf(calculatedIRDist, "%f", 1000000.0*pow(adc_read(), -1.468));
//        uart_sendChar('\n');
//        uart_sendChar('\r');
////        for(i = 0; i < 4; i ++){
////            uart_sendChar(pingScan[i]);
////        }
////        uart_sendChar(',');
////        uart_sendChar(' ');
//        for(i = 0; i < 4; i ++){
//            uart_sendChar(adcRead[i]);
//            lcd_putc(adcRead[i]);
//        }
//        uart_sendChar(',');
//        uart_sendChar(' ');
//        for(i = 0; i < 4; i ++){
//            uart_sendChar(calculatedIRDist[i]);
//
//        }
//        timer_waitMillis(300);
//        lcd_clear();
//
//    }
//
//
//    while(s != 'q')
//    {
//        oi_update(sensor);
//        if(flag == 1)
//        {
//            flag = 0;
//            s = uart_data;
//            uart_data = 0;
//        }
////        lcd_clear();
//        sprintf(array, "Got an %c", s);
//
//        for(i = 0; i < 15; i++)
//        {
//            uart_sendChar(array[i]);
//        }
//        uart_sendChar('\r');
//
////        lcd_putc(s);
//
//
//        if(s == 'w') {
//            oi_setWheels(150, 150);
//        }
//        else if(s == 'a') {
//            oi_setWheels(150, -150);
//        }
//        else if(s == 'd') {
//            oi_setWheels(-150, 150);
//        }
//        else if(s == 's') {
//            oi_setWheels(-150,-150);
//        }
//        else if(s == 'm')
//        {
////            lab3part1(sensor);
//            min_vals = object_scan(sensor);
//            s = 0;
//        }
////        else if(s == 'c')
////        {
////            char op[40];
////            cyBOT_Scan_t *ping_vals = malloc(sizeof(cyBOT_Scan_t));
////            int i = 0, sum = 0;
////
////            for(i = 0; i<20; i++)
////            {
////                cyBOT_Scan(90, ping_vals);
////                sum += adc_read();
////            }
////            sum /= 20;
////
////            float ret_val = est_distance(sum);
////
////            lcd_clear();
////            sprintf(op, "Raw: %04d           Est: %3.2f", sum, ret_val);
////            lcd_puts(op);
////            s = 0;
////        }
//        else if(s == 'n')
//        {
//            min_vals = object_scan(sensor);
//            auto_drive(min_vals, sensor);
//            s = 0;
//        }
//        else if(s == 't')
//        {
//            s = 0;
//            min_vals = object_scan(sensor);
////            bump_drive(min_vals, sensor);
//            while ((min_vals->objDist) > 6 && bump_drive(min_vals, sensor) < ((min_vals->objDist)-6))
//            {
//                min_vals = object_scan(sensor);
//            }
//        }
//        else
//        {
////            oi_free(sensor);
//            oi_setWheels(0,0);
//            if(hasBumpedLeft(sensor))
//                uart_sendChar('L');
//
//            if(hasBumpedRight(sensor))
//                uart_sendChar('R');
//        }
//
////        cyBOT_Scan(0, sensor);
//
//    }
//
//    oi_free(sensor);
//}
