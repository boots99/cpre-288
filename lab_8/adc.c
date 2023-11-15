#include "adc.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>

void adc_init(){

//    SYSCTL_RCGCADC_R |= 0x1;
//    while((SYSCTL_PRADC_R & 0x1)==0){};
//    SYSCTL_RCGCGPIO_R |= 0x02;
//    while((SYSCTL_PRGPIO_R & 0x2) | 0x2){};
//    GPIO_PORTB_DIR_R &= 0x01;
//    GPIO_PORTB_AFSEL_R |= 0x10;
//    GPIO_PORTB_DEN_R &= 0x01;
//    GPIO_PORTB_AMSEL_R |= 0x10;
//    ADC0_PC_R &= 0x0;
//    ADC0_PC_R |= 0x1;
//    ADC0_SSPRI_R = 0x0123;
//    ADC0_ACTSS_R &= ~0x0008;
//    ADC0_EMUX_R &= ~0x000F;
//    ADC0_SSMUX3_R &= ~0x000F;
//    ADC0_SSMUX3_R += 10;
//    ADC0_SSCTL3_R = 0x0006;
//    ADC0_ACTSS_R |= 0x0008;

    SYSCTL_RCGCADC_R |= 0x1;
    SYSCTL_RCGCGPIO_R |= 0x2;
    GPIO_PORTB_DEN_R   &= 0b11101111;
    GPIO_PORTB_AFSEL_R |= 0b00010000;
    GPIO_PORTB_AMSEL_R |= 0b00010000;

    // Using SS3
    ADC0_ACTSS_R  &= 0b0111;    //disabling SS3
    ADC0_EMUX_R  &= 0x0FFF;    //
//    ADC0_EMUX_R |= 0x4000;  // Setting trigger to just GPIO
    ADC0_SSMUX3_R = 0xA;
    ADC0_SSCTL3_R = 0b0110;      // Interrupt and end bit
    ADC0_IM_R    |= 0b1000;      // Mask for SS3
    ADC0_ACTSS_R |= 0b1000;    // Enable SS3
//    NVIC_EN0_R |= 0x00020000; //17

}

int adc_read(){

//    ADC0_PSSI_R = 0x1;
//    int output = ADC0_SSFIFO0_R & 0xFFF;
//    char poutput[3];
//    ADC0_ISC_R = 0x1;
//    sprintf(poutput, "%d", output);
//    lcd_puts(poutput);
//    return output;

    ADC0_PSSI_R |= 0b1000;
    while(~ADC0_RIS_R & 0b1000)
    {}
    ADC0_ISC_R |= 0b1000;
    return ADC0_SSFIFO3_R & 0xFFF;
}
