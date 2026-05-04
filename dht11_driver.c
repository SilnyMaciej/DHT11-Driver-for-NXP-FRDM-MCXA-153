#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "delay_us.h"

#define DHT11_GPIO DHT11_INITPINS_DHT11_GPIO
#define DHT11_PIN_MASK DHT11_INITPINS_DHT11_GPIO_PIN_MASK

uint32_t DHT11_Get_Temperature_And_RH(void){

    uint32_t data = 0;

    DHT11_GPIO->PCOR = DHT11_PIN_MASK;
    DHT11_GPIO->PDDR |= DHT11_PIN_MASK;


    delay_us(18000U);

    DHT11_GPIO->PDDR &= ~DHT11_PIN_MASK;
    delay_us(30U);


    while(DHT11_GPIO->PDIR & DHT11_PIN_MASK){
    	__asm("nop");
    }

    while(!(DHT11_GPIO->PDIR & DHT11_PIN_MASK)){
    	__asm("nop");
    }

    while(DHT11_GPIO->PDIR & DHT11_PIN_MASK){
       	__asm("nop");

    }

    for(uint8_t i = 0; i < 40; i++){

        while(!(DHT11_GPIO->PDIR & DHT11_PIN_MASK)){
        	__asm("nop");
        }

        delay_us(35U);

        if(DHT11_GPIO->PDIR & DHT11_PIN_MASK){

        	if (i < 8) 	 data |= (1U << (31 - i));

        	else if (i >= 16)  data |= (1U << (39 - i));


        	while(DHT11_GPIO->PDIR & DHT11_PIN_MASK){
        		__asm("nop");
        	}
        }
    }

        if(((uint8_t)(data >> 24) + (uint8_t)(data >> 16)) + (uint8_t)((data >> 8)) == (uint8_t)(data & 0xFF)) {return data;}

    return 200;
}
