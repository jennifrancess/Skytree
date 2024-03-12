#include <stdio.h>
#include "CommandHandle.h"
#include "ADXL343.h"

volatile uint8_t data_ready;
int main(int argc, char **argv)
{
    union interrupt_config int_config;
    union interrupt_config int_map ;
    FunctionStatus status;
    // disable interrupts before initialization
    status=ADXL343_disableInterrupts();
    if(status==FUNCTION_STATUS_OK){
        status=ADXL343_Initialize();
        if(status==FUNCTION_STATUS_OK) {
            // map the interrupts to the INT1 and INT2 pin
            printf("Initialized Successfully\n");
            int_map.bits.overrun    = 0;    /* Set the INT1 */
            int_map.bits.watermark  = 0;
            int_map.bits.freefall   = 0;
            int_map.bits.inactivity = 0;
            int_map.bits.activity   = 0;
            int_map.bits.double_tap = 0;
            int_map.bits.single_tap = 0;
            int_map.bits.data_ready = 1;
            printf("%d\n",int_map.value);
            status= ADXL343_MapInterrupts(int_map);
            if(status==FUNCTION_STATUS_OK) {
                printf("Mapped Interrupts\n");
                // enable the interrupts you want
                int_config.bits.activity = 1;
                int_config.bits.data_ready = 1;
                int_config.bits.double_tap = 0;
                int_config.bits.freefall = 0;
                int_config.bits.inactivity = 1;
                int_config.bits.overrun = 1;
                int_config.bits.single_tap = 0;
                int_config.bits.watermark = 0;
                status = ADXL343_enableInterrupts(int_config);
                if (status == FUNCTION_STATUS_OK) {
                    printf("Enabled interrupts\n");
                    Command cmd;
                    cmd.cmd_set = 0;
                    data_ready = 1;
                       //while(1) {
                            char command[20];
                            scanf("%[^\n]", command);
                            decode_command(&cmd, command);// command line argument can also be passed
                            if (cmd.cmd_set) {
                                status = handle_command(&cmd);
                                if (status == FUNCTION_STATUS_OK) {
                                    printf("PASS\n");
                                } else {
                                    printf("FAIL\n");
                                }
                            }
                        //}
                }
            }
        }
    }
    return 0;

}