#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#define FCY 16000000UL
#include <libpic30.h>


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


#include "lcd_api_v2.h"
#include "pattern.h"       // gives used_angles, PAT_ANGLES, etc.
#include "SensorAPI.h"
#include "LEDAPI.h"

#define LED_ON_US       500
#define PULSES_PER_REV      2

#define TEST_LED_TRIS   TRISBbits.TRISB12
#define TEST_LED_LAT    LATBbits.LATB12
#define TEST_LED_AN     AD1PCFGbits.PCFG12   // analog config for RB14

#define BTN_TRIS    TRISBbits.TRISB15
#define BTN_PORT    PORTBbits.RB15   // 1 = released (pulled up), 0 = pressed
#define DEBOUNCE_MS 20


volatile int testLedState = 0;

void initTestLed(void){
    TEST_LED_AN = 1;      // digital
    TEST_LED_TRIS = 0;    // output
    TEST_LED_LAT = 0;     // LED off
}

void testLed(void){
    testLedState ^= 1;    // toggle 0/1
    TEST_LED_LAT = testLedState;
}

void initButton(void)
{
    BTN_TRIS = 1;    // RB15 as input
    // No need to touch AD1PCFG here; main sets all digital.
}


uint8_t button_pressed_edge(void)
{
    static uint8_t prev_state = 1;   // assume idle high at startup
    uint8_t curr = BTN_PORT;

    // Rising vs falling edges:
    // Idle: 1, Press: 0 (pulled low)

    // Detect potential falling edge: 1 -> 0
    if (prev_state == 1 && curr == 0) {
        // debounce the press
        __delay_ms(DEBOUNCE_MS);
        curr = BTN_PORT;
        if (curr == 0) {
            // confirmed press
            prev_state = 0;
            return 1;  // report one "press" event
        }
    }

    // Detect potential rising edge: 0 -> 1 (button released)
    if (prev_state == 0 && curr == 1) {
        __delay_ms(DEBOUNCE_MS);
        curr = BTN_PORT;
        if (curr == 1) {
            prev_state = 1;
        }
    }

    // No new press event
    return 0;
}


// ... config bits + AD1PCFG + clock setup ...
static void delay_us_32(uint32_t us)
{
    while (us >= 60000U) {
        __delay_us(60000U);   // safe chunk under 16-bit limit
        us -= 60000U;
    }
    if (us > 0U) {
        __delay_us((uint16_t)us);
    }
}

static void t1_init(void)
{
    T1CON = 0;
    TMR1  = 0;

    T1CONbits.TCKPS = 0b10;   // 1:64 prescale -> 4 us per tick at 16 MHz
    T1CONbits.TCS   = 0;      // internal clock (Fosc/2 = FCY)
    T1CONbits.TON   = 1;      // start Timer1
}



// Blocks until buffer is full and returns a frozen step time (µs per angular step).
// Also optionally prints the RPM to the LCD once.
//static uint32_t measure_and_freeze_step_time_us(void)
//{
//    // Wait for buffer to fill
//    while (!sensor_buffer_is_full()) {
//        __delay_ms(20);
//    }
//
//    // OPTIONAL: wait a little longer to let the average settle
//    for (int i = 0; i < 20; i++) {
//        __delay_ms(20);
//    }
//
//    // Compute average tick period
//    uint32_t avg_ticks = sensor_get_average_delta_ticks();
//    if (avg_ticks == 0) {
//        // fallback to some default slow speed if something went wrong
//        return 5000; // 5 ms per step
//    }
//
//    float period_pulse_s = sensor_ticks_to_seconds(avg_ticks);   // seconds between pulses
//    float T_rev_s        = period_pulse_s * (float)PULSES_PER_REV;  // seconds per revolution
//
//    float step_us_f = 0.0f;
//    if (used_angles > 0 && rpm > 0.1f) {
//        float T_step_s = T_rev_s / (float)used_angles;   // seconds per angle
//        step_us_f = T_step_s * 1.0e6f;                   // µs per angle
//    } else {
//        // fallback if RPM is weird
//        step_us_f = 5000.0f;  // 5 ms/step
//    }
//
//    // Clamp to sane bounds so we don't get crazy fast / crazy slow
//    if (step_us_f < 500.0f)   step_us_f = 500.0f;   // min 0.5 ms per step
//
//    // OPTIONAL: show frozen RPM on LCD once
//    char buf[16];
//    lcd_setCursor(0, 0);
//    snprintf(buf, sizeof(buf), "RPM:%7.1f", (double)rpm);
//    for (int i = 0; buf[i] != '\0'; i++) {
//        lcd_printChar(buf[i]);
//    }
//
//    // Now we?re done with the sensor forever for this run
//    sensor_rpm_shutdown();
//
//    return (uint32_t)step_us_f;
//}



//int main(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0xFFFF;     // all digital
//
//    lcd_init();           // optional; you can comment this out if you don't care here
//    ws_init();            // WS2812 driver init
//    initButton();         // RB15 input
//
//    uint8_t theta = 0;
//
//    // Show initial angle
//    ws_show_column(theta);
//
//    while (1) {
//        if (button_pressed_edge()) {
//            // Advance to next angle on each press
//            theta++;
//            if (theta >= used_angles) {
//                theta = 0;
//            }
//
//            // Update the LEDs to show this angle
//            ws_show_column(theta);
//
//            // OPTIONAL: show theta on LCD for debugging
//            /*
//            char buf[16];
//            lcd_setCursor(0, 0);
//            snprintf(buf, sizeof(buf), "theta:%3u", theta);
//            for (int i = 0; buf[i] != '\0'; i++) {
//                lcd_printChar(buf[i]);
//            }
//            */
//        }
//
//        // Tiny idle delay to avoid hammering PORTB
//        __delay_ms(1);
//    }
//
//    return 0;
//}




//int main(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0xFFFF;
//
//    ws_init();
//
//    uint8_t theta = 0;
//    ws_
//    while (1) {
////        ws_show_column(theta);
////        theta++;
////        if (theta >= used_angles) {
////            theta = 0;
////        }
//        __delay_ms(300);   // slow so you can see each position
//    }
//}





//int main(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0xFFFF;     // all digital
//
//    lcd_init();
//    sensor_rpm_init();    // starts T2 + IC1 interrupts :contentReference[oaicite:2]{index=2}
//    ws_init();            // WS2812 driver init :contentReference[oaicite:3]{index=3}
//
//    // ---- PHASE 1: measure RPM & freeze timing ----
//    //uint32_t step_us = measure_and_freeze_step_time_us();
//
//    // ---- PHASE 2: pure LED POV using frozen timing ----
//    uint8_t theta = 0;
//
//    while (1) {
//        // Show one angular column
//        ws_flash_column(theta, LED_ON_US);   // uses pattern[][][] and ws_show() 
//
//        theta++;
//        if (theta >= used_angles) {
//            theta = 0;
//        }
//
//        // Wait the frozen step time; NO sensor calls, NO sensor interrupts
//        delay_us_32(4);
//    }
//
//    return 0;
//}




//int main(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0xFFFF;
//
//    initTestLed();         // LED on RB12
//    lcd_init();            // should now return
//
//    TEST_LED_LAT = 1;      // if this executes, lcd_init didn't hang
//    while (1) {
//        ;
//    }
//}
//int main(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0xFFFF;
//    
//    initTestLed();
//
//    // TODO: put your old LCD TRIS setup here
//    // e.g. TRISB = 0x0000; or specific bits
//
//    
//    lcd_init();
//    
//    TEST_LED_LAT = 1;
//    while(1){
//        ;
//    }
//    lcd_setCursor(0, 0);
//    lcd_printChar('A');
//    
//    
//
//    while (1) {
//        lcd_setCursor(0, 0);
//        lcd_printChar('A');
//        
//        testLed();
//        
//        __delay_ms(1000);
//    }
//}



int main(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFF;
    
    t1_init();
    lcd_init();
    sensor_rpm_init();
    ws_init();
    
    
    initTestLed();

    uint8_t theta = 0;
    char top_buf[16];
    char bot_buf[16];

    // We?ll update the LCD less frequently to avoid spamming it.
    uint16_t step_counter = 0;
    uint16_t step_ticks   = 0;     // Timer1 ticks per angular step (4 us per tick)
    uint16_t next_t1      = 0;     // scheduled time for next LED column
    uint8_t  pov_started  = 0;

//    int num1 = 28;
//    ws_total_reset(num1);
    
    //37us per LED, + 80us per col send, 2 cols sent
    
    testLed();
    while (!button_pressed_edge()) {
        __delay_ms(5);   // tiny debounce-friendly idle
    }
    testLed();
    while (1) {
        // --- Read sensor state ---
        uint32_t hits   = sensor_get_hit_count();
        uint8_t  buf_n  = sensor_get_buffer_count();
        uint8_t  buf_ok = sensor_buffer_is_full();

        uint32_t avg_ticks = 0;
        float    rpm_avg   = 0.0f;
        float    step_us_f = 0.0f;

        if (buf_ok) {
            rpm_avg = sensor_get_filtered_rpm();   // NEW filtered RPM

            if (used_angles > 0 && rpm_avg > 0.1f) {
                // T_rev_s = 60 / RPM  (rpm already accounts for pulses/rev)
                float T_rev_s  = 60.0f / rpm_avg;
                float T_step_s = T_rev_s / (float)used_angles;
                step_us_f      = T_step_s * 1.0e6f;

                // convert µs -> Timer1 ticks (4 us per tick)
                float ticks_f = step_us_f / 4.0f;
                if (ticks_f < 1.0f)     ticks_f = 1.0f;
                if (ticks_f > 65000.0f) ticks_f = 65000.0f;
                step_ticks = (uint16_t)ticks_f;
            } else {
                step_us_f  = 0.0f;
                step_ticks = 0;
            }
        }

        // --- Update LCD every N steps ---
        if ((step_counter++ % 100) == 0) {
            // Top row: hit count / buffer fill
            lcd_setCursor(0, 0);
            snprintf(top_buf, sizeof(top_buf), "%5lu %2u/%u",
                     (unsigned long)hits,
                     (unsigned)buf_n,
                     (unsigned)N_SAMPLES);   // N_SAMPLES from sensor_rpm.c
            for (int i = 0; top_buf[i] != '\0'; i++) {
                lcd_printChar(top_buf[i]);
            }

            // Bottom row: RPM only if buffer full
            lcd_setCursor(0, 1);
            if (buf_ok && rpm_avg > 0.1f) {
                snprintf(bot_buf, sizeof(bot_buf), "%7.1fRPM", (double)rpm_avg);
            } else {
                // indicate "not ready" / filling buffer
                snprintf(bot_buf, sizeof(bot_buf), "  FILLING ");
            }
            for (int i = 0; bot_buf[i] != '\0'; i++) {
                lcd_printChar(bot_buf[i]);
            }
        }

        // --- POV LED timing ---
        if (buf_ok && step_us_f > 0.0f) {
            // Ensure a sane minimum / maximum step time
            //if (step_us_f < 500.0f)   step_us_f = 500.0f;     // don't go crazy fast
//            if (step_us_f > 50000.0f) step_us_f = 50000.0f;   // cap at 50 ms/step

            uint16_t now = TMR1;
            
            if (!pov_started){
                pov_started = 1;
                next_t1 = now;
            }
            
            uint16_t elapsed = (uint16_t)(now - next_t1);

            
            if (elapsed >= step_ticks) {
                // Time for the next angular position
                ws_flash_column(theta, LED_ON_US);   // still 500 us ON-time :contentReference[oaicite:3]{index=3}

                theta++;
                if (theta >= used_angles) {
                    theta = 0;
                }

                // schedule next update; we add step_ticks instead of using 'now'
                // so small timing jitter doesn't accumulate
                next_t1 += step_ticks;
            }

        } else {
            // Buffer not full or RPM too small ? just idle a bit.
            __delay_ms(50);
        }
    }

    return 0;
}
