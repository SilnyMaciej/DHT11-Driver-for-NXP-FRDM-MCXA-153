/*
 * dht11_driver.h
 *
 *  Created on: 24 kwi 2026
 *      Author: silnymaciej
 */

#ifndef DHT11_DRIVER_H_
#define DHT11_DRIVER_H_


int8_t DHT11_Get_Temperature_And_RH(uint8_t *temp_int,uint8_t *temp_dec,uint8_t *rh_int,uint8_t *rh_dec);
#endif /* DHT11_DRIVER_H_ */
