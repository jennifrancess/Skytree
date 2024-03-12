//
// Created by Beheerder on 3/11/2024.
//

#include "CommandHandle.h"
#include "ADXL343.h"

static int16_t XYZ[3];
extern FunctionStatus ADXL343_changeRange(unsigned int range,const char* resolution);
extern FunctionStatus ADXL343_getXYZ(int16_t* XYZ);


void decode_command(Command* input, const char* command)
{
    memset(input->cmd,0,sizeof(input->cmd));
    if(!input->cmd_set){
        int result;
        for(int j=0;j<NO_OF_CMD;j++){
            result=strcmp(command,cmd_str[j]);
            if(result==0){
                input->cmd_set=1;
                break;
            }
        }

    }
    if(input->cmd_set){
        strcpy(input->cmd,command);
    }

    //if serial is used message handling is required

}

FunctionStatus  handle_command(Command* input)
{
    if(input->cmd_set ) //if command is set(serial)
    {
        if(strcmp((const char*)input->cmd,cmd_str[8])==0){
            XYZ[0]=0;
            XYZ[1]=0;
            XYZ[2]=0;
            return ADXL343_getXYZ(XYZ);
        }
        else {
            char *token;
            char temp_command[20];
            char strRange[5];
            char resolution[7];
            strcpy(temp_command, input->cmd); //copy the command to a temp variable to avoid being changed
            token = strtok(temp_command, " ");
            if (token != NULL) {
                strcpy(strRange, token);
                token = strtok(NULL, " ");
                if (token != NULL) {
                    strcpy(resolution, token);

                }
            }
            int range = atoi(strRange);
            printf("%f, %s\n",log2f(range)-1,resolution);
            return ADXL343_changeRange(log2f(range)-1, resolution);
        }
    }
    return  FUNCTION_STATUS_INCOMPLETE;
}

