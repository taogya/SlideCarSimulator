#include "TimerDrv.h"
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct
{
    TimerDrvCallback callback;
    uint32_t interval_ms;
} TimerDrvEvent;

static TimerDrvEvent TimerDrvEvents[TIMERDRV_ID_MAX];
static uint8_t TimerDrvIsInitialized = 0;

static void TimerSignalHandler(int signum)
{
    static uint32_t tick_count = 0;
    tick_count++;

    for (int i = 0; i < TIMERDRV_ID_MAX; i++)
    {
        if (TimerDrvEvents[i].callback != NULL && TimerDrvEvents[i].interval_ms > 0)
        {
            if (tick_count % TimerDrvEvents[i].interval_ms == 0)
            {
                TIMERDRV_DEBUG_PRINT("TimerDrv: Timer '%s' expired at tick %u\n", TimerDrv_GetString((TimerDrvID)i), tick_count);
                
                TimerDrvEvents[i].callback();
            }
        }
    }
}

void TimerDrv_Init(void)
{
    if (!TimerDrvIsInitialized)
    {
        for (int i = 0; i < TIMERDRV_ID_MAX; i++)
        {
            TimerDrvEvents[i].callback = NULL;
            TimerDrvEvents[i].interval_ms = 0;
        }

        /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
        struct sigaction sa;
        sa.sa_handler = TimerSignalHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        (void)sigaction(SIGALRM, &sa, NULL);
        struct itimerval timer;
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 1000; // 1ms
        timer.it_interval = timer.it_value;
        (void)setitimer(ITIMER_REAL, &timer, NULL);
        /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */

        TimerDrvIsInitialized = 1;
        TIMERDRV_DEBUG_PRINT("TimerDrv initialized.\n");
    }
    else {

        TIMERDRV_DEBUG_PRINT("TimerDrv already initialized.\n");
    }
}

void TimerDrv_Start(TimerDrvID timer_id, TimerDrvCallback callback, uint32_t interval_ms)
{
    if (TimerDrvIsInitialized && 0 <= timer_id && timer_id < TIMERDRV_ID_MAX && callback != NULL && interval_ms > 0)
    {
        TimerDrvEvents[timer_id].callback = callback;
        TimerDrvEvents[timer_id].interval_ms = interval_ms;

        TIMERDRV_DEBUG_PRINT("TimerDrv: Timer '%s' started with interval %u ms\n", TimerDrv_GetString(timer_id), interval_ms);
    }
}

void TimerDrv_Stop(TimerDrvID timer_id)
{
    if (TimerDrvIsInitialized && 0 <= timer_id && timer_id < TIMERDRV_ID_MAX)
    {
        TimerDrvEvents[timer_id].callback = NULL;
        TimerDrvEvents[timer_id].interval_ms = 0;

        TIMERDRV_DEBUG_PRINT("TimerDrv: Timer '%s' stopped\n", TimerDrv_GetString(timer_id));
    }
}

char* TimerDrv_GetString(TimerDrvID timer_id)
{
    switch (timer_id)
    {
        case TIMERDRV_ID_MAIN:
            return "TIMERDRV_ID_MAIN";
        case TIMERDRV_ID_DEBOUNCE:
            return "TIMERDRV_ID_DEBOUNCE";
        default:
            return "TIMERDRV_ID_UNKNOWN";
    }
}