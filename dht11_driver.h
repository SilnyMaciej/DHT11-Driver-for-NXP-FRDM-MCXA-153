/*
 * dht11_driver.h
 *
 *  Created on: 24 kwi 2026
 *      Author: silnymaciej
 */

#ifndef DHT11_DRIVER_H_
#define DHT11_DRIVER_H_

#include "delay_us.h"

#define DHT11_GPIO DHT11_INITPINS_DHT11_GPIO
#define DHT11_PIN_MASK DHT11_INITPINS_DHT11_GPIO_PIN_MASK

uint32_t DHT11_Get_Temperature_And_RH(void);
#endif /* DHT11_DRIVER_H_ */
