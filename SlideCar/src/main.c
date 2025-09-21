#include "UartDrv.h"
#include "MyPrintf.h"
#include "Motor_API.h"
#include "SlideCarCommand.pb.h"
#include <pb_decode.h>
#include <unistd.h>
#include <stdlib.h>

static void hexstr_to_bin(const char* hexstr, uint8_t* bin, size_t* bin_size) {
    size_t len = strlen(hexstr);
    size_t cnt = 0;

    for (size_t i = 0; i < len;) {
        while (hexstr[i] == ' ' || hexstr[i] == '\t' || hexstr[i] == '\n') i++; // スペース等スキップ
        if (i >= len) break;

        char byte_str[3] = {0};
        byte_str[0] = hexstr[i++];
        if (i < len && hexstr[i] != ' ' && hexstr[i] != '\t' && hexstr[i] != '\n') {
            byte_str[1] = hexstr[i++];
        }
        bin[cnt++] = (uint8_t)strtol(byte_str, NULL, 16);
    }
    *bin_size = cnt;
}

static void rx_callback(uint8_t *buf, uint32_t size) {
    Motor_State state = MOTOR_STATE_MAX;
    /* vvvvv For Test on PC vvvvvvvvvvvvvvvvvvvv */
    uint8_t cmd_buf[UARTDRV_RX_BUFFER_SIZE / 2];
    size_t bin_size = 0;
    hexstr_to_bin((char*)buf, cmd_buf, &bin_size);
    /* ^^^^^ For Test on PC ^^^^^^^^^^^^^^^^^^^^ */
    // Decode the received protobuf message
    SlideCarCommand command = SlideCarCommand_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(cmd_buf, bin_size);
    if (pb_decode(&stream, SlideCarCommand_fields, &command)) {
        // Successfully decoded the message
        switch(command.id) {
            case SlideCarCommandId_MOVE:
                switch(command.type) {
                case SlideCarCommandType_STOP:
                    state = MOTOR_STATE_STOP;
                    break;
                case SlideCarCommandType_LEFT:
                    state = MOTOR_STATE_LEFT_MOVE;
                    break;
                case SlideCarCommandType_RIGHT:
                    state = MOTOR_STATE_RIGHT_MOVE;
                    break;
                default:
                    MyPrintf("Received unknown command type: %d\n", command.type);
                    return;
                }
                if(state != MOTOR_STATE_MAX) {
                    Motor_SetMoveState(state, command.steps);
                    Motor_Run();
                }
                break;
            default:
                MyPrintf("Received unknown command ID: %d\n", command.id);
                return;
        }
    } else {
        // Handle decode error
        MyPrintf("Decode failed: %s\n", PB_GET_ERROR(&stream));
    }
}

static void error_callback(uint8_t *buf, uint32_t size) {
    MyPrintf("UART receive error\n");
}

int main(void)
{
    // driver initializations
    UartDrv_Init(rx_callback, error_callback);

    // Motor initialization
    Motor_Init();

    while (1)
    {
        pause();
    }

    return 0;
}