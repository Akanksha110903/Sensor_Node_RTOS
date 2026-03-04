#include "cli.h"
extern volatile uint8_t pwm_percentage;

volatile char cmdBuf[CMD_BUF_SIZE];
volatile uint8_t cmdIndex = 0;
char txBuf[64];

void CLI_ParseCommand(char *cmd)
{
    char meg[128];
    float tem;
    if(strcmp(cmd, "help")==0)
    {
        menu();
    }
    else if(strncmp(cmd, "set_pwm", 7)==0)
    {
        uint8_t percentage = 0;
        for(int i = 8; cmd[i] >= '0' && cmd[i] <= '9'; i++)
        {
           percentage = percentage * 10 + (cmd[i] - '0');
        }
        pwm_percentage = percentage;
        xTaskNotifyGive(control_task_handle);//notify control task to update the duty cycle of pwm signal according to the percentage value set by cli task
        snprintf(meg, sizeof(meg), "\r\nPWM duty cycle set to %d%%\r\n", percentage);
        xQueueSend(printDataQueue, meg, portMAX_DELAY);//send the response string to print task to print
        print_menu();//send the menu string to print task to print
    }
    else if(strcmp(cmd, "get_temp")==0)
    {
        xSemaphoreTake(sensorMutex, portMAX_DELAY);//mutex take
        tem = g_sensorData.temperature;
        xSemaphoreGive(sensorMutex);//mutex give
        snprintf(meg, sizeof(meg), "\r\nTemperature: %.2f C\r\n", tem);
        xQueueSend(printDataQueue, meg, portMAX_DELAY);//send the response string to print task to print
        print_menu();//send the menu string to print task to print

    }else if(strcmp(cmd, "get_accel")==0)
    {
        int16_t ax, ay, az;
        xSemaphoreTake(sensorMutex, portMAX_DELAY);//mutex take
        ax = g_sensorData.accel_x;
        ay = g_sensorData.accel_y;
        az = g_sensorData.accel_z;
        xSemaphoreGive(sensorMutex);//mutex give
        snprintf(meg, sizeof(meg), "\r\nAccelerometer: X=%d, Y=%d, Z=%d\r\n", ax, ay, az);
        xQueueSend(printDataQueue, meg, portMAX_DELAY);//send the response string to print task to print
        print_menu();//send the menu string to print task to print

    }
    else if(strcmp(cmd, "get_info")==0)
    {
        snprintf(meg, sizeof(meg), "\r\nSystem Information:\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);
        snprintf(meg, sizeof(meg), "Firmware Version: 1.0.0\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);
        snprintf(meg, sizeof(meg), "Build Date: %s %s\r\n", __DATE__, __TIME__);
        xQueueSend(printDataQueue, meg, portMAX_DELAY);
        print_menu();//send the menu string to print task to print
    }
    else if(strcmp(cmd, "do 1")==0)
    {
        //set the digital output to high
        /*HAL_GPIO_WritePin(GPIOD, LL_GPIO_PIN_12, GPIO_PIN_SET) make this ll code;*/
        snprintf(meg, sizeof(meg), "\r\nDigital output set to HIGH\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);//send the response string to print task to print
        print_menu();//send the menu string to print task to print
    }
    else if(strcmp(cmd, "do 0")==0)
    {
        //set the digital output to low
        /*HAL_GPIO_WritePin(GPIOD, LL_GPIO_PIN_12, GPIO_PIN_RESET) make this ll code;*/
        snprintf(meg, sizeof(meg), "\r\nDigital output set to LOW\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);//send the response string to print task to print
        print_menu();//send the menu string to print task to print
    }
    else if(strcmp(cmd, "modbus")==0)
    {
        snprintf(meg, sizeof(meg), "\r\nModbus Configuration:\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);
        snprintf(meg, sizeof(meg), "Baud Rate: 115200\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);
        snprintf(meg, sizeof(meg), "Data Bits: 8\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);
        snprintf(meg, sizeof(meg), "Stop Bits: 1\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);
        snprintf(meg, sizeof(meg), "Parity: None\r\n");
        xQueueSend(printDataQueue, meg, portMAX_DELAY);
        print_menu();//send the menu string to print task to print
    }
    else
    {
        snprintf(meg, sizeof(meg), "\r\nUnknown command: %s\r\n", cmd);
        xQueueSend(printDataQueue, meg, portMAX_DELAY);//send the response string to print task to print
        print_menu();//send the menu string to print task to print
    }
}


void print_menu(void)
{
    char msg[128];
    snprintf(msg, sizeof(msg), "\r\nEnter the command or \"help\" to see the menu list of commands:\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);//send the menu string to print task to print
}

void menu(void)
{
    char msg[128];
    snprintf(msg, sizeof(msg), "\r\nAvailable commands:\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);//send the menu string to
    snprintf(msg, sizeof(msg), "1. help - Show this menu\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);
    snprintf(msg, sizeof(msg), "2. set_pwm <percentage> - Set PWM duty cycle (0-100%%)\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);
    snprintf(msg, sizeof(msg), "3. get_temp - Get temperature reading\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);
    snprintf(msg, sizeof(msg), "4. get_accel - Get accelerometer readings\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);
    snprintf(msg, sizeof(msg), "5. get_info - Get system information\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);
    snprintf(msg, sizeof(msg), "6. do <0|1>  - set the digital output\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);
    snprintf(msg, sizeof(msg), "7. modbus - get Modbus configuration\r\n");
    xQueueSend(printDataQueue, msg, portMAX_DELAY);
}


