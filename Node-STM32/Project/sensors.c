#include "common.h"
#include "sensors.h"
#include "bh1750.h"
#include "bmp180.h"
#include "dht.h"
#include "board.h"
#include "systick.h"

static bool sensor_bh1750_init(struct sensor_t *s)
{
    BH1750_Init();
    return true;
}

static bool sensor_bh1750_measure(struct sensor_t *s)
{
    s->value.value_int = BH1750_ReadLightLevel();
    return true;
}

static struct sensor_t sensor_bh1750 = {
    .model = "bh1750",
    .input_name = "illuminance",
    .unit = "lx",
    .value_type = SENSOR_VALUE_INT,
    .sample_rate = 5000, //ms
    .driver_init = sensor_bh1750_init,
    .measure = sensor_bh1750_measure,
};

static bool sensor_dht_init(struct sensor_t *s)
{
    DHT_Init(DHT11);
    return true;
}

static bool sensor_dht_temp_measure(struct sensor_t *s)
{
    s->value.value_float = DHT_ReadTemperature();
    return true;
}

static bool sensor_dht_humi_measure(struct sensor_t *s)
{
    s->value.value_float = DHT_ReadHumidity();
    return true;
}

static struct sensor_t sensor_dht_temp = {
    .model = "dht11",
    .input_name = "temperature",
    .unit = "C",
    .value_type = SENSOR_VALUE_FLOAT,
    .sample_rate = 2000, //ms
    .driver_init = sensor_dht_init,
    .measure = sensor_dht_temp_measure,
};

static struct sensor_t sensor_dht_humi = {
    .model = "dht11",
    .input_name = "humidity",
    .unit = "%RH",
    .value_type = SENSOR_VALUE_FLOAT,
    .sample_rate = 2000, //ms
    .driver_init = sensor_dht_init,
    .measure = sensor_dht_humi_measure,
};

static bool sensor_bmp180_init(struct sensor_t *s)
{
    return (bool)BMP180_Init();
}

static bool sensor_bmp180_measure(struct sensor_t *s)
{
    double T, P;
    int8_t delay;
    delay = BMP180_StartTemperature();
    if(!delay)
        return false;
    Delay_ms(delay);
    BMP180_GetTemperature(&T);
    delay = BMP180_StartPressure(1);
    if(!delay)
        return false;
    Delay_ms(delay);
    BMP180_GetPressure(&P, &T);

    s->value.value_float = P;
    return true;
}

static struct sensor_t sensor_bmp180 = {
    .model = "bmp180",
    .input_name = "pressure",
    .unit = "Pa",
    .value_type = SENSOR_VALUE_FLOAT,
    .sample_rate = 5000, //ms
    .driver_init = sensor_bmp180_init,
    .measure = sensor_bmp180_measure,
};

struct sensor_t *sensors[] = {
#ifdef ENABLE_BH1750
    &sensor_bh1750,
#endif
#ifdef ENABLE_DHT11
    &sensor_dht_temp,
    &sensor_dht_humi,
#endif
#ifdef ENABLE_BMP180
    &sensor_bmp180,
#endif
};

const int sensors_count = sizeof(sensors)/sizeof(struct sensor_t *);


