#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#define FCY 16000000UL
#include <libpic30.h>

#include "lcd_api_v2.h"
#include "pattern.h"       // gives used_angles, PAT_ANGLES, etc.
#include "SensorAPI.h"
#include "LEDAPI.h"

#define LED_ON_US       500
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



int main(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFF;

    lcd_init();
    sensor_rpm_init();
    ws_init();

    uint8_t theta = 0;
    char top_buf[16];
    char bot_buf[16];

    // We?ll update the LCD less frequently to avoid spamming it.
    uint16_t step_counter = 0;

    while (1) {
        // --- Read sensor state ---
        uint32_t hits   = sensor_get_hit_count();
        uint8_t  buf_n  = sensor_get_buffer_count();
        uint8_t  buf_ok = sensor_buffer_is_full();

        uint32_t avg_ticks = 0;
        float    rpm_avg   = 0.0f;
        float    step_us_f = 0.0f;

        if (buf_ok) {
            avg_ticks = sensor_get_average_delta_ticks();
            rpm_avg   = sensor_rpm_from_ticks(avg_ticks);

            // time for 1 revolution in seconds
            float T_rev_s = sensor_ticks_to_seconds(avg_ticks);

            if (used_angles > 0 && rpm_avg > 0.1f) {
                float T_step_s = T_rev_s / (float)used_angles;
                step_us_f = T_step_s * 1.0e6f;
            } else {
                step_us_f = 0.0f;
            }
        }

        // --- Update LCD every N steps ---
        if ((step_counter++ % 20) == 0) {
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
            if (step_us_f < 500.0f)   step_us_f = 500.0f;     // don't go crazy fast
//            if (step_us_f > 50000.0f) step_us_f = 50000.0f;   // cap at 50 ms/step

            uint32_t step_us = (uint32_t)step_us_f;

            // One angular step:
            // ws_show() internally disables interrupts during the bitstream,
            // so we can just call ws_flash_column here.
            ws_flash_column(theta, LED_ON_US);   // 500 us ON; tweak if needed

            theta++;
            if (theta >= used_angles) {
                theta = 0;
            }

            // Wait until it?s time for the next angular position.
            // Interrupts are enabled during this delay, so sensor keeps updating.
            delay_us_32(step_us);
        } else {
            // Buffer not full or RPM too small ? just idle a bit.
            __delay_ms(50);
        }
    }

    return 0;
}
