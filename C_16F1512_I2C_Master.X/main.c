/*
 * File:   main.c
 * Author: Jonas
 *
 * Created on August 4, 2019, 2:28 PM
 */


#include <xc.h>
#include "conf.h"

#define _XTAL_FREQ 4000000


void I2C_Master_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start()
{
  I2C_Master_Wait();
  SEN = 1;
}

void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  RSEN = 1;
}

void I2C_Master_Stop()
{
  I2C_Master_Wait();
  PEN = 1;
}

void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSPBUF = d;
}

void i2cWait(){
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
  return;
}

void i2cSend(unsigned int data){
//    i2cWait();
//    SEN = 1;           //start condition
//    i2cWait();
//    SSPBUF = 0x30;     //7-bit address of slave device plus 0 for write
//    i2cWait(); 
//    SSPBUF = data;        //data to be sent
//    i2cWait();
//    PEN = 1;           //stop condition
//    __delay_ms(100);
    
    I2C_Master_Start();         //Start condition
    I2C_Master_Write(0x30);     //7 bit address + Write
    I2C_Master_Write(data);     //Write data
    I2C_Master_Stop();          //Stop condition
}


void main(void) {
    init();
    LATCbits.LATC7 = 1;
    
    // value that is going to be incrementet
    unsigned int v = 0;
    unsigned int i;
    while (1) {
        __delay_ms(100);
        v++;
        i2cSend(v);
        LATA = v;
    }
}
