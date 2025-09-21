#include "CommandSender.h"
#include "CommandSender_API.h"
#include "Button_API.h"
#include "TimerDrv.h"
#include "UartDrv.h"
#include "SlideCarCommand.pb.h"
#include "pb_encode.h"

void CommandSender_WAIT_enter(CommandSender* sm);
void CommandSender_WAIT_exit(CommandSender* sm);
void CommandSender_TICK_enter(CommandSender* sm);
void CommandSender_TICK_exit(CommandSender* sm);
void CommandSender_TICK_do(CommandSender* sm);


void CommandSender_WAIT_enter(CommandSender* sm)
{
    // Start timer
    TimerDrv_Start(TIMERDRV_ID_COMMANDSENDER, sm->vars.timer_callback, 100);

    COMMANDSENDER_DEBUG_PRINT("CommandSender: Entered WAIT state.\n");
}

void CommandSender_WAIT_exit(CommandSender* sm)
{
    // Stop timer
    TimerDrv_Stop(TIMERDRV_ID_COMMANDSENDER);

    COMMANDSENDER_DEBUG_PRINT("CommandSender: Exited WAIT state.\n");
}

void CommandSender_TICK_enter(CommandSender* sm)
{
    uint32_t button_state = Button_GetState();
    SlideCarCommandType type = SlideCarCommandType_STOP;
    uint32_t steps = 0;

    if(BUTTON_STATE_MASK(button_state, BUTTON_STATE_LEFT))
    {
        type = SlideCarCommandType_LEFT;
        steps = COMMANDSENDER_LEFT_STEPS_DEF;
    }
    else if(BUTTON_STATE_MASK(button_state, BUTTON_STATE_RIGHT))
    {
        type = SlideCarCommandType_RIGHT;
        steps = COMMANDSENDER_RIGHT_STEPS_DEF;
    }

    CommandSender_SetCommand(type, steps);
    COMMANDSENDER_DEBUG_PRINT("CommandSender: Entered TICK state.\n");
}

void CommandSender_TICK_exit(CommandSender* sm)
{
    COMMANDSENDER_DEBUG_PRINT("CommandSender: Exited TICK state.\n");
}

void CommandSender_TICK_do(CommandSender* sm)
{
    SlideCarCommand command = SlideCarCommand_init_zero;
    command.id = SlideCarCommandId_MOVE;
    command.type = sm->vars.type;
    command.steps = sm->vars.steps;

    uint8_t buffer[SlideCarCommand_size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    (void)pb_encode(&stream, SlideCarCommand_fields, &command);

    // Send command via UART
    UartDrv_SendData(buffer, stream.bytes_written);

    COMMANDSENDER_DEBUG_PRINT("CommandSender: Sent command of type %d with %d steps.\n", sm->vars.command_type, sm->vars.steps);
}
