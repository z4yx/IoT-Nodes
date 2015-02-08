#ifndef ESP8266_H__
#define ESP8266_H__


void ESP8266_Init(void);
void ESP8266_Enable_CDC_Forwarding(void);

void ESP8266_USART_IT_Handler(void);

#endif