#include <stdint.h>
#include "Button.h"
#include "Button_API.h"
#include "ButtonDrv.h"
#include "TimerDrv.h"

static Button gButton_sm = {0};

static void Button_InterruptHandler(ButtonDrvButtonID button_id);
static void Button_TimerHandler(void);

void Button_Init(void)
{
    Button_ctor(&gButton_sm);
    gButton_sm.vars.button_callback = Button_InterruptHandler;
    gButton_sm.vars.timer_callback = Button_TimerHandler;
    Button_start(&gButton_sm);

    BUTTON_DEBUG_PRINT("Button: initialized.\n");
}

uint32_t Button_GetState(void)
{
    return gButton_sm.vars.button_state;
}

static void Button_InterruptHandler(ButtonDrvButtonID button_id)
{
    BUTTON_DEBUG_PRINT("Button: Interrupt received for button %s\n", ButtonDrv_GetButtonString(button_id));

    if(button_id == BUTTONDRV_BUTTONID_LEFT || button_id == BUTTONDRV_BUTTONID_RIGHT)
    {
        // Go to IDLE
        Button_dispatch_event(&gButton_sm, Button_EventId_INTERRUPTED);
    }
}

static void Button_TimerHandler(void)
{
    BUTTON_DEBUG_PRINT("Button: Timer expired\n");

    // Go to WAIT
    Button_dispatch_event(&gButton_sm, Button_EventId_TIMEOUT);
    // Do button state update
    Button_dispatch_event(&gButton_sm, Button_EventId_DO);
}