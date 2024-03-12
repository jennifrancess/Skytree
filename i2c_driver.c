// --------------------------------------------------------------------------------------------------------------------
/// \file  i2c_driver.c
/// \brief Description
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// Include files
// --------------------------------------------------------------------------------------------------------------------
#include <stddef.h>
#include "i2c_driver.h"

// --------------------------------------------------------------------------------------------------------------------
// Constant and macro definitions
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// Type definitions
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// File-scope variables
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// Function declarations
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// Function definitions
// --------------------------------------------------------------------------------------------------------------------
FunctionStatus i2c_write(const char* dataToWrite, size_t length, uint32_t timeout)
{
    if (dataToWrite == NULL)
    {
        return FUNCTION_STATUS_ARGUMENT_ERROR;
    }

    // Assume there is a nice implementation of I2C driver here :)

    return FUNCTION_STATUS_OK;
}

FunctionStatus i2c_read(char* dataToRead, size_t length, uint32_t timeout)
{
    if (dataToRead == NULL)
    {
        return FUNCTION_STATUS_ARGUMENT_ERROR;
    }

    // Assume there is a nice implementation of I2C driver here :)

    return FUNCTION_STATUS_OK;
}

