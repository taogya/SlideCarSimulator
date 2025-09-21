#include <stdint.h>
#include "Motor.h"
#include "Motor_API.h"
#include "MyPrintf.h"

void Motor_STOP_enter(Motor* sm);
void Motor_STOP_exit(Motor* sm);
void Motor_STOP_do(Motor* sm);
void Motor_LEFT_MOVE_enter(Motor* sm);
void Motor_LEFT_MOVE_exit(Motor* sm);
void Motor_LEFT_MOVE_do(Motor* sm);
void Motor_RIGHT_MOVE_enter(Motor* sm);
void Motor_RIGHT_MOVE_exit(Motor* sm);
void Motor_RIGHT_MOVE_do(Motor* sm);
void Motor_LEFT_MOVE_MAX_enter(Motor* sm);
void Motor_LEFT_MOVE_MAX_exit(Motor* sm);
void Motor_LEFT_MOVE_MAX_do(Motor* sm);
void Motor_RIGHT_MOVE_MAX_enter(Motor* sm);
void Motor_RIGHT_MOVE_MAX_exit(Motor* sm);
void Motor_RIGHT_MOVE_MAX_do(Motor* sm);


void Motor_STOP_enter(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Entered STOP state.\n");
}

void Motor_STOP_exit(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Exited STOP state.\n");
}

void Motor_STOP_do(Motor* sm)
{
    /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
    MyPrintf("%d %d %d\n", sm->vars.state, sm->vars.step, sm->vars.total_step);
    /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */
}

void Motor_LEFT_MOVE_enter(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Entered LEFT_MOVE state.\n");
}

void Motor_LEFT_MOVE_exit(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Exited LEFT_MOVE state.\n");
}

void Motor_LEFT_MOVE_do(Motor* sm)
{
    if(MOTOR_LEFT_MOVE_MAX_STEP < sm->vars.total_step - (int32_t)sm->vars.step) {
        sm->vars.total_step -= (int32_t)sm->vars.step;
    }
    else {
        sm->vars.total_step = MOTOR_LEFT_MOVE_MAX_STEP;
    }
    /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
    MyPrintf("%d %d %d\n", sm->vars.state, sm->vars.step, sm->vars.total_step);
    /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */

    if(sm->vars.total_step == MOTOR_LEFT_MOVE_MAX_STEP) {
        Motor_dispatch_event(sm, Motor_EventId_LEFT_MOVE_MAX);
    }
}

void Motor_RIGHT_MOVE_enter(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Entered RIGHT_MOVE state.\n");
}

void Motor_RIGHT_MOVE_exit(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Exited RIGHT_MOVE state.\n");
}

void Motor_RIGHT_MOVE_do(Motor* sm)
{
    if(sm->vars.total_step + (int32_t)sm->vars.step < MOTOR_RIGHT_MOVE_MAX_STEP) {
        sm->vars.total_step += (int32_t)sm->vars.step;
    }
    else {
        sm->vars.total_step = MOTOR_RIGHT_MOVE_MAX_STEP;
    }
    /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
    MyPrintf("%d %d %d\n", sm->vars.state, sm->vars.step, sm->vars.total_step);
    /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */

    if(sm->vars.total_step == MOTOR_RIGHT_MOVE_MAX_STEP) {
        Motor_dispatch_event(sm, Motor_EventId_RIGHT_MOVE_MAX);
    }
}

void Motor_LEFT_MOVE_MAX_enter(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Entered LEFT_MOVE_MAX state.\n");
}
void Motor_LEFT_MOVE_MAX_exit(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Exited LEFT_MOVE_MAX state.\n");
}
void Motor_LEFT_MOVE_MAX_do(Motor* sm)
{
    /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
    MyPrintf("%d %d %d\n", sm->vars.state, sm->vars.step, sm->vars.total_step);
    /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */
}
void Motor_RIGHT_MOVE_MAX_enter(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Entered RIGHT_MOVE_MAX state.\n");
}
void Motor_RIGHT_MOVE_MAX_exit(Motor* sm)
{
    MOTOR_DEBUG_PRINT("Motor: Exited RIGHT_MOVE_MAX state.\n");
}
void Motor_RIGHT_MOVE_MAX_do(Motor* sm)
{
    /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
    MyPrintf("%d %d %d\n", sm->vars.state, sm->vars.step, sm->vars.total_step);
    /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */
}