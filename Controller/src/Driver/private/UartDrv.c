#include "UartDrv.h"
#include "MyPrintf.h"

static uint8_t gUartDrvIsInitialized = 0;

void UartDrv_Init(void)
{
    if (!gUartDrvIsInitialized)
    {
        /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
        /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */
        gUartDrvIsInitialized = 1;
        UARTDRV_DEBUG_PRINT("UART Driver Initialized.\n");
    }
    else
    {
        UARTDRV_DEBUG_PRINT("UART Driver already initialized.\n");
    }
}

void UartDrv_SendData(uint8_t *data, uint32_t size)
{
    if (gUartDrvIsInitialized)
    {
        /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
        UARTDRV_DEBUG_PRINT("UART Driver Sending Data: size=%d, data=", size);
        for (uint32_t i = 0; i < size; i++)
        {
            MyPrintf("%02X ", data[i]);
        }
        MyPrintf("\n");
        /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */
    }
}
