#ifndef __CLOCK_CONFIG_H
#define __CLOCK_CONFIG_H
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_gpio.h"

void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void Error_Handler(void);

#endif /*__CLOCK_CONFIG_H */ 