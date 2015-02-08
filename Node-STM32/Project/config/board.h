#ifndef BOARD_H__
#define BOARD_H__

#define BOARD_CUSTOM

#if defined(BOARD_LIGHT_SENSOR)

#define ENABLE_BH1750

#elif defined(BOARD_TEMP_HUMI)

#define ENABLE_DHT11

#elif defined(BOARD_CUSTOM)

#define ENABLE_DHT11

#else
#error Must choose one board
#endif
