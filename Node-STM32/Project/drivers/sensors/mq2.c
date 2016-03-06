#include "mq2.h"
#include "analog.h"
#include "common.h"

void MQ2_Init()
{
    Analog_SetChannel(MQ2_ADC_CHANNEL, true);
}

float MQ2_Read()
{
    return ADC2MilliVolts(Analog_GetChannelValue(MQ2_ADC_CHANNEL));
}