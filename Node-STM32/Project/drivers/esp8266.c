#include "common.h"
#include "esp8266.h"
#include "usart.h"
#include "usbcdc.h"
#include "systick.h"
#include "iot_node.h"

static bool CDC_Forwarding;
static bool wifi_connected;
static bool mqtt_connected;
static bool esp8266_stated;

static void RedirectUSBToESP8266(uint8_t* data_buffer, uint8_t Nb_bytes)
{
    for (int i = 0; i < Nb_bytes; ++i)
    {
        USART_putchar(ESP8266_USART, data_buffer[i]);
    }
}

static void gotResponse(char *token, char *param)
{
    DBG_MSG("[[%s]][[%s]]\n", token, param);
    if(strcmp(token, "control") == 0)
        IoTNode_HandleControl(param);
    if(strcmp(token, "connected") == 0)
        mqtt_connected = true;
    else if(strcmp(token, "offline") == 0)
        mqtt_connected = false;
    else if(strcmp(token, "wifi") == 0){
        DBG_MSG("Wifi state: %s", param);
        if(*param == '5')
            wifi_connected = true;
    }else if(strcmp(token, "started") == 0)
        esp8266_stated = true;
}

static void parse8266Output(uint8_t in)
{
    enum{STATE_WAIT, STATE_TOKEN, STATE_PARAM};
    static uint8_t state = STATE_WAIT;
    static int tokenLen, paramLen;
    static char tokenBuf[16], paramBuf[16];
    switch(state){
    case STATE_WAIT:
        if(in == '#'){
            tokenLen = 0;
            state = STATE_TOKEN;
        }
        break;
    case STATE_TOKEN:
        if(in == '+' || in == '\r' || in == '\n'){
            tokenBuf[tokenLen] = '\0';
            if(in == '+'){
                paramLen = 0;
                state = STATE_PARAM;
            }else{
                gotResponse(tokenBuf, NULL);
                state = STATE_WAIT;
            }
        }else if(tokenLen+1 < sizeof(tokenBuf)){
            tokenBuf[tokenLen++] = in;
        }
        break;
    case STATE_PARAM:
        if(in == '\r' || in == '\n'){
            paramBuf[paramLen] = '\0';
            gotResponse(tokenBuf, paramBuf);
            state = STATE_WAIT;
        }else if(paramLen+1 < sizeof(paramBuf)){
            paramBuf[paramLen++] = in;
        }
        break;

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
        parse8266Output(byte);
    }

}

void ESP8266_CheckWifiState()
{
    USART_puts(ESP8266_USART, "print('\\035wifi+'..wifi.sta.status())");
}

bool ESP8266_IsStarted()
{
    return esp8266_stated;
} 

bool ESP8266_IsWifiConnected()
{
    return wifi_connected;
}

void ESP8266_InitMqtt(char *name)
{
    USART_printf(ESP8266_USART, "c=require('comm');c.init('%s')\r", name);
    Delay_ms(200);
}

void ESP8266_MqttConnect(char *ip, int port)
{
    //connect is async
    USART_printf(ESP8266_USART, "c.connect('%s',%d)\r", ip, port);
    // USART_puts(ESP8266_USART, "c.enable_config()");
    Delay_ms(200);
}

bool ESP8266_IsMqttConnected()
{
    return mqtt_connected;
}

void ESP8266_MqttPublishValue(char *key, char *value)
{
    USART_printf(ESP8266_USART, "c.publish('values','%s','%s',1,1)\r", key, value);
    Delay_ms(300);
}

void ESP8266_MqttPublishEvent(char *key, char *value)
{
    USART_printf(ESP8266_USART, "c.publish('events','%s','%s',2,0)\r", key, value);
    Delay_ms(300);
}

