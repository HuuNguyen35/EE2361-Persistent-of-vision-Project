/* 
 * File:   LEDAPI.h
 * Author: piano
 *
 * Created on November 17, 2025, 8:59 PM
 */

#ifndef LEDAPI_H
#define	LEDAPI_H

#ifdef	__cplusplus
extern "C" {
#endif


#include <stdint.h>

void ws_init(void);
void ws_clear_all(void);
void ws_set_all(uint8_t r, uint8_t g, uint8_t b);
void ws_set_pixel(int idx, uint8_t r, uint8_t g, uint8_t b);
void ws_show(void);
void ws_show_column(uint8_t theta_idx); 

void ws_flash_column(uint8_t theta_idx, uint16_t on_time_us);



#ifdef	__cplusplus
}
#endif

#endif	/* LEDAPI_H */

