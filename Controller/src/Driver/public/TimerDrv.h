#ifndef TIMERDRV_H
#define TIMERDRV_H

#include <stdint.h>

#ifdef TIMERDRV_DEBUG
#include "MyPrintf.h"
#define TIMERDRV_DEBUG_PRINT(...) MyPrintf(__VA_ARGS__)
#else
#define TIMERDRV_DEBUG_PRINT(...) (void)0
#endif

typedef enum 
{
    TIMERDRV_ID_MAIN = 0,
    TIMERDRV_ID_DEBOUNCE,
    TIMERDRV_ID_COMMANDSENDER,
    TIMERDRV_ID_MAX
} TimerDrvID;

typedef void (*TimerDrvCallback)(void);

void TimerDrv_Init(void);
void TimerDrv_Start(TimerDrvID timer_id, TimerDrvCallback callback, uint32_t interval_ms);
void TimerDrv_Stop(TimerDrvID timer_id);
char* TimerDrv_GetString(TimerDrvID timer_id);

#endif // TIMERDRV_H
