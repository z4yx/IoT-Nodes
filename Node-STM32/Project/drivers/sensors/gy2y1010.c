#include "gp2y1010.h"
#include "analog.h"
#include "systick.h"
#include "common.h"

bool GP2Y1010_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = GP2Y1010_IR_PIN;
    RCC_GPIOClockCmd(GP2Y1010_IR_PORT, ENABLE);
    GPIO_Init(GP2Y1010_IR_PORT, &GPIO_InitStructure);
    Analog_SetChannel(GP2Y1010_ADC_CHANNEL, true);
    return true;
}

float GP2Y1010_Measure()
{
    uint16_t adc;
    GPIO_WriteBit(GP2Y1010_IR_PORT, GP2Y1010_IR_PIN, 0);
    Delay_us(280);
    adc = Analog_GetChannelValue(GP2Y1010_ADC_CHANNEL);
    GPIO_WriteBit(GP2Y1010_IR_PORT, GP2Y1010_IR_PIN, 1); 
    return ADC2MilliVolts((float)adc);
}