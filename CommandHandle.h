//
// Created by Beheerder on 3/10/2024.
//

#ifndef SOURCE_COMMANDHANDLE_H
#define SOURCE_COMMANDHANDLE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>                            //!< Include to use standard boolean
#include <stdint.h>
#include <math.h>
#include "FunctionStatus.h"

#define NO_OF_CMD 9
typedef struct {
    unsigned char cmd[15];
    uint8_t cmd_set;
}Command;

const static char* cmd_str[NO_OF_CMD] ={"2g 10bit","2g full","4g 10bit","4g full","8g 10bit","8g full","16g 10bit","16g full","getXYZ"};
extern volatile uint8_t command_recieved; /**< set this flag in a  ISR routine if a command is received*/

void decode_command(Command* input, const char* command);
FunctionStatus  handle_command(Command* input);
#endif //SOURCE_COMMANDHANDLE_H
