//
// Created by Oduche Ijeoma on 3/11/2024.
//
#include <string.h>
#include <stdio.h>
#include "ADXL343.h"
#include "i2c_driver.h"

static FunctionStatus ADXL343_write_register(uint8_t addr,uint8_t value);
static FunctionStatus ADXL343_read_register(uint8_t addr,uint8_t* buffer, size_t length);
static uint8_t get_device_id();
static FunctionStatus setRange_Res( adxl343_range_t range, adxl343_res_t resolution);
static adxl343_range_t getRange();
static adxl343_range_t currentRange;
static adxl343_res_t currentResolution;

static FunctionStatus ADXL343_write_register(uint8_t addr,uint8_t value)
{
    uint8_t data[2];
    data[0] = addr;
    data[1] = value ;
    return i2c_write((const char*)data,2,100);
}

static FunctionStatus ADXL343_read_register(uint8_t addr,uint8_t* buffer,size_t length)
{
    FunctionStatus status;
    uint8_t data[2]={0};
    data[0]= addr;
    status=i2c_write((const char*)data,1,100);
    if(status==FUNCTION_STATUS_OK){
        return i2c_read(buffer,length,100);
    }
   return FUNCTION_STATUS_ERROR;
}

static uint8_t get_device_id()
{
    uint8_t device_id=0xE5;
    ADXL343_read_register(ADXL343_REG_DEVID,&device_id,1);
    return device_id;
}

static FunctionStatus setRange_Res( adxl343_range_t range,adxl343_res_t resolution)
{
    uint8_t device_range;
    uint8_t device_int;
    FunctionStatus status;
    status=ADXL343_read_register(ADXL343_REG_INT_ENABLE,&device_int,1);
    if(status==FUNCTION_STATUS_OK) {
        status = ADXL343_write_register(ADXL343_INT_ENABLE,0); // disable interrupts before configuring the data format
        if(status ==FUNCTION_STATUS_OK) {
            // read the current data format register contents
            status = ADXL343_read_register(ADXL343_REG_DATA_FORMAT, &device_range, 1);
            if (status == FUNCTION_STATUS_OK) {
                // clear the the resolution and range bit in the data format register
                device_range &=  ~(ADXL343_RANGEMASK|ADXL343_RESOLUTIONMASK);
                // set the range and resolution bit with the chosen range and resolution
                device_range |= (range | resolution);
                printf("range mask:%x \n",(range|resolution));
                status=ADXL343_write_register(ADXL343_DATA_FORMAT, device_range);
                if(status==FUNCTION_STATUS_OK){
                    return ADXL343_write_register(ADXL343_INT_ENABLE,device_int); // Enable interrupts back after configuring the data format
                }
            }
        }
    }
    return FUNCTION_STATUS_ERROR;
}

FunctionStatus ADXL343_enableInterrupts(union interrupt_config cfg)
{

    return ADXL343_write_register(ADXL343_REG_INT_ENABLE, cfg.value);
}
FunctionStatus ADXL343_disableInterrupts()
{
    return ADXL343_write_register(ADXL343_REG_INT_ENABLE, 0);
}
FunctionStatus ADXL343_MapInterrupts(union interrupt_config cfg)
{

    return ADXL343_write_register(ADXL343_REG_INT_MAP, cfg.value);
}

FunctionStatus ADXL343_Initialize(void)
{
    /* Check connection */
    uint8_t deviceid = get_device_id();
    if (deviceid != 0xE5) {
        /* No ADXL343 detected ... return false */
        return FUNCTION_STATUS_ERROR;
    }
    else {

        FunctionStatus  status;
        uint8_t thresh_act;
        /*check if the sensor has been initialized by checking if the contents of thresh_ac register is 0(reset value) d*/
        status=ADXL343_read_register(ADXL343_REG_THRESH_ACT,&thresh_act,1);
        if(status==FUNCTION_STATUS_OK) {
            if (thresh_act == 0 ) {
                return FUNCTION_STATUS_ALREADY_INITIALIZED;
            }
            else {
                /* initialize the sensor */
                currentRange = ADXL343_RANGE_2_G;
                currentResolution = ADXL343_RESOLUTION_10BIT;
                status = ADXL343_write_register(ADXL343_REG_THRESH_ACT, ADXL343_THRESH_ACT);
                if (status == FUNCTION_STATUS_OK) {
                    status = ADXL343_write_register(ADXL343_REG_THRESH_INACT, ADXL343_THRESH_INACT);
                    if (status == FUNCTION_STATUS_OK) {
                        status = ADXL343_write_register(ADXL343_REG_TIME_INACT, ADXL343_TIME_INACT);
                        if (status == FUNCTION_STATUS_OK) {
                            status = ADXL343_write_register(ADXL343_REG_ACT_INACT_CTL, ADXL343_ACT_INACT_CTL);
                            if (status == FUNCTION_STATUS_OK) {
                                status = ADXL343_write_register(ADXL343_REG_TAP_AXES, ADXL343_TAP_AXES);
                                //ADXL343_write_register(ADXL343_REG_INT_MAP, ADXL343_INT_MAP);
                                if (status == FUNCTION_STATUS_OK) {
                                    status = ADXL343_write_register(ADXL343_REG_FIFO_CTL, ADXL343_FIFO_CTL);
                                    if (status == FUNCTION_STATUS_OK) {
                                        status = ADXL343_write_register(ADXL343_REG_POWER_CTL, ADXL343_POWER_CTL);
                                    }
                                }
                            }
                        }
                    }
                }
                return status;
            }
        }
        return FUNCTION_STATUS_ERROR;
    }
}

FunctionStatus ADXL343_getXYZ(int16_t* XYZ)
{
    if(data_ready==1) { // only sample data is the data_ready interrupt has been triggered
        int16_t temp_buffer[3] = {0};
        FunctionStatus status;
        status = ADXL343_read_register(ADXL343_REG_DATAX0, (uint8_t*) & temp_buffer, 6);
        if (status == FUNCTION_STATUS_OK) {
            XYZ[0] = temp_buffer[0];
            XYZ[1] = temp_buffer[1];
            XYZ[2] = temp_buffer[2];
        }
        return status;
    }
    return FUNCTION_STATUS_DATA_READY_ERROR;
}

FunctionStatus ADXL343_changeRange(unsigned int range, const char* resolution)
{
    printf("%x\n",currentRange);
    adxl343_range_t new_range;
    adxl343_res_t new_resolution;
    int result;
    new_range=range;
    result=strcmp(resolution,"10bit");
    if (result==0) {
        new_resolution=ADXL343_RESOLUTION_10BIT;
    } else {
        new_resolution=ADXL343_RESOLUTION_FULL;
    }
    //uncomment below if the math.h library is not available on the used microcontroller
//    switch(range){
//        case 2: {
//            new_range=ADXL343_RANGE_2_G;
//            result=strcmp(resolution,"10bit");
//            if (result==0) {
//               new_resolution=ADXL343_RESOLUTION_10BIT;
//            } else {
//                new_resolution=ADXL343_RESOLUTION_FULL;
//            }
//            break;
//        }
//        case 4:{
//            new_range=ADXL343_RANGE_4_G;
//            result=strcmp(resolution,"10bit");
//            if (result==0) {
//                new_resolution=ADXL343_RESOLUTION_10BIT;
//            } else {
//                new_resolution=ADXL343_RESOLUTION_FULL;
//            }
//            break;
//        }
//        case 8:{
//            new_range=ADXL343_RANGE_8_G;
//            result=strcmp(resolution,"10bit");
//            if (result==0) {
//                new_resolution=ADXL343_RESOLUTION_10BIT;
//            } else {
//                new_resolution=ADXL343_RESOLUTION_FULL;
//            }
//            break;
//        }
//        case 16:{
//            new_range=ADXL343_RANGE_16_G;
//            result=strcmp(resolution,"10bit");
//            if (result==0) {
//                new_resolution=ADXL343_RESOLUTION_10BIT;
//            } else {
//                new_resolution=ADXL343_RESOLUTION_FULL;
//            }
//            break;
//        }
//        default:
//            break;
//    }
    FunctionStatus status;

    if(new_range==currentRange){
        if(new_resolution==currentResolution){
            // do nothing since no change in range or resolution
            status=FUNCTION_STATUS_OK;
        }
        else{
            status=setRange_Res(currentRange,new_resolution);
            if(status==FUNCTION_STATUS_OK) {
                currentResolution = new_resolution;
            }
        }
    }
    else{
        if(new_resolution==currentResolution){
            status=setRange_Res(new_range,currentResolution);
            if(status==FUNCTION_STATUS_OK) {
                currentRange = new_range;
            }
        }
        else{
            status=setRange_Res(new_range,new_resolution);
            if(status==FUNCTION_STATUS_OK) {
                currentRange=new_range;
                currentResolution = new_resolution;
            }

        }
        printf("%x\n",currentRange);
    }
    return status;
}

