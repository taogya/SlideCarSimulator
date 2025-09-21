#ifndef BUTTONDRV_H
#define BUTTONDRV_H

#include <stdint.h>

#ifdef BUTTONDRV_DEBUG
#include "MyPrintf.h"
#define BUTTONDRV_DEBUG_PRINT(...) MyPrintf(__VA_ARGS__)
#else
#define BUTTONDRV_DEBUG_PRINT(...) (void)0
#endif

typedef enum 
{
    BUTTONDRV_ID_BUTTON_DRV = 0,
    BUTTONDRV_ID_MAX
} ButtonDrvID;

typedef enum
{
    BUTTONDRV_BUTTONID_LEFT = 0,
    BUTTONDRV_BUTTONID_RIGHT,
    BUTTONDRV_BUTTONID_MAX
} ButtonDrvButtonID;

typedef void (*ButtonDrvCallback)(ButtonDrvButtonID button_id);

void ButtonDrv_Init(void);
void ButtonDrv_Enable(ButtonDrvID button_id, ButtonDrvCallback callback);
void ButtonDrv_Disable(ButtonDrvID button_id);
uint8_t ButtonDrv_GetState(ButtonDrvButtonID button_id);
char *ButtonDrv_GetString(ButtonDrvID button_id);
char *ButtonDrv_GetButtonString(ButtonDrvButtonID button_id);
#endif // BUTTONDRV_H