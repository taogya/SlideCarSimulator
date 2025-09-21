#ifndef MOTOR_API_H
#define MOTOR_API_H

#ifdef MOTOR_DEBUG
#include "MyPrintf.h"
#define MOTOR_DEBUG_PRINT(...) MyPrintf(__VA_ARGS__)
#else
#define MOTOR_DEBUG_PRINT(...) (void)0
#endif

typedef enum {
    MOTOR_STATE_STOP = 0,
    MOTOR_STATE_LEFT_MOVE,
    MOTOR_STATE_RIGHT_MOVE,
    MOTOR_STATE_MAX
} Motor_State;

#define MOTOR_LEFT_MOVE_MAX_STEP -100
#define MOTOR_RIGHT_MOVE_MAX_STEP 100

extern void Motor_Init(void);
extern void Motor_SetMoveState(Motor_State state, uint32_t step);
extern void Motor_Run(void);

#endif // MOTOR_API_H