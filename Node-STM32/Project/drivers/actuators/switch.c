#include "common.h"
#include "switch.h"

void Switch_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_GPIOClockCmd(RELAY_PORT, ENABLE);

    GPIO_InitStructure.GPIO_Pin = RELAY_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RELAY_PORT, &GPIO_InitStructure);
}

void Switch_Action(bool isON)
{
    GPIO_WriteBit(RELAY_PORT, RELAY_PIN, 
        isON ? RELAY_ACTIVE_VAL : (RELAY_ACTIVE_VAL^1));
    DBG_MSG("isON: %u", isON);
}

