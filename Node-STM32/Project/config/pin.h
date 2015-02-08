#ifndef PIN_H__
#define PIN_H__

#define RELAY_PORT  GPIOA
#define RELAY_PIN   GPIO_Pin_0

#define BMP180_PORT    GPIOB
#define BMP180_SDA_PIN GPIO_Pin_7
#define BMP180_SCL_PIN GPIO_Pin_6

#define BH1750_PORT    GPIOB
#define BH1750_SDA_PIN GPIO_Pin_7
#define BH1750_SCL_PIN GPIO_Pin_6

#define DHT11_PORT GPIOA
#define DHT11_PIN  GPIO_Pin_1

#define DS18B20_PORT GPIOA
#define DS18B20_PIN  GPIO_Pin_1

#define IR_EMIT_PORT GPIOA
#define IR_EMIT_PIN  GPIO_Pin_6

#define IR_RECV_PORT GPIOA
#define IR_RECV_PIN  GPIO_Pin_0

#define EEP24Cxx_PORT    GPIOB
#define EEP24Cxx_SDA_PIN GPIO_Pin_7
#define EEP24Cxx_SCL_PIN GPIO_Pin_6

#define LED_BRD_PORT  GPIOB
#define LED_BRD_B_PIN GPIO_Pin_9
#define LED_BRD_G_PIN GPIO_Pin_8

#define USART_DBG     USART1

#endif
