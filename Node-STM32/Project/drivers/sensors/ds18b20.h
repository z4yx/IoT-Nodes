#ifndef __DS18B20_H
#define __DS18B20_H
//DS18B20驱动函数
//正点原子@ALIENTEK
//2010/6/17
#include <stdint.h>

uint8_t DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
uint8_t DS18B20_Read_Byte(void);//读出一个字节
uint8_t DS18B20_Read_Bit(void);//读出一个位
uint8_t DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20
#endif