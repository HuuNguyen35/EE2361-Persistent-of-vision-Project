/* 
 * File:   pattern.h
 * Author: piano
 *
 * Created on November 19, 2025, 6:59 PM
 */

#ifndef PATTERN_H
#define	PATTERN_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Must match the defines at the top of pattern.c
#define PAT_RADIUS 5
#define PAT_ANGLES 4

extern const uint8_t pattern[PAT_ANGLES][PAT_RADIUS][3];

#ifdef	__cplusplus
}
#endif

#endif	/* PATTERN_H */

