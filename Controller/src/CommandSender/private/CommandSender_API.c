#include "CommandSender.h"
#include "CommandSender_API.h"

static CommandSender gCommandSender_sm = {0};

static void CommandSender_TimerHandler(void);

void CommandSender_Init(void)
{
    CommandSender_ctor(&gCommandSender_sm);
    gCommandSender_sm.vars.timer_callback = CommandSender_TimerHandler;
    CommandSender_start(&gCommandSender_sm);

    COMMANDSENDER_DEBUG_PRINT("CommandSender: Initialized.\n");
}

void CommandSender_SetCommand(int type, int steps)
{
    gCommandSender_sm.vars.type = (SlideCarCommandType)type;
    gCommandSender_sm.vars.steps = (uint32_t)steps;

    COMMANDSENDER_DEBUG_PRINT("CommandSender: Set command type to %d with %d steps.\n", type, steps);
}

static void CommandSender_TimerHandler(void)
{
    COMMANDSENDER_DEBUG_PRINT("CommandSender: Timer expired\n");

    // Go to TICK
    CommandSender_dispatch_event(&gCommandSender_sm, CommandSender_EventId_TIMEOUT);
    // Do command action
    CommandSender_dispatch_event(&gCommandSender_sm, CommandSender_EventId_DO);
}

