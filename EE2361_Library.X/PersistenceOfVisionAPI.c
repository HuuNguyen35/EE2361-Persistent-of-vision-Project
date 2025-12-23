// PersistenceOfVisionAPI.c
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#define FCY 16000000UL
#include <libpic30.h>

#include "lcd_api_v2.h"
#include "pattern.h"
#include "SensorAPI.h"
#include "LEDAPI.h"
#include "PersistenceOfVisionAPI.h"


#define led_time_on_us      500
#define DEBOUNCE_MS    20

#define TEST_LED_TRIS   TRISBbits.TRISB12
#define TEST_LED_LAT    LATBbits.LATB12
#define TEST_LED_AN     AD1PCFGbits.PCFG12

#define BTN_TRIS        TRISBbits.TRISB15
#define BTN_PORT        PORTBbits.RB15 


static volatile int testLedState = 0;

static uint8_t  theta       = 0;
static uint16_t step_counter = 0;
static uint16_t step_ticks   = 0; //Timer 1 = 1/4us
static uint16_t next_t1      = 0;
static uint8_t  pov_started  = 0;
static uint8_t  sensing_enabled = 0;
static uint8_t pov_enabled = 0;  
static float   g_last_rpm  = 0.0f; 
static uint16_t loops_per_lcd_update = 10000; //10,000 = not fast, good LED




static void initTestLed(){
    TEST_LED_AN  = 1;
    TEST_LED_TRIS = 0; 
    TEST_LED_LAT  = 0;
}

static void testLed_toggle(){
    testLedState ^= 1;
    TEST_LED_LAT = testLedState;
}

static void initButton(){
    BTN_TRIS = 1;
}

static uint8_t button_pressed_edge(){
    static uint8_t prev_state = 1; 
    uint8_t curr = BTN_PORT;

    // Falling edge
    if (prev_state == 1 && curr == 0) {
        __delay_ms(DEBOUNCE_MS);
        curr = BTN_PORT;
        if (curr == 0) {
            prev_state = 0;
            return 1;
        }
    }

    // Rising edge
    if (prev_state == 0 && curr == 1) {
        __delay_ms(DEBOUNCE_MS);
        curr = BTN_PORT;
        if (curr == 1) {
            prev_state = 1;
        }
    }

    return 0;
}


static void pov_t1_init(){
    T1CON = 0;
    TMR1  = 0;

    T1CONbits.TCKPS = 0b10;   // 1:64 prescale = 4 us per tick
    T1CONbits.TCS   = 0;
    T1CONbits.TON   = 1; 
}

// ============PUBLIC API ===========================
void POV_Start(){
    /*
     Arguments: Void
Function: Makes the systems start running. Separate from initialization.
     * This flips state variables to tell the PersistenceOfVisionTask to start 
     * the infrared sensor sampling for the RPM timing. This also allows for 
     * the LEDs to display. Used to start the system.
Return: Void

     */
    if (!sensing_enabled) {
        sensor_rpm_init();
        sensing_enabled = 1;
    }
    pov_enabled = 1;
    pov_started = 0;
}


void POV_Stop(){
    /*
     * Arguments: Void
Function: Stops the infrared sensor from updating the RPM value. 
     * Also prevents the LEDs from displaying anything. Used to stop the system.
Return: Void

     */
    if (sensing_enabled) {
        sensor_rpm_shutdown();
        sensing_enabled = 0;
    }

    pov_enabled = 0;
    pov_started = 0;
    step_ticks  = 0;

    ws_clear_all();
    ws_show();
}

void POV_Stop_LED_updates(){
    /*
     Arguments: Void
Function: When calling the PersistenceOfVision_Task(), it will no longer flash
     *  the LEDs with the latest theta value. The purpose is to allow for the
     *  LEDs to be controlled by the POV_SetStaticColor() function while the
     *  sensor is still able to calculate the RPM. To re-enable the LED 
     *  updates, call POV_Start().
Return: Void
*/
    pov_enabled = 0;
    pov_started = 0;
    step_ticks  = 0;

    ws_clear_all();
    ws_show();
}

float POV_GetRPM(){
    /*
     Arguments: Void
Function: This function allows us to easily access the latest and most recent
     *  rpm from the sensor.
Return: float = the average RPM gathered from the sensor?s circular buffer.

     */
    return g_last_rpm;
}

void POV_SetStaticColor(uint8_t r, uint8_t g, uint8_t b){
    /*
     Arguments: uint8_t r, uint8_t g, uint8_t b
Function: This function writes every accessible LED into the same color and
     * then writes that color value into ws_show() so that the LEDs can update. 
     * This function is useful for test cases.
Return: Void
*/
    ws_set_all(r, g, b);
    ws_show();
}

void POV_ClearDisplay(){
    /*
     Arguments: Void
Function: This function clears the color of every accessible LED. It then sends 
     * that data to ws_show() so the LEDs can turn off. This function is useful
     *  for test case.
Return: Void
*/
    ws_clear_all();
    ws_show();
}

void POV_SetLcdUpdate(uint16_t newNum){
    /*
     Arguments: uint16_t newNum
Function: This function allows us to write any number of loops that it takes
     *  for the LCD display to update. This is useful as too many loops can
     *  cause the LCD display to update slowly and unreliably and too little 
     * loops can cause the LCD display to be more ?janky?.
Return: Void
*/
    loops_per_lcd_update = newNum;
}


void InitPersistenceOfVision(){
    /*
     Arguments: Void
Function: This initializes all components necessary for the POV module. 
     * It runs through each of the required software init() functions and 
     * initializes all persistent variables to their respective values. 
Return: Void

     */
    pov_t1_init();
    
    lcd_init(); 
    ws_init(); 

    //optinal for debugging
    initTestLed();
    initButton();

    theta        = 0;
    step_counter = 0;
    step_ticks   = 0;
    next_t1      = 0;
    pov_started  = 0;
    loops_per_lcd_update = 10000;
}

//while(1) do this function
void PersistenceOfVision_Task(){
    /*
     Arguments: Void
Function: This is the core logic that should be called infinitely. 
     * This is the main driver function for our module. It controls when to
     *  call each of the sub-functions, and keeps track and updates the state 
     * variables as needed. It first creates local variables for each call that
     *  represent the LCD?s display and the current state of the system. 
     * It checks if the user has pressed the ?start? button. If unpressed, the 
     * only logic the function will perform is displaying ?PRESS BTN? ?TO START?
     *  on the LCD display. If the button has been pressed, the following logic
     *  will be performed. The average RPM from the sensor?s circular buffer is
     *  gathered. We use that value to convert into Timer 1 ticks per LED 
     * state, and calculate the time in between each LED flash. Then the RPM
     *  information is sent to the LCD to be displayed. Then it checks if 
     * it?s time for an LED flash. If it?s time, the LEDs are blinked on for 
     * a certain amount of time, then turned off. The theta value is 
     * incremented and we schedule the time for the next LED flash based on 
     * the most recent amount of ticks per flash.
Return: Void
*/
    char top_buf[11];
    char bot_buf[11];

    
    uint32_t hits   = sensor_get_hit_count();
    uint8_t  buf_n  = sensor_get_buffer_count();
    uint8_t  buf_ok = sensor_buffer_is_full();

    float rpm_avg = 0.0f;
    float step_us_f = 0.0f;
    
    if (button_pressed_edge()) {
        testLed_toggle();
        POV_Start();
    }

    
    if (!sensing_enabled) {
        if ((step_counter++ % 200) == 0) {
            lcd_setCursor(0, 0);
            snprintf(top_buf, sizeof(top_buf), "PRESS BTN");
            for (int i = 0; top_buf[i] != '\0'; i++) {
                lcd_printChar(top_buf[i]);
            }

            lcd_setCursor(0, 1);
            snprintf(bot_buf, sizeof(bot_buf), "TO START ");
            for (int i = 0; bot_buf[i] != '\0'; i++) {
                lcd_printChar(bot_buf[i]);
            }
        }

        __delay_ms(20);
        return;//Skip the sensor and led stuff
    }


    if (buf_ok) {//if buffer is full, calculate the RPM
        rpm_avg = sensor_get_rpm();
        g_last_rpm = rpm_avg;   

        if (used_angles > 0 && rpm_avg > 0.1f) {
            // T_rev_s = 60 / RPM
            float T_rev_s  = 60.0f / rpm_avg;
            float T_step_s = T_rev_s / (float)used_angles;
            step_us_f      = T_step_s * 1.0e6f; //put into us

            
            float ticks_f = step_us_f / 4.0f;//how many Timer1 ticks
            if (ticks_f < 1.0f)     ticks_f = 1.0f;
            if (ticks_f > 65000.0f) ticks_f = 65000.0f;
            step_ticks = (uint16_t)ticks_f;
        } else {//safety for if buffer isn't fill
            step_us_f  = 0.0f;
            step_ticks = 0;
        }
    }

    //less frequently we gotta update the LCD to whatever the RPM is
    if ((step_counter++ % loops_per_lcd_update) == 0 ) {
        //top
        lcd_setCursor(0, 0);
        snprintf(
            top_buf,
            sizeof(top_buf),
            "%4lu %2u/%u",
            (unsigned long)hits,
            (unsigned)buf_n,
            (unsigned)getBufferSize()
        );
        for (int i = 0; top_buf[i] != '\0'; i++) {
            lcd_printChar(top_buf[i]);
        }

        //Bottom = Filling or the RPM
        lcd_setCursor(0, 1);
        
        if (buf_ok && rpm_avg > 0.1f) {
            snprintf(bot_buf, sizeof(bot_buf), "%7.1fRPM", (double)rpm_avg);
        } 
        else {
            snprintf(bot_buf, sizeof(bot_buf), "  FILLING ");
        }
        
        for (int i = 0; bot_buf[i] != '\0'; i++) {
            lcd_printChar(bot_buf[i]);
        }
    }

    

    //LED logic
    if (pov_enabled && buf_ok && step_us_f > 0.0f) {
        uint16_t now = TMR1;

        if (!pov_started) {
            pov_started = 1;
            next_t1     = now;
        }
        
        uint16_t elapsed = (uint16_t)(now - next_t1);

        if (elapsed >= step_ticks) {
            ws_flash_column(theta, led_time_on_us); 

            theta++;
            if (theta >= used_angles) {
                theta = 0;
            }

            next_t1 += step_ticks;
        }
        //no delay if the leds are running
    } 
    else {
        //if the RPM isn't ready yet we can chill for a bit
        __delay_ms(1);
    }
}
