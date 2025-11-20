// Sensor API

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd_api_v2.h"

// ========================= FCY & DELAY =========================
// Set this to your instruction clock frequency (Fosc/2).
// Example: if Fosc = 32 MHz, FCY = 16 MHz
#define FCY 16000000UL
#include <libpic30.h>  // for __delay_ms, __delay_us

// ========================= CONFIG BITS =========================
// *** Adjust these for your specific PIC24 and clock setup ***

#pragma config FNOSC = FRCPLL    // Internal FRC with PLL (example)
#pragma config POSCMOD = NONE    // No primary oscillator
#pragma config FCKSM  = CSECME   // Clock switching enabled, fail-safe enabled
#pragma config OSCIOFNC = OFF    // CLKO pin disabled
#pragma config FWDTEN = OFF      // Watchdog Timer disabled

#pragma config JTAGEN = OFF      // Disable JTAG

// ========================= RPM CONFIG ==========================

#define N_SAMPLES        10      // circular buffer length
#define T2_PRESCALE      64      // must match T2 prescaler bits
#define PULSES_PER_REV   1       // 1 pulse per revolution (1 reflective mark)


#define DEBOUNCE_US      50000UL   // 5000 µs = 5 ms
// Convert debounce time to Timer2 ticks:
// ticks = DEBOUNCE_US * FCY / (1e6 * T2_PRESCALE)
#define DEBOUNCE_TICKS  ((uint16_t)((DEBOUNCE_US * (FCY / 1000000UL)) / T2_PRESCALE))

// ========================= GLOBALS =============================

volatile uint16_t period_buf[N_SAMPLES];
volatile uint8_t  period_index = 0;
volatile uint16_t last_capture = 0;
volatile uint32_t ic1_hits = 0;
volatile uint32_t t2_overflows = 0;
volatile uint32_t last_time32       = 0;
volatile uint32_t latest_delta32    = 0;   // replace latest_delta_ticks

// ========================= STUB: PRINT RPM =====================

static void print_rpm(float rpm)
{
    // Replace this with your own display function.
    // For now, if you've got UART/stdio hooked up, you could do:
    // printf("RPM: %.1f\r\n", rpm);

    (void)rpm; // prevent unused warning if empty
}

// ========================= GPIO / PPS SETUP ====================
// Assumes sensor DO is connected to RP0 / RB0.
// Adjust if you use a different RP pin.

static void sensor_pin_init(void)
{
    // RB0 as input
    TRISBbits.TRISB4 = 1;

    // Map RP0 to IC1 using PPS:
    // On many PIC24FJ devices, RPINR7bits.IC1R selects the RPx input.
    // IC1R = 0 -> IC1 <- RP0
    RPINR7bits.IC1R = 4;
}

// ========================= TIMER2 SETUP ========================
// Free-running timer used as the timebase for Input Capture.

void timer2_init(void)
{
    T2CON = 0;
    TMR2  = 0;

    T2CONbits.TCKPS = 0b10;  // 1:64 prescale (tick = 4 us at 16 MHz)
    T2CONbits.TCS   = 0;     // internal clock
    PR2             = 0xFFFF;

    IFS0bits.T2IF = 0;       // clear flag
    IEC0bits.T2IE = 1;       // enable Timer2 interrupt

    T2CONbits.TON = 1;       // start Timer2
}

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;       // clear Timer2 interrupt flag
    t2_overflows++;          // increment overflow counter
}

// ========================= INPUT CAPTURE 1 SETUP ===============
// Capture edges from the sensor signal and store periods.

static void ic1_init(void)
{
    // Use Timer2 as timebase
    IC1CONbits.ICTMR = 1;      // 1 = Timer2

    // Interrupt on every capture event
    IC1CONbits.ICI = 0b00;     // Interrupt on every capture

    // Clear buffer & state
    last_capture = 0;
    for (uint8_t i = 0; i < N_SAMPLES; i++) {
        period_buf[i] = 0;
    }
    period_index = 0;

    // Clear and enable interrupt
    IFS0bits.IC1IF = 0;        // Clear flag
    IEC0bits.IC1IE = 1;        // Enable IC1 interrupt

    // Capture mode: falling edge (010) or rising edge (011).
    // TCRT5000 module is typically HIGH = nothing, LOW = object.
    // We'll capture on FALLING edge (object enters beam):
    IC1CONbits.ICM = 0b010;    // 010 = capture every falling edge
}

// ========================= IC1 INTERRUPT SERVICE ===============

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    IFS0bits.IC1IF = 0;    // Clear IC1 interrupt flag

    while (IC1CONbits.ICBNE) {
        uint16_t cap = IC1BUF;          // 16-bit captured Timer2 value

        // Snapshot overflow count *around* this capture.
        // For your use-case, this is plenty accurate.
        uint32_t ov  = t2_overflows;
        uint32_t now32 = (ov << 16) | cap;

        uint32_t delta32 = now32 - last_time32;  // 32-bit wrap-safe
        last_time32 = now32;

        // Debounce in 32-bit ticks
        if (delta32 < (uint32_t)DEBOUNCE_TICKS) {
            continue;   // ignore as noise/bounce
        }

        latest_delta32 = delta32;       // <<< latest dt in ticks (32-bit)

        // If you still want an average, you can either:
        //   - keep period_buf as 32-bit, or
        //   - only use latest_delta32 for RPM and ignore the old buffer.
        // For now I'll just store a truncated 16-bit version:
        period_buf[period_index] = (uint16_t)(delta32 & 0xFFFF);
        period_index = (period_index + 1) % N_SAMPLES;

        ic1_hits++;
    }
}


// ========================= MATH: TICKS -> RPM ==================

static uint16_t average_ticks(void)
{
    // Average the contents of period_buf.
    // Very simple approach; you could filter out zeros / outliers later.

    uint32_t sum = 0;
    uint8_t  count = 0;

    for (uint8_t i = 0; i < N_SAMPLES; i++) {
        uint16_t v = period_buf[i];
        if (v != 0) { // ignore zeros (before sensor starts)
            sum += v;
            count++;
        }
    }

    if (count == 0) return 0;
    return (uint16_t)(sum / count);
}

static inline float ticks_to_seconds(uint32_t ticks)
{
    return (float)ticks * ((float)T2_PRESCALE / (float)FCY);
}

static float rpm_from_ticks32(uint32_t ticks32)
{
    if (ticks32 == 0) return 0.0f;

    float period_s  = ticks_to_seconds(ticks32);
    float rev_per_s = 1.0f / (period_s * (float)PULSES_PER_REV);

    return rev_per_s * 60.0f;
}


// ========================= MAIN ================================
// Choose what to show on the bottom row:
// 0 = show dt (seconds)
// 1 = show RPM
#define DISPLAY_MODE_DT   0
#define DISPLAY_MODE_RPM  1
#define DISPLAY_MODE      DISPLAY_MODE_DT   // <-- change this to DISPLAY_MODE_RPM to show RPM

int main(void)
{
    // Basic clock divider: ensure FRCPLL / FCY matches the FCY define above
    CLKDIVbits.RCDIV = 0;  // FRC post-scaler = 1 (if using FRCPLL, adjust as needed)

    // Disable all analog inputs by default, then re-enable specific ones if needed
    AD1PCFG = 0xFFFF;      // All digital

    // Init sensor pin & PPS
    sensor_pin_init();

    // Init Timer2 and Input Capture
    timer2_init();
    ic1_init();
    
    // init the LCD
    lcd_init();
    
    char txt_str[16];      // shared buffer for dt or RPM text
    char cnt_str[8];       // buffer for hit counter

    while (1) {
        __delay_ms(100);

        // Read latest 32-bit delta (ticks)
        uint32_t dt_ticks = latest_delta32;

        float dt_s = ticks_to_seconds(dt_ticks);
        float rpm  = rpm_from_ticks32(dt_ticks);

        // ---------- Top row: IC1 hit count ----------
        lcd_setCursor(0, 0);
        sprintf(cnt_str, "%lu", (unsigned long)ic1_hits);
        for (int i = 0; cnt_str[i] != '\0'; i++) {
            lcd_printChar(cnt_str[i]);
        }

        // ---------- Bottom row: dt or RPM depending on DISPLAY_MODE ----------
        lcd_setCursor(0, 1);

        #if (DISPLAY_MODE == DISPLAY_MODE_DT)
            // Show dt in seconds, e.g. " 1.234"
            sprintf(txt_str, "%6.3f", (double)dt_s);
        #elif (DISPLAY_MODE == DISPLAY_MODE_RPM)
            // Show RPM, e.g. "1234.5"
            sprintf(txt_str, "%6.1f", (double)rpm);
        #else
            sprintf(txt_str, "MODE ERR");
        #endif

        for (int i = 0; txt_str[i] != '\0'; i++) {
            lcd_printChar(txt_str[i]);
        }
    }

    // Should never reach here
    return 0;
}