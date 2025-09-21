#ifndef UART_DRV_H
#define UART_DRV_H

#include <stdint.h>

#ifdef UARTDRV_DEBUG
#include "MyPrintf.h"
#define UARTDRV_DEBUG_PRINT(...) MyPrintf(__VA_ARGS__)
#else
#define UARTDRV_DEBUG_PRINT(...) (void)0
#endif

void UartDrv_Init(void);
void UartDrv_SendData(uint8_t *data, uint32_t size);

#endif // UART_DRV_H