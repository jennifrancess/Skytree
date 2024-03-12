****README*****
use gcc  i2c_driver.c ADXL343.c CommandHandle.c  main.
c -lm -o SkyTreeProject to build the project on linux with a gcc compiler

There are two libraries ADXL343 handles the  communication with the sensor using I2C communication 
therefore the i2c_drive library is only included in the source file. the CommandHandle handles the command operation and first the command the decoded if it is a valid command
before it is processed.

ADXL343
The Command registers and values are defined as macros, the union structure is used for configuring and enabling 
the interrupts. this is exposed such the the user can enable needed interrupts and map the chosen interrup to the INT1 or INT2 pin
the needed functions by ADXL343 is declared static and defined in ADXL343.c limiting its call to only ADXL343 library. Two functions are defined in ADXl343.c.
Change_range()
get_XYZ()
These functions are defined in ADXL343 because they use some of the static functions and also the 
They are declared  in the CommandHandle.c using the extern keyword informing the compiler that the definition is somewhere else
This is done such that only the CommandHandle source file can call those function therefore not exposing those function to the main.c source file when the  ADXL343 header file is included.

Initialization
First the device_id is read out to determine if the sensor is properly connected to the hardware, then Interrupts are disabled as this is recommended to disable interrupts during initialization
then thresh_tap and tresh_act is initialized with the value for single tap interrupt and activity interrupt.
configures the axes used for the chosen functionality and set the FIFO in stream mode.


CommandHandle
This declares the needed commands, a struct is used to store the command to used by the program and also a flag to determine if a command has been set.
the handle_command determines which function to call from the ADXl343 library based on the command.

Change_range function
the available ranges and resoulutions is defined in enum adxl343_range_t and adxl343_res_t, two implementation are done in the Change_range function, one uses the switch_case statement 
to determine which hex value of the range to implement while the other uses the math.h library to compute the log2 of the chosen index and 1 is subtracted from the result to determine the range.


Assumptions
it was assumed that there was a nice implementation of I2C communication driver for communication between the microcontroller and the sensor
it is assumed the interrupts is set up to triger the data_ready variable
it is assumed the the range and resolution is changed with user input. An example format is"2g 10bit" or "2g full"
it is assumed the user wants to be informed of when the acceleration goes higher than chosen threshhold
it is assumed the user wants to be informed when acceleration goes higher and lower than the chosen threshold

Testing
Due to the asumption that there is a nice implementation of I2C driver, i asuumed that the function will return OK if there is no argument error

Limitations
Time constraints to simulate sensor responses
most microcontrollers do not have the needed library due to different compiler used
