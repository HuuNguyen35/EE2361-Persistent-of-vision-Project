#include <xc.h>
#include <stdint.h>
#include "PersistenceOfVisionAPI.h"

#pragma config ICS = PGx1
#pragma config FWDTEN = OFF
#pragma config GWRP = OFF
#pragma config GCP  = OFF
#pragma config JTAGEN = OFF
#pragma config I2C1SEL = PRI
#pragma config IOL1WAY = OFF
#pragma config OSCIOFNC = ON
#pragma config FCKSM = CSECME
#pragma config FNOSC = FRCPLL


void SystemInit(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFF;
}

int main(void){
    SystemInit();
    InitPersistenceOfVision();
    while (1) {
        PersistenceOfVision_Task();
    }
}