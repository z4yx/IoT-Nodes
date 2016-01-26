#ifndef BOARD_H__
#define BOARD_H__

//===修改这个定义来选择外设===
#define BOARD_RELAY
//=========================

#if defined(BOARD_LIGHT_SENSOR)  //选择光线传感器

#define ENABLE_BH1750

#elif defined(BOARD_TEMP_HUMI)  //选择湿度传感器

#define ENABLE_DHT11

#elif defined(BOARD_PRESSURE)  //选择气压传感器

#define ENABLE_BMP180

#elif defined(BOARD_RELAY)  //选择继电器

#define ENABLE_SWITCH

#elif defined(BOARD_PWM)  //选择PWM输出，舵机、功率控制等

#define ENABLE_PWM

#elif defined(BOARD_PYROELECTRICITY)  //选择热释电传感器

#define ENABLE_SR501

#elif defined(BOARD_GAS)  //选择可燃气体传感器

#define ENABLE_MQ2

#elif defined(BOARD_IR)  //选择红外遥控

#define ENABLE_IR

#elif defined(BOARD_RFID)  //选择射频卡读卡器

#define ENABLE_PN532

#elif defined(BOARD_CUSTOM)  //自定义传感器、执行器

#define ENABLE_DHT11
#define ENABLE_BMP180

#else
#error Must choose one board
#endif

#endif
