#ifndef BUTTON_API_H
#define BUTTON_API_H

#ifdef BUTTON_DEBUG
#include "MyPrintf.h"
#define BUTTON_DEBUG_PRINT(...) MyPrintf(__VA_ARGS__)
#else
#define BUTTON_DEBUG_PRINT(...) (void)0
#endif

#define BUTTON_DEBOUNCE_TICK        1   // milliseconds
#define BUTTON_DEBOUNCE_TIMEOUT     10  // Ticks
#define BUTTON_PRESSED_THRESHOLD    5   // Counts

#define BUTTON_STATE_NONE  0x00
#define BUTTON_STATE_LEFT  0x01
#define BUTTON_STATE_RIGHT 0x02
#define BUTTON_STATE_MASK(state, mask) ((state & mask) > 0)

extern void Button_Init(void);
extern uint32_t Button_GetState(void);

#endif // BUTTON_API_H