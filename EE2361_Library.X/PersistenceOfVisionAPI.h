/* 
 * File:   PersistenceOfVisionAPI.h
 * Author: piano
 *
 * Created on December 7, 2025, 7:12 PM
 */

#ifndef PERSISTENCEOFVISIONAPI_H
#define	PERSISTENCEOFVISIONAPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    void InitPersistenceOfVision();
    void PersistenceOfVision_Loop();
    void POV_Start(void);
    void POV_Stop(void);
    float POV_GetRPM(void);
    void POV_SetStaticColor(uint8_t r, uint8_t g, uint8_t b);
    void POV_ClearDisplay(void);
    void POV_SetLcdUpdate(uint16_t newNum);


#include "PersistenceOfVisionAPI.h"


#ifdef	__cplusplus
}
#endif

#endif	/* PERSISTENCEOFVISIONAPI_H */

