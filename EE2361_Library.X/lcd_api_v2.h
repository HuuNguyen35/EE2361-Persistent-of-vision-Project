/* 
 * File:   blommel_lab2b_asmLib_v1.h
 * Author: piano
 *
 * Created on September 24, 2025, 5:22 PM
 */

#ifndef BLOMMEL_LAB4_ASMLIB_V1_H
#define	BLOMMEL_LAB4_ASMLIB_V1_H

#ifdef	__cplusplus
extern "C" {
#endif
    void lcd_cmd(char arg);
    void lcd_init();
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char character);
    void lcd_printStr(const char *str, int delay_ms, char row);


//    void conditionStart(void);
//    void conditionStop(void);
//    int shouldWaitForSending(void);
//    void blockSendByte(char message);
//    void lcd_printStr(char *str);
//    int _str_len(const char *s);
//    void print_window(const char *str, int start);


#ifdef	__cplusplus
}
#endif

#endif	/* BLOMMEL_LAB2B_ASMLIB_V1_H */

