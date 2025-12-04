/*
 * WS2812 LED strip driver using Timer4-based bit timing.
 * Data pin: RA0
 */

#include <xc.h>
#include <stdint.h>

#define FCY 16000000UL
#include <libpic30.h>

#include "pattern.h"
#include "blommel_lab2b_asmLib_v1.h"


// ============================ LED CONFIG ============================

#define NUM_LEDS   14

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

static Pixel leds[NUM_LEDS];

// RA0 as WS2812 data pin
#define LED_PIN_TRIS   TRISAbits.TRISA0
#define LED_PIN_LAT    LATAbits.LATA0
// THE RED WIRE IS +V = VCC
// THE WHITE WIRE IS GND

#define LED_ON_TIME_US_DEFAULT 500000   // tweak as needed - was 500


// ============================ TIMER4-BASED BIT DRIVER ============================
//
// WS2812B timing from datasheet:
//   T0H ? 0.4 us, T0L ? 0.85 us
//   T1H ? 0.8 us, T1L ? 0.45 us
//
// At FCY = 16 MHz, TCY = 62.5 ns.
// Choose counts (rounded to nearest tick):
//   0-bit: T0H =  6 ticks ? 0.375 us
//         T0L = 14 ticks ? 0.875 us
//   1-bit: T1H = 13 ticks ? 0.8125 us
//         T1L =  7 ticks ? 0.4375 us
//

#define WS_T0H_TICKS   6u
#define WS_T0L_TICKS   14u
#define WS_T1H_TICKS   13u
#define WS_T1L_TICKS   7u

static inline void ws_timing_init(void)
{
    // Timer4 as a free-running 16-bit timer @ FCY with prescaler 1:1
    T4CONbits.TON   = 0;    // stop
    T4CONbits.TCKPS = 0;    // 1:1 prescale
    T4CONbits.TCS   = 0;    // internal clock (FOSC/2 = FCY)
    T4CONbits.T32   = 0;    // 16-bit mode
    TMR4            = 0;
    PR4             = 0xFFFF; // free-running
    IFS1bits.T4IF   = 0;
    IEC1bits.T4IE   = 0;    // no interrupts
    T4CONbits.TON   = 1;    // start
}

// Busy-wait until Timer4 reaches target tick count
static inline void ws_wait_ticks(uint16_t target)
{
    while (TMR4 < target) {
        ; // spin
    }
}

// If you ever want a pure C bit-banger instead of assembly, you can
// re-enable these and remove ws_send_pixel_asm(). For now, we use
// your assembly in blommel_lab2b_asmLib_v1.h.
/*
// Generate a '0' bit on RA0
void write_0(void)
{
    TMR4 = 0;
    LED_PIN_LAT = 1;
    ws_wait_ticks(WS_T0H_TICKS);
    LED_PIN_LAT = 0;
    ws_wait_ticks(WS_T0H_TICKS + WS_T0L_TICKS);
}

// Generate a '1' bit on RA0
void write_1(void)
{
    TMR4 = 0;
    LED_PIN_LAT = 1;
    ws_wait_ticks(WS_T1H_TICKS);
    LED_PIN_LAT = 0;
    ws_wait_ticks(WS_T1H_TICKS + WS_T1L_TICKS);
}
*/

// ============================ LOW-LEVEL SEND ============================

// Send one byte MSB-first using your assembly bit writers
static inline void ws_send_byte(uint8_t b)
{
    for (int i = 7; i >= 0; --i) {
        if ((b >> i) & 0x01) {
            write_1();
        } else {
            write_0();
        }
    }
}

// We use your optimized assembly that already sends GRB in one shot.
static inline void ws_send_pixel(uint8_t r, uint8_t g, uint8_t b)
{
    ws_send_pixel_asm(g, r, b);  // GRB order
}

static inline void ws_send_pixel_old(uint8_t r, uint8_t g, uint8_t b)
{
    // Standard WS2812B is GRB
    ws_send_byte(g);
    ws_send_byte(r);
    ws_send_byte(b);
}


// Latch: low for >= 50 us
static inline void ws_reset_latch(void)
{
    LED_PIN_LAT = 0;
    __delay_us(80);   // comfortably above 50 us
}

// ============================ FRAMEBUFFER API ============================

void ws_clear_all(void)
{
    for (int i = 0; i < NUM_LEDS; ++i) {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
    }
}

//void _ws_total_resest(int numLeds){
//    for (int i = 0; i < numLeds; ++i) {
//        ws_send_pixel(0, 0, 0);
//    }
//}

void ws_set_all(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < NUM_LEDS; ++i) {
        leds[i].r = r;
        leds[i].g = g;
        leds[i].b = b;
    }
}

void ws_set_pixel(int idx, uint8_t r, uint8_t g, uint8_t b)
{
    if (idx < 0 || idx >= NUM_LEDS) return;
    leds[idx].r = r;
    leds[idx].g = g;
    leds[idx].b = b;
}
void ws_flash_column(uint8_t theta_idx, uint16_t on_time_us)
{
    // Show pattern for this angle
    ws_show_column(theta_idx);

    __delay_us(on_time_us);

    // Turn strip off
    ws_clear_all();
    ws_show();
}

// Display one angular column from pattern[][][] onto strip
void ws_show_column(uint8_t theta_idx)
{
    if (theta_idx >= used_angles) {
        theta_idx = 0;
    }

    int max_leds = NUM_LEDS;
    if (PAT_RADIUS < NUM_LEDS) {
        max_leds = PAT_RADIUS;
    }

    for (int r = 0; r < max_leds; ++r) {
        uint8_t R = pattern[theta_idx][r][0];
        uint8_t G = pattern[theta_idx][r][1];
        uint8_t B = pattern[theta_idx][r][2];
        ws_set_pixel(r, R, G, B);
    }

    // Turn off extra LEDs if NUM_LEDS > PAT_RADIUS
    for (int r = max_leds; r < NUM_LEDS; ++r) {
        ws_set_pixel(r, 0, 0, 0);
    }

    ws_show();
}

// Push framebuffer to strip (blocking)
void ws_show(void)
{
    // Disable interrupts for up to ~1 ms (0x3FFF instruction cycles).
    __builtin_disi(0x3FFF);

    for (int i = 0; i < NUM_LEDS; ++i) {
        ws_send_pixel(leds[i].r, leds[i].g, leds[i].b);
    }
    ws_reset_latch();

    // Re-enable interrupts
    __builtin_disi(0x0000);
}



// ============================ INIT ============================

void ws_init(void)
{
    // RA0 digital output, start low
    LED_PIN_TRIS = 0;
    LED_PIN_LAT  = 0;

    ws_timing_init();

    ws_clear_all();
    ws_show();   // send "all off" once
}
