#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define FCY 16000000UL  // must match system frequency

// ========================= RPM CONFIG ==========================

#define N_SAMPLES        10      // circular buffer length
#define T2_PRESCALE      64      // must match T2 prescaler bits
#define PULSES_PER_REV   1       // 1 pulse per revolution (1 reflective mark)

#define DEBOUNCE_US      50000UL   // 50000 µs = 50 ms

// Convert debounce time to Timer2 ticks:
// ticks = DEBOUNCE_US * FCY / (1e6 * T2_PRESCALE)
#define DEBOUNCE_TICKS  ((uint32_t)((DEBOUNCE_US * (FCY / 1000000UL)) / T2_PRESCALE))

// ========================= GLOBALS =============================

static volatile uint32_t period_buf[N_SAMPLES];
static volatile uint8_t  period_index   = 0;
static volatile uint8_t  buffer_count   = 0;   // how many valid entries (0..N_SAMPLES)
static volatile uint32_t ic1_hits       = 0;
static volatile uint32_t t2_overflows   = 0;
static volatile uint32_t last_time32    = 0;
static volatile uint32_t latest_delta32 = 0;

// ========================= GPIO / PPS SETUP ====================
// Assumes sensor DO is connected to RP4 / RB4.

static void sensor_pin_init(void)
{
    TRISBbits.TRISB4 = 1;      // RB4 as input

    // Map RP4 to IC1 using PPS:
    // RPINR7bits.IC1R selects the RPx input.
    // IC1R = 4 -> IC1 <- RP4
    RPINR7bits.IC1R = 4;
}

// ========================= TIMER2 SETUP ========================

static void timer2_init(void)
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

static void ic1_init(void)
{
    IC1CONbits.ICTMR = 1;    // use Timer2
    IC1CONbits.ICI   = 0b00; // interrupt on every capture

    last_time32      = 0;
    latest_delta32   = 0;
    ic1_hits         = 0;
    t2_overflows     = 0;
    buffer_count     = 0;

    for (uint8_t i = 0; i < N_SAMPLES; i++) {
        period_buf[i] = 0;
    }
    period_index = 0;

    IFS0bits.IC1IF = 0;
    IEC0bits.IC1IE = 1;
    IC1CONbits.ICM = 0b010;    // capture every falling edge
}

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    IFS0bits.IC1IF = 0;

    while (IC1CONbits.ICBNE) {
        uint16_t cap = IC1BUF;
        uint32_t ov    = t2_overflows;
        uint32_t now32 = (ov << 16) | cap;

        uint32_t delta32 = now32 - last_time32;
        last_time32 = now32;

        if (delta32 < (uint32_t)DEBOUNCE_TICKS) {
            continue;   // ignore bounce / noise
        }

        latest_delta32 = delta32;

        // Store low 16 bits; enough for 600 RPM+ with our prescale.
        period_buf[period_index] = delta32;
        period_index = (period_index + 1) % N_SAMPLES;


        if (buffer_count < N_SAMPLES) {
            buffer_count++;
        }

        ic1_hits++;
    }
}

// ========================= PUBLIC API ==========================

void sensor_rpm_init(void)
{
    // Note: AD1PCFG should already be set to digital in main.
    sensor_pin_init();
    timer2_init();
    ic1_init();
}

uint32_t sensor_get_latest_delta_ticks(void)
{
    return latest_delta32;
}

uint32_t sensor_get_hit_count(void)
{
    return ic1_hits;
}

uint8_t sensor_get_buffer_count(void)
{
    return buffer_count;
}

uint8_t sensor_buffer_is_full(void)
{
    return (buffer_count >= N_SAMPLES);
}

// Average over current buffer contents (16-bit values).
uint32_t sensor_get_average_delta_ticks(void)
{
    uint64_t sum   = 0;     // 64-bit to be extra safe
    uint8_t  count = 0;

    // Briefly disable IC1 interrupt so buffer doesn't change while summing.
    IEC0bits.IC1IE = 0;

    uint8_t local_count = buffer_count;
    for (uint8_t i = 0; i < local_count; i++) {
        uint32_t v = period_buf[i];
        if (v != 0) {
            sum += (uint64_t)v;
            count++;
        }
    }

    IEC0bits.IC1IE = 1;

    if (count == 0) {
        return 0;
    }
    return (uint32_t)(sum / (uint64_t)count);
}


float sensor_ticks_to_seconds(uint32_t ticks)
{
    return (float)ticks * ((float)T2_PRESCALE / (float)FCY);
}

float sensor_rpm_from_ticks(uint32_t ticks)
{
    if (ticks == 0) return 0.0f;

    float period_s  = sensor_ticks_to_seconds(ticks);
    float rev_per_s = 1.0f / (period_s * (float)PULSES_PER_REV);

    return rev_per_s * 60.0f;
}
