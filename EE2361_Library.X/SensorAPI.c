#include <xc.h>
#include <stdint.h>

#define FCY 16000000UL  

#define N_SAMPLES           10  // circular buffer length
#define T2_PRESCALE         64 // gotta match Timer2 prescaler
#define PULSES_PER_REV      2  //pulses per revolution (2 for 2 fan blades)

//The "debounding" for the sensor
#define DEBOUNCE_US      50000UL   // 5000 us = 5 ms
#define DEBOUNCE_TICKS  ((uint32_t)((DEBOUNCE_US * (FCY / 1000000UL)) / T2_PRESCALE))


static volatile uint32_t period_buf[N_SAMPLES];
static volatile uint8_t  period_index = 0;
static volatile uint8_t  buffer_count = 0;
static volatile uint32_t ic1_hits = 0;
static volatile uint32_t t2_overflows = 0;
static volatile uint32_t last_time32 = 0;
static volatile uint32_t latest_delta32 = 0;


static void sensor_pin_init(void)
{
    TRISBbits.TRISB4 = 1;
    RPINR7bits.IC1R = 4;
}

static void timer2_init(void)
{
    T2CON = 0;
    TMR2  = 0;

    T2CONbits.TCKPS = 0b10;  // 1:64 prescale (tick = 4 us at 16 MHz)
    T2CONbits.TCS = 0;     // internal clock
    PR2 = 0xFFFF;

    IFS0bits.T2IF = 0; 
    IEC0bits.T2IE = 1; 

    //start timer 2
    T2CONbits.TON = 1;
}

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;
    t2_overflows++;
}

int getBufferSize(){
    return N_SAMPLES;
}

static void ic1_init(void)
{
    IC1CONbits.ICTMR = 1; // use Timer2
    IC1CONbits.ICI   = 0b00; // interrupt on every capture

    last_time32 = 0;
    latest_delta32 = 0;
    ic1_hits = 0;
    t2_overflows = 0;
    buffer_count = 0;

    for (uint8_t i = 0; i < N_SAMPLES; i++) {
        period_buf[i] = 0;
    }
    period_index = 0;

    IFS0bits.IC1IF = 0;
    IEC0bits.IC1IE = 1;
    IC1CONbits.ICM = 0b010;// capture every falling edge
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

// ============== PUBLIC API =======================================
void sensor_rpm_init(void)
{
    sensor_pin_init();
    timer2_init();
    ic1_init();
}


void sensor_rpm_shutdown(void)
{
    IEC0bits.IC1IE = 0;
    IC1CONbits.ICM = 0;
    
    //Disable Timer2
    IEC0bits.T2IE = 0; 
    T2CONbits.TON = 0;
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
    uint64_t sum   = 0;
    uint8_t  count = 0;

    // don't wanna update the values as we calculate this
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





float sensor_get_rpm(void)
{
    uint32_t avg_ticks = sensor_get_average_delta_ticks();
    float rpm = sensor_rpm_from_ticks(avg_ticks);
    return rpm;
}
