#ifndef RTOS_HANDLES_H
#define RTOS_HANDLES_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


typedef struct
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;

    float temperature;
    float voltage;

} sensor_state_t;


extern TaskHandle_t communication_task_handle;
extern TaskHandle_t adc_task_handle;
extern TaskHandle_t accelerometer_task_handle;
extern TaskHandle_t control_task_handle;
extern TaskHandle_t cli_task_handle;
extern TaskHandle_t print_task_handle;
extern TaskHandle_t monitor_task_handle;
extern sensor_state_t g_sensorData;
extern SemaphoreHandle_t sensorMutex;
extern QueueHandle_t printDataQueue;


#endif /* RTOS_HANDLES_H */