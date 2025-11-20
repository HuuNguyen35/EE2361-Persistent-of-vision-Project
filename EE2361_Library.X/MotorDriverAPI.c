//#include <xc.h>
//#include <stdint.h>
//
//// ========================= FCY & DELAY =========================
//#define FCY 8000000UL      // Instruction clock = Fosc/2 = 16MHz/2
//#include <libpic30.h>      // Gives us __delay_ms(), __delay_us()
//
//// ========================= CONFIG BITS =========================
//// Using 16 MHz primary crystal (XT mode)
//#pragma config FNOSC = PRI           // Primary oscillator
//#pragma config POSCMOD = XT          // XT crystal mode
//#pragma config FCKSM = CSDCMD        // Clock switching disabled
//#pragma config FWDTEN = OFF          // Watchdog Timer disabled
//#pragma config JTAGEN = OFF          // Disable JTAG
//
//// ===============================================================
//// DRV8835 PHASE / ENABLE PIN MAPPING
//// ===============================================================
//// Direction pin = APHASE = RA0
//#define DIR_TRIS   TRISAbits.TRISA0
//#define DIR_LAT    LATAbits.LATA0
//
//// PWM pin = AENBL = RB2 (OC1)
//#define PWM_TRIS   TRISBbits.TRISB2
//
//// ===============================================================
//// PWM + MOTOR DRIVER SETUP
//// ===============================================================
//
//void pps_init(void)
//{
//    // Unlock PPS
//    __builtin_write_OSCCONL(OSCCON & 0xBF);
//
//    // Map OC1 to RP2 (RB2)
//    RPOR1bits.RP2R = 18;   // 18 = OC1
//
//    // Lock PPS
//    __builtin_write_OSCCONL(OSCCON | 0x40);
//}
//
//
//void pwm_init(void)
//{
//    // ----- Configure Timer2 as PWM time base -----
//    T2CON = 0;
//    TMR2 = 0;
//
//    // 20 kHz PWM with 16MHz crystal ? FCY = 8 MHz
//    // PR2 = (FCY / PWMfreq) - 1
//    // PR2 = (8,000,000 / 20,000) - 1 = 399
//    PR2 = 399;
//
//    T2CONbits.TCKPS = 0b00;   // 1:1 prescaler
//    T2CONbits.TON = 1;        // Start Timer2
//
//    // ----- Configure OC1 for PWM output -----
//    OC1CON = 0;               // Turn off module to configure
//    OC1R = 0;                 // Initial duty
//    OC1RS = 0;                // Reset secondary compare
//
//    OC1CONbits.OCTSEL = 0;    // Timer2 as source
//    OC1CONbits.OCM = 0b110;   // PWM mode
//
//    PWM_TRIS = 0;             // RB2 ? output
//}
//
//void pwm_set_duty(uint16_t duty)
//{
//    if (duty > PR2) duty = PR2;  // Limit
//    OC1RS = duty;
//}
//
//// ===============================================================
//// MOTOR CONTROL
//// ===============================================================
//
//void motor_init(void)
//{
//    DIR_TRIS = 0;   // RA0 output
//    DIR_LAT = 0;    // default direction
//    pps_init();
//    pwm_init();
//}
//
//void motor_set_direction(uint8_t dir)
//{
//    DIR_LAT = (dir ? 1 : 0);   // 1 = forward, 0 = reverse
//}
//
//void motor_set_speed_percent(uint8_t speed)
//{
//    if (speed > 100) speed = 100;
//    uint16_t duty = (uint16_t)((speed * PR2) / 100);
//    pwm_set_duty(duty);
//}
//
//// ===============================================================
//// MAIN
//// ===============================================================
//
//int main(void)
//{
//    AD1PCFG = 0xFFFF;
//    motor_init();
//
//    while (1)
//    {
//        // Forward ramp test
//        motor_set_direction(1);
//        for (int i = 0; i <= 100; i++)
//        {
//            motor_set_speed_percent(i);
//            __delay_ms(20);
//        }
//
//        // Reverse ramp test
//        motor_set_direction(0);
//        for (int i = 0; i <= 100; i++)
//        {
//            motor_set_speed_percent(i);
//            __delay_ms(20);
//        }
//    }
//
//    return 0;
//}