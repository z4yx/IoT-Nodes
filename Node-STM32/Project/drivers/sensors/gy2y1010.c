#include "gp2y1010.h"
#include "analog.h"
#include "systick.h"
#include "pwm.h"
#include "common.h"

bool GP2Y1010_Init()
{
    PWM_Init(100);
    PWM_Channel_Precise(GP2Y1010_IR_PWM_OUT_CH, 320, true); //IR control active high
    Analog_SetChannel(GP2Y1010_ADC_CHANNEL, true);
    return true;
}

float GP2Y1010_Measure()
{
    uint16_t adc;
    while(TIM_GetCounter(GP2Y1010_IR_PWM_TIM) != 1);
    Delay_us(280);
    adc = Analog_GetChannelValue(GP2Y1010_ADC_CHANNEL);
    // DBG_MSG("counter=%d", (int)TIM_GetCounter(GP2Y1010_IR_PWM_TIM));
    return ADC2MilliVolts((float)adc);
}