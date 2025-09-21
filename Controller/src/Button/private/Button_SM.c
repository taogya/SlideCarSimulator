#include <stdint.h>
#include "Button.h"
#include "Button_API.h"
#include "TimerDrv.h"
#include "ButtonDrv.h"

void Button_IDLE_enter(Button* sm);
void Button_IDLE_exit(Button* sm);
void Button_WAIT_enter(Button* sm);
void Button_WAIT_exit(Button* sm);
void Button_TICK_enter(Button* sm);
void Button_TICK_exit(Button* sm);
void Button_TICK_do(Button* sm);

void Button_IDLE_enter(Button* sm)
{
    // Enable button interrupt
    ButtonDrv_Enable(BUTTONDRV_ID_BUTTON_DRV, sm->vars.button_callback);

    BUTTON_DEBUG_PRINT("Button: Entered IDLE state.\n");
}

void Button_IDLE_exit(Button* sm)
{
    // Disable button interrupt
    ButtonDrv_Disable(BUTTONDRV_ID_BUTTON_DRV);

    // Reset button counts and tick count
    sm->vars.left_count = 0;
    sm->vars.right_count = 0;
    sm->vars.tick_count = 0;
    
    BUTTON_DEBUG_PRINT("Button: Exited IDLE state.\n");
}

void Button_WAIT_enter(Button* sm)
{
    // Start timer
    TimerDrv_Start(TIMERDRV_ID_DEBOUNCE, sm->vars.timer_callback, BUTTON_DEBOUNCE_TICK);

    BUTTON_DEBUG_PRINT("Button: Entered WAIT state.\n");
}

void Button_WAIT_exit(Button* sm)
{
    // Stop timer
    TimerDrv_Stop(TIMERDRV_ID_DEBOUNCE);

    BUTTON_DEBUG_PRINT("Button: Exited WAIT state.\n");
}

void Button_TICK_enter(Button* sm)
{
    BUTTON_DEBUG_PRINT("Button: Entered TICK state.\n");
}

void Button_TICK_exit(Button* sm)
{
    BUTTON_DEBUG_PRINT("Button: Exited TICK state.\n");
}

void Button_TICK_do(Button* sm)
{
    uint32_t button_state = 0;
    Button_EventId event_id = Button_EventId_NOT_DETECTED;

    sm->vars.left_count += ButtonDrv_GetState(BUTTONDRV_BUTTONID_LEFT);
    sm->vars.right_count += ButtonDrv_GetState(BUTTONDRV_BUTTONID_RIGHT);

    sm->vars.tick_count++;

    // Check press left button
    if (sm->vars.left_count >= BUTTON_PRESSED_THRESHOLD)
    {
        button_state |= BUTTON_STATE_LEFT;
        event_id = Button_EventId_DETECTED;
    }
    // Check press right button
    if (sm->vars.right_count >= BUTTON_PRESSED_THRESHOLD)
    {
        button_state |= BUTTON_STATE_RIGHT;
        event_id = Button_EventId_DETECTED;
    }
    // Check timeout
    if (sm->vars.tick_count >= BUTTON_DEBOUNCE_TIMEOUT)
    {
        event_id = Button_EventId_DETECTED;
    }

    // Update button state if detected
    if (event_id == Button_EventId_DETECTED)
    {
        sm->vars.button_state = button_state;
    }

    BUTTON_DEBUG_PRINT("Button: TICK - LeftCount=%d, RightCount=%d, TickCount=%d, Event=%d\n",
                sm->vars.left_count, sm->vars.right_count, sm->vars.tick_count, event_id);
    Button_dispatch_event(sm, event_id);
}
