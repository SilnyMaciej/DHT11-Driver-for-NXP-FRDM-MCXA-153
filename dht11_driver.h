/*
 * dht11_driver.h
 *
 *  Created on: 24 kwi 2026
 *      Author: silnymaciej
 */

#ifndef DHT11_DRIVER_H_
#define DHT11_DRIVER_H_

#include "delay_us.h"
#include "clock_config.h"

#define DHT11_GPIO DHT11_INITPINS_DHT11_GPIO
#define DHT11_PIN_MASK DHT11_INITPINS_DHT11_GPIO_PIN_MASK
#define DHT11_TIMEOUT_CYCLES 100000U

typedef enum{
	GND_ERROR = 112,
	VCC_ERROR = 445,
	CHECK_SUM_ERROR = 554
}g_Sensor_Check_t;

uint32_t DHT11_Get_Temperature_And_RH(void);
#endif /* DHT11_DRIVER_H_ */
