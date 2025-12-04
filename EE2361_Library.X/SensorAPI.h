/* 
 * File:   SensorAPI.h
 * Author: piano
 *
 * Created on November 17, 2025, 8:59 PM
 */

#ifndef SENSORAPI_H
#define	SENSORAPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define N_SAMPLES 10
    
void     sensor_rpm_init(void);

uint32_t sensor_get_latest_delta_ticks(void);
uint32_t sensor_get_hit_count(void);

float    sensor_ticks_to_seconds(uint32_t ticks);
float    sensor_rpm_from_ticks(uint32_t ticks);

// --- New circular-buffer helpers ---

// How many valid samples are currently in the buffer (0..N_SAMPLES).
uint8_t  sensor_get_buffer_count(void);

// Has the buffer reached its full size (N_SAMPLES)?
uint8_t  sensor_buffer_is_full(void);

// Average period (in Timer2 ticks) over the current buffer contents.
// Returns 0 if no valid samples yet.
uint32_t sensor_get_average_delta_ticks(void);
void sensor_rpm_shutdown(void);
float sensor_get_filtered_rpm(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SENSORAPI_H */

