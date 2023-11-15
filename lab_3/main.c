

/**
 * main.c
 */

#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"
#include <math.h>


int main(void)
{
    cyBot_uart_init();
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0011);
    //cyBOT_SERVO_cal();

    right_calibration_value = -29750;
    left_calibration_value = 1498000 ;

    int i;
    char scanval[4];
    float previousScanVal;
    int objectStartDegree;
    int objectEndDegree;
    char angle[4];
    char distance[4];
    char width[4];
    char linearWidth[4];
    typedef struct{
        int degree;
        int distance;
        int width;
        int linearWidth;
    }Object;

    Object objectList[4];
    int objectCount = 0;

    cyBOT_Scan_t scan;
    cyBot_getByte();
    cyBot_sendByte('D');
    cyBot_sendByte('e');
    cyBot_sendByte('g');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte('D');
    cyBot_sendByte('i');
    cyBot_sendByte('s');
    cyBot_sendByte('t');
    cyBot_sendByte('\n');
    cyBot_sendByte('\r');

    for(i = 0; i <= 180; i += 2){
        cyBOT_Scan(i, &scan);
        if(scan.sound_dist - previousScanVal < -25){
            //Start object
            objectStartDegree = i;
        }
        if(previousScanVal - scan.sound_dist < -25){
            //end object
            objectEndDegree = i;
            if(objectEndDegree-objectStartDegree > 8){
                //object is valid
                cyBot_sendByte('o');
                cyBot_sendByte('b');
                cyBot_sendByte('j');
                cyBot_sendByte('\n');
                cyBot_sendByte('\r');
                //save object
                objectList[objectCount].degree = (objectEndDegree + objectStartDegree )/ 2;
                objectList[objectCount].distance = previousScanVal;
                objectList[objectCount].linearWidth = abs(2*((sin((objectEndDegree - objectStartDegree)/2))*objectList[objectCount].distance));
                objectList[objectCount].width = objectEndDegree-objectStartDegree;
                objectCount++;
            }

        }
        cyBot_sendByte((i/100+48));
        cyBot_sendByte((((i%100)/10)+48));
        cyBot_sendByte(((i%10)+48));
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        sprintf(scanval, "%f", scan.sound_dist);
        cyBot_sendByte(scanval[0]);
        cyBot_sendByte(scanval[1]);
        cyBot_sendByte(scanval[2]);
        cyBot_sendByte(scanval[3]);
        //cyBot_sendByte('\r');
        cyBot_sendByte('\n');
        cyBot_sendByte('\r');
        previousScanVal = scan.sound_dist;
    }

    cyBot_sendByte('O');
    cyBot_sendByte('b');
    cyBot_sendByte('j');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte('A');
    cyBot_sendByte('n');
    cyBot_sendByte('g');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte('D');
    cyBot_sendByte('i');
    cyBot_sendByte('s');
    cyBot_sendByte('t');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte('W');
    cyBot_sendByte('i');
    cyBot_sendByte('d');
    cyBot_sendByte('t');
    cyBot_sendByte('h');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte(' ');
    cyBot_sendByte('L');
    cyBot_sendByte('W');
    cyBot_sendByte('i');
    cyBot_sendByte('d');
    cyBot_sendByte('t');
    cyBot_sendByte('h');
    cyBot_sendByte('\n');
    cyBot_sendByte('\r');
    for(i = 0; i < objectCount; i ++){
        cyBot_sendByte(i + 48);
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        sprintf(angle, " %d", objectList[i].degree);
        sprintf(distance, " %d", objectList[i].distance);
        sprintf(width, " %d", objectList[i].width);
        sprintf(linearWidth, " %d", objectList[i].linearWidth);
        cyBot_sendByte(angle[0]);
        cyBot_sendByte(angle[1]);
        cyBot_sendByte(angle[2]);
        cyBot_sendByte(angle[3]);
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(distance[0]);
        cyBot_sendByte(distance[1]);
        cyBot_sendByte(distance[2]);
        cyBot_sendByte(distance[3]);
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(width[0]);
        cyBot_sendByte(width[1]);
        cyBot_sendByte(width[2]);
        cyBot_sendByte(width[3]);
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(' ');
        cyBot_sendByte(linearWidth[0]);
        cyBot_sendByte(linearWidth[1]);
        cyBot_sendByte(linearWidth[2]);
        cyBot_sendByte(linearWidth[3]);
        cyBot_sendByte('\n');
        cyBot_sendByte('\r');
    }
    int shortestWidth = objectList[0].width;
    int shortestObjectIndex = 0;
    for(i = 0; i < objectCount; i ++){
        if(objectList[i].width < shortestWidth){
            shortestWidth = objectList[i].width;
            shortestObjectIndex = i;
        }
    }
    cyBOT_Scan(objectList[shortestObjectIndex].degree, &scan);

	return 0;
}
