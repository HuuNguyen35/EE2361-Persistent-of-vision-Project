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
//
//
//
//#define RPM_SLOW_THRESHOLD    400.0f
//#define RPM_FAST_THRESHOLD    600.0f
//
//
//int main(){
//  SystemInit();
//  InitPersistenceOfVision();
//
//
//  POV_SetLcdUpdate(2000);
//  POV_Start();
//
//
//  uint8_t showingWarning = 0;
//  uint16_t loopCounter = 0;
//
//
//  while(1){
//    PersistenceOfVision_Task();
//
//
//    loopCounter++;
//    if (loopCounter >= 1000){
//      loopCounter = 0;
//      float rpm = POV_GetRPM();
//
//
//      if (rpm < RPM_SLOW_THRESHOLD){
//          if (!showingWarning){
//              POV_Stop_LED_updates();
//              POV_SetStaticColor(255, 0, 0);
//              showingWarning = 1;
//          }
//      }
//      else if (rpm > RPM_FAST_THRESHOLD){
//          if (!showingWarning){
//              POV_Stop_LED_updates();
//              POV_SetStaticColor(255, 0, 0);
//              showingWarning = 1;
//          }
//      }
//      
//      else{
//        if (showingWarning){
//              POV_ClearDisplay();
//              POV_Start();
//              showingWarning = 0;
//          }
//      }
//    }
//   
//  }
//  return 0;
//}
