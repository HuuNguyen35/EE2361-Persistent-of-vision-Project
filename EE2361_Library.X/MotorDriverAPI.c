#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "blommel_lab2b_asmLib_v1.h"

#define FCY 8000000UL
#include <libpic30.h>
//
//#pragma config FNOSC = PRI
//#pragma config POSCMOD = XT
//#pragma config FCKSM = CSDCMD
//#pragma config FWDTEN = OFF
//#pragma config JTAGEN = OFF

#define DIR_TRIS   TRISAbits.TRISA1
#define DIR_LAT    LATAbits.LATA1
#define PWM_TRIS   TRISBbits.TRISB2

void delay(int nDelay) {
    for (int i = 0; i < nDelay; ++i ) {
        oneMilliSecond();
    }
}

void pwm_init(void)
{
    T2CON = 0; TMR2 = 0;
    PR2 = 399;
    T2CONbits.TCKPS = 0b00;
    T2CONbits.TON = 1;

    OC1CON = 0;
    OC1R = 0; OC1RS = 0;
    OC1CONbits.OCTSEL = 0;  
    OC1CONbits.OCM = 0b110;

    PWM_TRIS = 0;
}

void pps_init(void)
{
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    RPOR1bits.RP2R = 18;
    __builtin_write_OSCCONL(OSCCON | 0x40);
}

void pwm_set_duty(uint16_t duty)
{
    if (duty > PR2) duty = PR2;
    OC1RS = duty;
}

void motor_init(void)
{
    DIR_TRIS = 0; DIR_LAT = 0;
    pps_init();
    pwm_init();
}

void motor_set_direction(uint8_t dir)
{
    DIR_LAT = (dir ? 1 : 0);
}

void motor_set_speed_percent(uint8_t speed)
{
    if (speed > 100) speed = 100;
    uint16_t duty = (uint16_t)((speed * PR2) / 100);
    pwm_set_duty(duty);
}

//int main(void)
//{
//    motor_init();
//
//    while(1)
//    {
//        // Forward, high speed
//        motor_set_direction(1);
//        motor_set_speed_percent(80);
//        delay(2000);
//
//        // Forward, medium speed
//        motor_set_speed_percent(50);
//        delay(2000);
//
//        // Forward, low speed
//        motor_set_speed_percent(20);
//        delay(2000);
//
//        // Stop
//        motor_set_speed_percent(0);
//        delay(1000);
//
//        // Reverse, low speed
//        motor_set_direction(0);
//        motor_set_speed_percent(20);
//        delay(2000);
//
//        // Reverse, medium speed
//        motor_set_speed_percent(50);
//        delay(2000);
//
//        // Reverse, high speed
//        motor_set_speed_percent(80);
//        delay(2000);
//
//        // Stop
//        motor_set_speed_percent(0);
//        delay(1000);
//    }
//
//    return 0;
//}
