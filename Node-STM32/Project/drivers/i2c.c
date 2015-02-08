#include "i2c.h"
#include "common.h"
#include "cpal.h"

void I2C_Lib_WaitForRWDone(void)
{
    while ((I2C_HOST_DEV.CPAL_State != CPAL_STATE_READY) && (I2C_HOST_DEV.CPAL_State != CPAL_STATE_ERROR) )
    { }
}

void I2C_Lib_Init()
{
    static bool initialized = false;
    if(initialized)
        return;
    initialized = true;

    CPAL_I2C_StructInit(&I2C_HOST_DEV);

    // I2C_HOST_DEV.CPAL_Dev is already initialized in definition
    I2C_HOST_DEV.pCPAL_I2C_Struct->I2C_ClockSpeed = 350000;
    I2C_HOST_DEV.pCPAL_I2C_Struct->I2C_OwnAddress1 = 0xAA;

    CPAL_I2C_Init(&I2C_HOST_DEV);
}

void CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t Device_Error) 
{
    DBG_MSG("Error: %d", Device_Error);
}


