/*
 * WS2812 LED strip driver using Isaac's assembly timing routines.
 *
 * - Uses write_0(), write_1(), hundredMicroSecond(), oneMilliSecond()
 *   from blommel_lab2b_asmLib_v1.h for precise timing.
 * - Keeps a framebuffer of NUM_LEDS Pixel structs.
 * - Sends GRB data for each LED in sequence, then a >50us reset.
 *
 * Data pin: RA0  (same as your original working code)
 */

#include <xc.h>
#include <stdint.h>

#define FCY 16000000UL
#include <libpic30.h>


#include "pattern.h"   // brings in PAT_RADIUS, PAT_ANGLES, pattern

#include "blommel_lab2b_asmLib_v1.h"   // write_0, write_1, hundredMicroSecond, oneMilliSecond :contentReference[oaicite:2]{index=2}

// ============================ CONFIG BITS ============================
// CW1: FLASH CONFIGURATION WORD 1
#pragma config ICS    = PGx1      // Debug on PGC1/PGD1
#pragma config FWDTEN = OFF       // Watchdog Timer disabled
#pragma config GWRP   = OFF       // General Code Segment Write Protect off
#pragma config GCP    = OFF       // General Code Segment Code Protect off
#pragma config JTAGEN = OFF       // JTAG disabled

// CW2: FLASH CONFIGURATION WORD 2
#pragma config I2C1SEL  = PRI     // Default SCL1/SDA1 pins
#pragma config IOL1WAY  = OFF     // IOLOCK may be changed
#pragma config OSCIOFNC = ON      // CLKO/RC15 as I/O
#pragma config FCKSM    = CSECME  // Clock switching enabled, Fail-Safe Monitor enabled
#pragma config FNOSC    = FRCPLL  // Fast RC Oscillator with PLL (FRCPLL)

// ============================ LED CONFIG ============================

#define NUM_LEDS  5

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

static Pixel leds[NUM_LEDS];

// Use RA0 as the WS2812 data pin (matches your original code)
#define LED_PIN_TRIS   TRISAbits.TRISA0
#define LED_PIN_LAT    LATAbits.LATA0

#define BTN_TRIS   TRISBbits.TRISB15
#define BTN_PORT   PORTBbits.RB15

void ws_show(void);
// ============================ LOW-LEVEL SEND USING ASM ============================

// Send one byte MSB-first using your write_0 / write_1 assembly routines.
// Each call to write_0() or write_1() is responsible for:
//
//   - Driving RA0 high
//   - Holding it for the correct T0H/T1H
//   - Pulling it low and holding for T0L/T1L
//
// so from C we just choose which function to call per bit.
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

// WS2812 expects GRB, MSB first.
// If your assembly code previously used RGB order, you can swap here
// by changing the call order.
static inline void ws_send_pixel(uint8_t r, uint8_t g, uint8_t b)
{
    ws_send_byte(g);  // G
    ws_send_byte(r);  // R
    ws_send_byte(b);  // B
}

// After sending all bits, hold the line low for >= 50us to latch.
static inline void ws_reset_latch(void)
{
    LED_PIN_LAT = 0;
    hundredMicroSecond();   // your asm routine (~100us)
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

// Display one angular column (theta index) from the pattern onto the strip.
// Assumes NUM_LEDS == PAT_RADIUS (or uses the min of both).
void ws_show_column(uint8_t theta_idx)
{
    if (theta_idx >= PAT_ANGLES) {
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

    // If NUM_LEDS > PAT_RADIUS, turn the extra LEDs off
    for (int r = max_leds; r < NUM_LEDS; ++r) {
        ws_set_pixel(r, 0, 0, 0);
    }

    ws_show();
}

// Push the framebuffer to the strip.
// This is blocking (like your original code), but the timing is handled
// entirely in assembly.
void ws_show(void)
{
    for (int i = 0; i < NUM_LEDS; ++i) {
        ws_send_pixel(leds[i].r, leds[i].g, leds[i].b);
    }
    ws_reset_latch();
}

// ============================ INIT + DEMO MAIN ============================

static void ws_init(void)
{
    // Make sure FRCPLL is running at full speed (RCDIV = 0). :contentReference[oaicite:3]{index=3}
    CLKDIVbits.RCDIV = 0;

    // All analog pins digital so RA0 behaves as a digital output
    AD1PCFG = 0xFFFF;

    // Configure RA0 as output, start low
    LED_PIN_TRIS = 0;
    LED_PIN_LAT  = 0;

    ws_clear_all();
    ws_show();   // send "all off" once
}

static void button_init(void)
{
    BTN_TRIS = 1;  // input
    // If you have internal pull-ups, enable them here; otherwise external resistor
}

int main(void)
{
    ws_init();
    button_init();

    uint8_t theta = 0;
    int last_btn = BTN_PORT;

    // Start at column 0
    ws_show_column(theta);
    ws_clear_all();

    while (1) {
        int cur_btn = BTN_PORT;

        // Detect falling edge: 1 -> 0
        if (last_btn == 1 && cur_btn == 0) {
            // Advance to next angle
            theta++;
            if (theta >= PAT_ANGLES) {
                theta = 0;
            }

            ws_show_column(theta);

            // crude debounce
            __delay_ms(150);
        }

        last_btn = cur_btn;
    }

    return 0;
}

//
//int main(void)
//{
//    ws_init();
//
//    while (1) {
//
//        // Red
//        ws_set_all(5, 0, 0);
//        ws_show();
//        __delay_ms(2000);
//
//        // Green
//        ws_set_all(0, 5, 0);
//        ws_show();
//        __delay_ms(2000);
//
//        // Blue
//        ws_set_all(0, 0, 5);
//        ws_show();
//        __delay_ms(2000);
//
//        // White
//        ws_set_all(5, 5, 5);
//        ws_show();
//        __delay_ms(2000);
//
//        // Off
//        ws_set_all(0, 0, 0);
//        ws_show();
//        __delay_ms(2000);
//    }
//
//    return 0;
//}
