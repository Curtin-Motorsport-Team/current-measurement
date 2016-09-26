/*
 * File:   Main.c
 * Author: Timothy Moisley
 *
 * Created on 17 August 2016, 12:31 PM
 */


#include <xc.h>
#include<pic18f46k80.h>
#include<stdint.h>
#include"CAN.h"


unsigned char data[8], data1[8], data2[8]; //Need to get these variables from CAN.c, now declared in this file
void timersetter (void);

void main(void) 
{
    
    CAN_Initialise();
    
    
    
    uint8_t result_high, result_low; //creates variables
   
    timersetter();
  
    TRISB0 = 1;
    ANCON0bits.ANSEL0 = 1;
    ADCON1bits.VCFG = 0b00; //upper voltage reference for ADC
    ADCON1bits.VNCFG = 0; //sets lower reference to VSS pin - connect to ground
    ADCON0bits.CHS = 0b0001; //sets to channel one -- pg 366 for setup for whole ADC
    ADCON2bits.ACQT = 0b110; // Acquition divider set to 16 --- NOTE: come back and change if it doesn't work
    ADCON2bits.ADCS = 0b101; //divider of 16 for ADC 
    ADCON2bits.ADFM = 1; //right justified output of data (8 lowest bits in one reg, 4 highest bits in another reg)
    ADCON0bits.ADON = 1; //Turns on ADC
            
    while(1)
    {
        ADCON0bits.GO_DONE = 1; //Starts reading for ADC
        while (ADCON0bits.GO_DONE);
        
                        //pg 361 for result register info
        
        result_high = ADRESH & 0b00001111; //Places 4/12 high bits into the output register
        result_low = ADRESL; //Places 8/12 low bits into low output register 
        
        data[0]=result_high;
        data[1]=result_low;
        
        CAN_Transmit(data[8],  ,2);
        
        
        
        
    }
    
    return;
}

void timersetter(void)
{
    OSCCONbits.IRCF=0b111; //set clock to 16MHz, pg 53 of data sheet
}

