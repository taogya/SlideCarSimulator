#include "ButtonDrv.h"
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

static ButtonDrvCallback gButtonDrv_Callbacks[BUTTONDRV_ID_MAX] = {0};
static uint8_t gButtonDrvIsInitialized = 0;

static void ButtonDrv_InterruptHandler(ButtonDrvButtonID button_id)
{
    for(int i = 0; i < BUTTONDRV_ID_MAX; i++)
    {
        if (gButtonDrv_Callbacks[i] != NULL)
        {
            BUTTONDRV_DEBUG_PRINT("ButtonDrv: Button ID %s triggered callback for %s\n", 
                ButtonDrv_GetButtonString(button_id), 
                ButtonDrv_GetString((ButtonDrvID)i));
            gButtonDrv_Callbacks[i](button_id);
        }
    }
}

/* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
static pthread_t gButtonDrvInputThread;
static int gButtonDrvThreadRunning = 0;
static int gButtonDrvCurrentChar = '\0';
static void* ButtonDrv_InputThread(void* arg)
{
    gButtonDrvThreadRunning = 1;
    while (gButtonDrvThreadRunning) {
        int c = getchar();
        if (c == EOF || c == '\n') continue;

        gButtonDrvCurrentChar = c;
        if (c == 'L' || c == 'l')
        {
            ButtonDrv_InterruptHandler(BUTTONDRV_BUTTONID_LEFT);
        }
        else if (c == 'R' || c == 'r')
        {
            ButtonDrv_InterruptHandler(BUTTONDRV_BUTTONID_RIGHT);
        }
        else
        {
            BUTTONDRV_DEBUG_PRINT("ButtonDrv: Ignored input '%c'\n", c);
        }
    }
    return NULL;
}
/* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */

void ButtonDrv_Init(void)
{
    if (!gButtonDrvIsInitialized)
    {
        for (int i = 0; i < BUTTONDRV_ID_MAX; i++)
        {
            gButtonDrv_Callbacks[i] = NULL;
        }

        /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
        // キーボード入力監視スレッド起動
        (void)pthread_create(&gButtonDrvInputThread, NULL, ButtonDrv_InputThread, NULL);
        BUTTONDRV_DEBUG_PRINT("ButtonDrv: Input thread started.\n");
        /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */

        // set mouse interrupt handler
        gButtonDrvIsInitialized = 1;
        BUTTONDRV_DEBUG_PRINT("ButtonDrv initialized.\n");
    }
    else
    {
        BUTTONDRV_DEBUG_PRINT("ButtonDrv already initialized.\n");
    }
}

void ButtonDrv_Enable(ButtonDrvID button_id, ButtonDrvCallback callback)
{
    if (gButtonDrvIsInitialized && 0 <= button_id && button_id < BUTTONDRV_ID_MAX)
    {
        gButtonDrv_Callbacks[button_id] = callback;

        BUTTONDRV_DEBUG_PRINT("ButtonDrv: Button %s enabled\n", ButtonDrv_GetString(button_id));
    }
}

void ButtonDrv_Disable(ButtonDrvID button_id)
{
    if (gButtonDrvIsInitialized && 0 <= button_id && button_id < BUTTONDRV_ID_MAX)
    {
        gButtonDrv_Callbacks[button_id] = NULL;

        BUTTONDRV_DEBUG_PRINT("ButtonDrv: Button %s disabled\n", ButtonDrv_GetString(button_id));
    }
}

uint8_t ButtonDrv_GetState(ButtonDrvButtonID button_id)
{
    switch(button_id)
    {
        /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
        case BUTTONDRV_BUTTONID_LEFT:
            return gButtonDrvCurrentChar == 'L' ? 1 : 0;
        case BUTTONDRV_BUTTONID_RIGHT:
            return gButtonDrvCurrentChar == 'R' ? 1 : 0;
        /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */
        default:
            return 0;
    }
}

char* ButtonDrv_GetString(ButtonDrvID button_id)
{
    switch (button_id)
    {
        case BUTTONDRV_ID_BUTTON_DRV:
            return "BUTTONDRV_ID_BUTTON_DRV";
        default:
            return "BUTTONDRV_ID_UNKNOWN";
    }
}

char* ButtonDrv_GetButtonString(ButtonDrvButtonID button_id)
{
    switch (button_id)
    {
        case BUTTONDRV_BUTTONID_LEFT:
            return "BUTTONDRV_BUTTONID_LEFT";
        case BUTTONDRV_BUTTONID_RIGHT:
            return "BUTTONDRV_BUTTONID_RIGHT";
        default:
            return "BUTTON_ID_UNKNOWN";
    }
}