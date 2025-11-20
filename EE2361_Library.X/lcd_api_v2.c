

//#include "lcd.h"

#include <xc.h>
#define FCY 16000000UL
//#include "blommel_lab4_asmLib_v1.h"
#include <libpic30.h>



#define FUNCTIONSET_2LINE   0x38
#define ENTRY_MODE_INC      0x06
#define DISPLAY_ON          0x0C
#define CLEAR_DISPLAY       0x01

#define BAUD_RATE       0x009D
#define COMMAND         0b00000000
#define DATA            0b01000000

#define LCDaddy         0x3C
#define LCDaddy_writ    (LCDaddy << 1) & 0b11111110
#define LCDaddy_read    (LCDaddy << 1) | 0b00000001

#define LCD_COLS        10


void lcd_init(){
    /*Goal: To initialize the DOGS104-A LCD. It sets up the 
     * I2C pins, and initializes the I2C connection.
     * This function also handles the initialization and formatting
     * of the LCD display, such as setting the font size,
     * power control, and contrast
     * 
     * Parameters: void
     * 
     * Return: void
     */
    
    TRISBbits.TRISB8 = 1;   //SCL1
    TRISBbits.TRISB9 = 1;   //SDA1
    
    //reset pin as output and pulse
    TRISBbits.TRISB6 = 0;
    __delay_ms(1);
    
    LATBbits.LATB6 = 0;
    __delay_ms(10);
    LATBbits.LATB6 = 1;
    __delay_ms(10);
    
    
    //config i2c
    I2C1CONbits.I2CEN = 0;
    
    I2C1BRG = BAUD_RATE;
    I2C1CONbits.DISSLW = 1;
    _MI2C1IF = 0;
    _MI2C1IE = 0;
    
    I2C1CONbits.I2CEN = 1;

    lcd_cmd(0x3A);  //function set
    lcd_cmd(0x09);  //extended function set 4-line mode
    lcd_cmd(0x06);  //entry mode (0x05 for top view)
    lcd_cmd(0x1E);  //bias setting
    lcd_cmd(0x39);  // function set (re=0 is=1)
    lcd_cmd(0x1B);  // internal OSC frequency
    lcd_cmd(0x6E);  //follower control
    lcd_cmd(0x56);  //power control, contrast high bits =c5:c4
    lcd_cmd(0x7A);  // contrast low bits
    lcd_cmd(0x38);  //function set (re=0, is=0) normal
    lcd_cmd(0x0c);  //display on, cursor+blink=0x0F 
    lcd_cmd(0x01);  //clear display
    lcd_cmd(0x3a);  //function set
    lcd_cmd(0x09);  //extended function set 4-line mode
    lcd_cmd(0x1a);  // bias 
    lcd_cmd(0x3c);  //finalize setup
    
    __delay_ms(2);
}


void conditionStart(){
    /*start sending an i2c command
     */
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN);
}

void conditionStop(){
    /*end i2c communciation
     */
    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN);
}

int shouldWaitForSending(){
    // return 1 if should wait, 0 if should stop waiting
    return (_MI2C1IF != 1) || (I2C1STATbits.TRSTAT == 1);
}

void blockSendByte(char message){
    /*blocking, send a byte thorugh i2c
     */
    _MI2C1IF = 0;
    I2C1TRN = message;
    while(shouldWaitForSending());
}

void lcd_cmd(char arg){
    /*  Goal: to send one full I2C command message. This includes
     * a start sequence, address, command, the data, and
     * a stop sequance
     * 
     *  Parameters: char args = the message
     * in the data byte to be sent to the lcd
     * 
     *  Return: void
     */
    conditionStart();
    
    blockSendByte(LCDaddy_writ);
    blockSendByte(COMMAND);
    blockSendByte(arg);
    
    conditionStop();
}


void lcd_printChar(char character){
    /*  Goal: to send one full I2C message holding 
     * a data byte for the lcd. This includes
     * a start sequence, address, command, the data, and
     * a stop sequance
     * 
     *  Parameters: char character = the character
     * sent to and displayed by the lcd
     * 
     *  Return: void
     */
    conditionStart();
    
    blockSendByte(LCDaddy_writ);
    blockSendByte(DATA);
    blockSendByte(character);
    
    conditionStop();
    
}


void lcd_setCursor(char x, char y){
    /*  Goal: to change the location of the cursor. This
     * will calculate what the data byte should be based
     * on the x and y positions determined by the
     * arguments, and will send that to the lcd
     * 
     *  Parameters:
     *  char x = the x position the cursor should move to.
     * This is 0 indexed so it starts counting at 0
     *  char y = the y position the cursor should move to.
     * This is 0 indexed so it starts counting at 0
     * 
     *  Return: void
     */
    
    char position = 0x00;
    
    if (y == 0x01){
        position = 0x20;
    }
    
    position += x;
    
    position |= 0b10000000;
    
    char controlByte = 0x00;
    
    conditionStart();
    
    blockSendByte(LCDaddy_writ);
    blockSendByte(controlByte);
    blockSendByte(position);
    
    conditionStop();
}

int _str_len(const char *s) {
    if (!s) return 0;
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}


void print_window(const char *str, int start) {
    int i;
    const char *p = str;
    
    int skip = start;
    while (skip > 0 && *p != '\0') {
        p++;
        skip--;
    }

    
    for (i = 0; i < LCD_COLS; i++) {
        if (*p == '\0') {
            lcd_printChar(' '); 
        } else {
            lcd_printChar(*p);
            p++;
        }
    }
}



void lcd_printStr(const char *str, int delay_ms, char row) {
    /*  Goal: to create an animated display that cycles throug
     * all charcters in a string. This should look like a 
     * string of characters moving across the screen like a 
     * moving display. This is a blocking function, so
     * no other function can run without interrupting this one
     * 
     *  Parameters:
     *  const char *str = the pointer to an "array" of 
     * characters the user wishes to be sent to the lcd
     * for an animated/moving display
     *  int delay_ms = the time in milliseconds between
     * frames of the display
     *  char row = the rown number the user wishes the string
     * to be displayed on. 
     * 
     *  Return: void
     * 
     */
    
    
    int len = _str_len(str);
    int offset = 0;
    
    while (offset <= len) {
        
        
        lcd_setCursor(0, row);
        print_window(str, offset);
        
        offset++;

        __delay_ms(delay_ms);
    }
}


