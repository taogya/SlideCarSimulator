#ifndef UART_DRV_H
#define UART_DRV_H

#include <stdint.h>

#ifdef UARTDRV_DEBUG
#include "MyPrintf.h"
#define UARTDRV_DEBUG_PRINT(...) MyPrintf(__VA_ARGS__)
#else
#define UARTDRV_DEBUG_PRINT(...) (void)0
#endif

typedef void (*UartDrvCallback)(uint8_t *buf, uint32_t size);

typedef enum {
    UARTDRV_RX_COMPLETE,
    UARTDRV_RX_ERROR,
    UARTDRV_RX_MAX
} UartDrvEvent;

#define UARTDRV_RX_TIMEOUT     3    // 受信タイムアウト時間（秒）
#define UARTDRV_RX_BUFFER_SIZE 256  // 受信バッファサイズ

void UartDrv_Init(UartDrvCallback rx_complete_callback, UartDrvCallback rx_error_callback);

#endif // UART_DRV_H