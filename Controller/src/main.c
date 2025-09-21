#include "ButtonDrv.h"
#include "TimerDrv.h"
#include "UartDrv.h"
#include "MyPrintf.h"
#include "Button_API.h"
#include "CommandSender_API.h"
#include <unistd.h>

int main(void)
{
    // driver initializations
    ButtonDrv_Init();
    TimerDrv_Init();
    UartDrv_Init();

    // Button initialization
    Button_Init();

    // Command initialization
    CommandSender_Init();

    while (1)
    {
        pause();
    }

    return 0;
}