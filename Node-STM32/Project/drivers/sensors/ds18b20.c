#include "ds18b20.h"
#include "systick.h"
#include "common.h"
//DS18B20驱动函数
//正点原子@ALIENTEK
//2010/6/17

static GPIO_InitTypeDef GPIO_InitStructure;

//IO方向设置
#define DS18B20_IO_IN() do{GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);}while(0)
#define DS18B20_IO_OUT() do{GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);}while(0)
////IO操作函数
#define   DS18B20_DQ_OUT(x) GPIO_WriteBit(DS18B20_PORT, DS18B20_PIN, (x))
#define   DS18B20_DQ_IN() GPIO_ReadInputDataBit(DS18B20_PORT, DS18B20_PIN)

//复位DS18B20
void DS18B20_Rst(void)
{
    DS18B20_IO_OUT(); //SET PA0 OUTPUT
    DS18B20_DQ_OUT(0); //拉低DQ
    Delay_us(750);    //拉低750us
    DS18B20_DQ_OUT(1); //DQ=1
    Delay_us(15);     //15US
}
//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
uint8_t DS18B20_Check(void)
{
    uint8_t retry = 0;
    DS18B20_IO_IN();//SET PA0 INPUT
    while (DS18B20_DQ_IN() && retry < 200) {
        retry++;
        Delay_us(1);
    };
    if (retry >= 200)return 1;
    else retry = 0;
    while (!DS18B20_DQ_IN() && retry < 240) {
        retry++;
        Delay_us(1);
    };
    if (retry >= 240)return 1;
    return 0;
}
//从DS18B20读取一个位
//返回值：1/0
uint8_t DS18B20_Read_Bit(void)                                    // read one bit
{
    uint8_t data;
    DS18B20_IO_OUT();//SET PA0 OUTPUT
    DS18B20_DQ_OUT(0);
    Delay_us(2);
    DS18B20_DQ_OUT(1);
    DS18B20_IO_IN();//SET PA0 INPUT
    Delay_us(12);
    if (DS18B20_DQ_IN())data = 1;
    else data = 0;
    Delay_us(50);
    return data;
}
//从DS18B20读取一个字节
//返回值：读到的数据
uint8_t DS18B20_Read_Byte(void)    // read one byte
{
    uint8_t i, j, dat;
    dat = 0;
    for (i = 1; i <= 8; i++) {
        j = DS18B20_Read_Bit();
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}
//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t j;
    uint8_t testb;
    DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j = 1; j <= 8; j++) {
        testb = dat & 0x01;
        dat = dat >> 1;
        if (testb) {
            DS18B20_DQ_OUT(0); // Write 1
            Delay_us(2);
            DS18B20_DQ_OUT(1);
            Delay_us(60);
        } else {
            DS18B20_DQ_OUT(0); // Write 0
            Delay_us(60);
            DS18B20_DQ_OUT(1);
            Delay_us(2);
        }
    }
}
//开始温度转换
void DS18B20_Start(void)// ds1820 start convert
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}
//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在
uint8_t DS18B20_Init(void)
{
    RCC_GPIOClockCmd(DS18B20_PORT, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
    GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);
    DS18B20_Rst();
    return DS18B20_Check();
}
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250）
short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL, TH;
    short tem;
    DS18B20_Start();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert
    TL = DS18B20_Read_Byte(); // LSB
    TH = DS18B20_Read_Byte(); // MSB

    if (TH > 7) {
        TH = ~TH;
        TL = ~TL;
        temp = 0; //温度为负
    } else temp = 1; //温度为正
    tem = TH; //获得高八位
    tem <<= 8;
    tem += TL; //获得底八位
    tem = (int)tem * 625 / 1000; //转换
    if (temp)return tem; //返回温度值
    else return -tem;
}
