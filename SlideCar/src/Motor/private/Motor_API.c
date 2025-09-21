#include <stdint.h>
#include "Motor.h"
#include "Motor_API.h"

static Motor gMotor_sm = {0};

void Motor_Init(void)
{
    Motor_ctor(&gMotor_sm);
    Motor_start(&gMotor_sm);

    MOTOR_DEBUG_PRINT("Motor: initialized.\n");
}

void Motor_SetMoveState(Motor_State state, uint32_t step)
{
    Motor_EventId event_id = Motor_EventId_DO;
    switch(state) {
    case MOTOR_STATE_STOP:
        event_id = Motor_EventId_STOP;
        break;
    case MOTOR_STATE_LEFT_MOVE:
        event_id = Motor_EventId_LEFT_MOVE;
        break;
    case MOTOR_STATE_RIGHT_MOVE:
        event_id = Motor_EventId_RIGHT_MOVE;
        break;
    default:
        MOTOR_DEBUG_PRINT("Motor: WARNING: unknown state %d\n", state);
        break;
    }

    if(event_id != Motor_EventId_DO) {
        gMotor_sm.vars.state = state;
        gMotor_sm.vars.step = step;
        Motor_dispatch_event(&gMotor_sm, event_id);
    }
}

void Motor_Run(void)
{
    switch (gMotor_sm.vars.state) {
    case MOTOR_STATE_STOP:
    case MOTOR_STATE_LEFT_MOVE:
    case MOTOR_STATE_RIGHT_MOVE:
        Motor_dispatch_event(&gMotor_sm, Motor_EventId_DO);
        break;
    default:
        MOTOR_DEBUG_PRINT("Motor: WARNING: unknown command type %d\n", gMotor_sm.vars.state);
        break;
    }
}