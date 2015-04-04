#ifndef BOARD_H__
#define BOARD_H__

//===select a board here===
#define BOARD_GAS
//=========================

#if defined(BOARD_LIGHT_SENSOR)

#define ENABLE_BH1750

#elif defined(BOARD_TEMP_HUMI)

#define ENABLE_DHT11

#elif defined(BOARD_PRESSURE)

#define ENABLE_BMP180

#elif defined(BOARD_RELAY)

#define ENABLE_SWITCH

#elif defined(BOARD_PYROELECTRICITY)

#define ENABLE_SR501

#elif defined(BOARD_GAS)

#define ENABLE_MQ2

#elif defined(BOARD_CUSTOM)

#define ENABLE_DHT11
#define ENABLE_BMP180

#else
#error Must choose one board
#endif

#endif
