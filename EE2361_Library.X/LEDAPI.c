#include <xc.h>
#include <stdint.h>

#define FCY 16000000UL
#include <libpic30.h>

#include "pattern.h"
#include "blommel_lab2b_asmLib_v1.h"

#define LED_RADIUS  14
//#define NUM_LEDS   14
#define POV_MODE_RADIUS    0
#define POV_MODE_DIAMETER  1

#define POV_MODE   POV_MODE_RADIUS

#if POV_MODE == POV_MODE_RADIUS
#define NUM_LEDS    LED_RADIUS
#elif POV_MODE == POV_MODE_DIAMETER
#define NUM_LEDS    LED_RADIUS * 2
#endif

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




static inline void ws_send_pixel(uint8_t r, uint8_t g, uint8_t b)
{//simple kinda useless wrapper to switch to GRB values
    ws_send_pixel_asm(g, r, b);  //GRB order
}




//use after sending all LEDS their code
static inline void ws_reset_latch(void)
{
    LED_PIN_LAT = 0;
    __delay_us(80); //above 50 us
}


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
void ws_flash_column(uint8_t theta_idx, uint16_t on_time_us)
{
    // Show pattern for this angle
    ws_show_column(theta_idx);

    __delay_us(on_time_us);

    // Turn strip off
    ws_clear_all();
    ws_show();
}


void ws_show_column(uint8_t theta_idx)
{
    if (theta_idx >= used_angles) {
        theta_idx = 0;
    }

#if POV_MODE == POV_MODE_RADIUS

    // Radius-only: just light along one side of the strip
    int max_leds = NUM_LEDS;
    if (PAT_RADIUS < NUM_LEDS) {
        max_leds = PAT_RADIUS;
    }

    // First clear everything (avoids stale pixels if pattern shrinks)
    for (int i = 0; i < NUM_LEDS; ++i) {
        ws_set_pixel(i, 0, 0, 0);
    }

    // Use pattern[theta_idx][r] along the strip
    for (int r = 0; r < max_leds; ++r) {
        uint8_t R = pattern[theta_idx][r][0];
        uint8_t G = pattern[theta_idx][r][1];
        uint8_t B = pattern[theta_idx][r][2];
        ws_set_pixel(r, R, G, B);
    }

#elif POV_MODE == POV_MODE_DIAMETER

    int half_strip = NUM_LEDS / 2;
    int radius = PAT_RADIUS;
    
    if (radius > half_strip) {//safety
        radius = half_strip;
    }

    // Opposite angle index = (theta + 180) with wrapping
    uint8_t theta_opp = theta_idx + (used_angles / 2);
    if (theta_opp >= used_angles) {
        theta_opp -= used_angles; 
    }

    for (int i = 0; i < NUM_LEDS; ++i) {
        ws_set_pixel(i, 0, 0, 0);
    }
    
    //Front
    for (int r = 0; r < radius; ++r) {
        uint8_t R = pattern[theta_idx][r][0];
        uint8_t G = pattern[theta_idx][r][1];
        uint8_t B = pattern[theta_idx][r][2];
        ws_set_pixel(r, R, G, B);
    }

    //Opposite
    for (int r = 0; r < radius; ++r) {
        uint8_t R = pattern[theta_opp][radius - r - 1][0];
        uint8_t G = pattern[theta_opp][radius - r - 1][1];
        uint8_t B = pattern[theta_opp][radius - r - 1][2];

        int idx = half_strip + r;
        if (idx < NUM_LEDS) {
            ws_set_pixel(idx, R, G, B);
        }
    }

#endif

    ws_show();
}



void ws_show(void)
{
    //nothing gonna fuck up my led timing
    __builtin_disi(0x3FFF);

    for (int i = 0; i < NUM_LEDS; ++i) {
        ws_send_pixel(leds[i].r, leds[i].g, leds[i].b);
    }
    
    __builtin_disi(0x0000);
    
    ws_reset_latch();
}





void ws_init(void)
{
    LED_PIN_TRIS = 0;
    LED_PIN_LAT  = 0;

    ws_clear_all();
    ws_show();
}
