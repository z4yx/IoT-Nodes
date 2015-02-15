#include "iot_node.h"
#include "common.h"
#include "esp8266.h"
#include "sensors.h"
#include "systick.h"

static void initSensors()
{
    for (int i = 0; i < sensors_count; ++i) {
        if (sensors[i]->driver_init(sensors[i])) {
            sensors[i]->flags |= SENSOR_FLAG_INITIALIZED;
        } else {
            ERR_MSG("Initialization of sensor %s failed",
                    sensors[i] -> model);
        }
    }
}

static void publishMeasurement(struct sensor_t *s)
{
    static char value_buf[16];
    if(!ESP8266_IsMqttConnected())
        return;
    if (s->value_type == SENSOR_VALUE_INT)
        snprintf(value_buf, sizeof(value_buf), "%d %s",
                 s->value.value_int,
                 s->unit);
    else if (s->value_type == SENSOR_VALUE_FLOAT)
        snprintf(value_buf, sizeof(value_buf), "%.2f %s",
                 s->value.value_float,
                 s->unit);
    else {
        ERR_MSG("Unknown value type of %s", s->model);
        return;
    }
    DBG_MSG("%s-%s: %s", s->model, s->input_name, value_buf);
    ESP8266_MqttPublishValue(s->input_name, value_buf);
}

static void updateMeasurement()
{
    for (int i = 0; i < sensors_count; ++i) {
        SysTick_t now = GetSystemTick();
        if (!sensors[i]->flags & SENSOR_FLAG_INITIALIZED)
            continue;
        if (now - sensors[i]->latest_sample < sensors[i]->sample_rate)
            continue;
        if (sensors[i]->measure(sensors[i])) {
            sensors[i]->latest_sample = now;
            publishMeasurement(sensors[i]);
        } else {
            ERR_MSG("Measuring %s on %s failed",
                    sensors[i]->input_name,
                    sensors[i]->model);
        }
    }
}

static void initNetwork(void)
{
    uint32_t id[3];
    char name_buf[32];
    
    while (!ESP8266_IsStarted());
    do {
        Delay_ms(1000);
        ESP8266_CheckWifiState();
        Delay_ms(300);
    } while (!ESP8266_IsWifiConnected());

    Chip_GetUniqueID(id);
    snprintf(name_buf, sizeof(name_buf),
             "IoT_%08x%08x%08x", id[0], id[1], id[2]);
    ESP8266_InitMqtt(name_buf);
    ESP8266_MqttConnect("192.168.1.103", 1883);
    while (!ESP8266_IsMqttConnected());
    DBG_MSG("MQTT connected");
}

void IoTNode_Begin()
{
    DBG_MSG("%d sensors defined", sensors_count);
    initSensors();
    initNetwork();
    while (1) {
        updateMeasurement();
    }
}