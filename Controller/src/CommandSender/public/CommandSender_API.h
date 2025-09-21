#ifndef COMMANDSENDER_API_H
#define COMMANDSENDER_API_H

#ifdef COMMANDSENDER_DEBUG
#include "MyPrintf.h"
#define COMMANDSENDER_DEBUG_PRINT(...) MyPrintf(__VA_ARGS__)
#else
#define COMMANDSENDER_DEBUG_PRINT(...) (void)0
#endif

#define COMMANDSENDER_LEFT_STEPS_DEF  1
#define COMMANDSENDER_RIGHT_STEPS_DEF 1

extern void CommandSender_Init(void);
extern void CommandSender_SetCommand(int type, int steps);

#endif // COMMANDSENDER_API_H