#include "common.h"
#include "esp8266.h"
#include "usart.h"
#include "usbcdc.h"

static bool CDC_Forwarding;

static void RedirectUSBToESP8266(uint8_t* data_buffer, uint8_t Nb_bytes)
{
    for (int i = 0; i < Nb_bytes; ++i)
    {
        USART_putchar(ESP8266_USART, data_buffer[i]);
    }
}

void ESP8266_Init()
{
    CDC_Forwarding = false;
    USARTx_Config(ESP8266_USART, 9600);
    USART_RxInt_Config(true, ESP8266_USART, ESP8266_USART_IRQ);
}

void ESP8266_Enable_CDC_Forwarding()
{
    USBCDC_SetReceiveCallback(RedirectUSBToESP8266);
    CDC_Forwarding = true;
}

void ESP8266_USART_IT_Handler()
{
    if(USART_GetITStatus(ESP8266_USART, USART_IT_RXNE) == SET){
        uint8_t byte = USART_ReceiveData(ESP8266_USART);
        if(CDC_Forwarding)
            USBCDC_SendByte(byte);
    }

}
