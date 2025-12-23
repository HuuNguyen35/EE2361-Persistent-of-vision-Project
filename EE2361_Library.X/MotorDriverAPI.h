/* 
 * File:   MotorDriverAPI.h
 * Author: piano
 *
 * Created on November 17, 2025, 8:59 PM
 */

#ifndef MOTORDRIVERAPI_H
#define	MOTORDRIVERAPI_H

#ifdef	__cplusplus
extern "C" {
#endif


//    void pwm_init(void);
//    void pps_init(void);
//    void pwm_set_duty(uint16_t duty);
    void motor_init(void);
    void motor_set_direction(uint8_t dir);
    void motor_set_speed_percent(uint8_t speed);


#ifdef	__cplusplus
}
#endif

#endif	/* MOTORDRIVERAPI_H */

