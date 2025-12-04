//// Main file for testing the LEDs
//#include <xc.h>
//#include <stdint.h>
//#define FCY 16000000UL
//#include <libpic30.h>
//
//#include "LEDAPI.h"
//
//#define NUM_LEDS        10
//
//int main(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0xFFFF;
//
//    ws_init();
//
//    int idx = 0;
//    while (1) {
//        ws_clear_all();
//        ws_set_pixel(idx, 80, 0, 0);  // red
//        ws_show();
//
//        idx++;
//        if (idx >= NUM_LEDS) idx = 0;
//
//        __delay_ms(4);
//    }
//}




