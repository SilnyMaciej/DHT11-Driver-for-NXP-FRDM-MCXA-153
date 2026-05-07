#include "pin_mux.h"
#include "dht11_driver.h"


uint32_t DHT11_Get_Temperature_And_RH(void){

	volatile uint32_t timeout = DHT11_TIMEOUT_CYCLES;
    uint8_t data[5] = {0};

    DHT11_GPIO->PCOR = DHT11_PIN_MASK;
    DHT11_GPIO->PDDR |= DHT11_PIN_MASK;


    delay_us(18000U);

    DHT11_GPIO->PDDR &= ~DHT11_PIN_MASK;

    delay_us(42U);

    while(!(DHT11_GPIO->PDIR & DHT11_PIN_MASK)){
    	__asm("nop");
    	 if(--timeout == 0) return GND_ERROR;
    }


    timeout = DHT11_TIMEOUT_CYCLES;

    while(DHT11_GPIO->PDIR & DHT11_PIN_MASK){
    	__asm("nop");

    	if(--timeout == 0) return VCC_ERROR;

    }

    uint8_t bit = 0;

    for(uint8_t i = 0; i < 40; i++){

    	timeout = DHT11_TIMEOUT_CYCLES;

    	while(!(DHT11_GPIO->PDIR & DHT11_PIN_MASK)){
    		__asm("nop");

    		if(--timeout == 0) return GND_ERROR;
    	 }

        delay_us(35U);

        bit = (DHT11_GPIO->PDIR & DHT11_PIN_MASK) != 0;
        *(data + (i >> 3)) |= (bit << (7 - (i & 7)));

        while(DHT11_GPIO->PDIR & DHT11_PIN_MASK){
        	__asm("nop");

        	if(--timeout == 0) return VCC_ERROR;
        }


    }
    
   if(*data + *(data + 1) + *(data + 2) + *(data + 3) == *(data + 4)){

	  return ((uint32_t)(*(data)) << 24) | ((uint32_t)(*(data + 1)) << 16) | ((uint32_t)(*(data + 2)) << 8) | ((uint32_t)(*(data + 3)));
   }
   return CHECK_SUM_ERROR;

}
