#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "delay_us.h"

int8_t DHT11_Get_Temperature_And_RH(uint8_t *temp_int,uint8_t *temp_dec,uint8_t *rh_int,uint8_t *rh_dec){
    uint8_t data[5] = {0};

    DHT11_INITPINS_DHT11_GPIO->PDDR |= DHT11_INITPINS_DHT11_GPIO_PIN_MASK;

    DHT11_INITPINS_DHT11_GPIO->PCOR = DHT11_INITPINS_DHT11_GPIO_PIN_MASK;
    delay_us(18000U);

    DHT11_INITPINS_DHT11_GPIO->PSOR =  DHT11_INITPINS_DHT11_GPIO_PIN_MASK;
    delay_us(30U);

    DHT11_INITPINS_DHT11_GPIO->PDDR &= ~ DHT11_INITPINS_DHT11_GPIO_PIN_MASK;

    while(DHT11_INITPINS_DHT11_GPIO->PDIR &  DHT11_INITPINS_DHT11_GPIO_PIN_MASK){
    	__asm("nop");
    }

    while(!(DHT11_INITPINS_DHT11_GPIO->PDIR &  DHT11_INITPINS_DHT11_GPIO_PIN_MASK)){
    	__asm("nop");
    }

    while(DHT11_INITPINS_DHT11_GPIO->PDIR &  DHT11_INITPINS_DHT11_GPIO_PIN_MASK){
       	__asm("nop");

    }

    for(uint8_t i = 0; i < 40; i++){

        while(!(DHT11_INITPINS_DHT11_GPIO->PDIR &  DHT11_INITPINS_DHT11_GPIO_PIN_MASK)){
        	__asm("nop");
        }

        delay_us(35U);

        if(DHT11_INITPINS_DHT11_GPIO->PDIR &  DHT11_INITPINS_DHT11_GPIO_PIN_MASK){
            data[i / 8] |= (1U << (7U - (i % 8U)));

            while(DHT11_INITPINS_DHT11_GPIO->PDIR &  DHT11_INITPINS_DHT11_GPIO_PIN_MASK){
            	__asm("nop");
            }
        }
    }
    if(((uint8_t)(*(data) + *(data + 1) + *(data + 2) + * (data + 3)) == *(data + 4))){
        *rh_int = *data;
        *rh_dec = *(data + 1);
        *temp_int = *(data + 2);
        *temp_dec = *(data + 3);
        return 1;
    }
    return -1;
}
