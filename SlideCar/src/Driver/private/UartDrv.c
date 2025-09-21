#include "UartDrv.h"
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

static uint8_t gUartDrvIsInitialized = 0;
static uint8_t gUartDrvRxBuffer[UARTDRV_RX_BUFFER_SIZE];
static uint32_t gUartDrvRxBufferSize = 0;
static UartDrvCallback gUartDrvRxCallbacks[UARTDRV_RX_MAX] = {0};

static void UartDrv_InterruptHandler(char c)
{
    UartDrvEvent event = UARTDRV_RX_MAX;
    UARTDRV_DEBUG_PRINT("UART Driver: Received char '%c'\n", c);

    gUartDrvRxBuffer[gUartDrvRxBufferSize++] = c;
    if(gUartDrvRxBufferSize >= UARTDRV_RX_BUFFER_SIZE)
    {
        event = UARTDRV_RX_ERROR;
        UARTDRV_DEBUG_PRINT("UART Driver: RX buffer overflow\n");
    }
    else if(c == '\n')
    {
        event = UARTDRV_RX_COMPLETE;
        UARTDRV_DEBUG_PRINT("UART Driver: RX complete\n");
    }

    if(event != UARTDRV_RX_MAX)
    {
        if(gUartDrvRxCallbacks[event] != NULL)
        {
            gUartDrvRxCallbacks[event](gUartDrvRxBuffer, gUartDrvRxBufferSize);
        }
        gUartDrvRxBufferSize = 0; // バッファサイズリセット
        memset(gUartDrvRxBuffer, 0, UARTDRV_RX_BUFFER_SIZE); // バッファクリア
    }
}

/* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
static pthread_t gUartDrvInputThread;
static int gUartDrvThreadRunning = 0;

static void* UartDrv_InputThread(void* arg)
{
    gUartDrvThreadRunning = 1;
    while (gUartDrvThreadRunning) {
        int c = getchar();
        if (c == EOF) continue;
        UartDrv_InterruptHandler(c);
    }
    return NULL;
}
/* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */

void UartDrv_Init(UartDrvCallback rx_complete_callback, UartDrvCallback rx_error_callback)
{
    if (!gUartDrvIsInitialized)
    {
        gUartDrvRxCallbacks[UARTDRV_RX_COMPLETE] = rx_complete_callback;
        gUartDrvRxCallbacks[UARTDRV_RX_ERROR] = rx_error_callback;
        /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
        // キーボード入力監視スレッド起動
        (void)pthread_create(&gUartDrvInputThread, NULL, UartDrv_InputThread, NULL);
        UARTDRV_DEBUG_PRINT("UART Driver: Input thread started.\n");
        /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */
        gUartDrvIsInitialized = 1;
        UARTDRV_DEBUG_PRINT("UART Driver Initialized.\n");
    }
    else
    {
        UARTDRV_DEBUG_PRINT("UART Driver already initialized.\n");
    }
}
