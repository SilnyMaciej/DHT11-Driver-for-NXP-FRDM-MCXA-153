# Efficient DHT11 Driver for NXP MCX Series (32-bit Optimized)

A lightweight, high-performance C driver for the DHT11 temperature and humidity sensor, specifically designed for NXP MCX microcontrollers (tested on FRDM-MCXA153). 

**Optimization Note:** This driver uses a smart 32-bit packing technique. It stores the entire relevant payload—Humidity (Integral), Temperature (Integral), Temperature (Decimal), and Checksum—into a single `uint32_t` variable by omitting the unused Humidity decimal byte.

## Features
- **Self-Contained Logic**: Data acquisition and checksum validation are handled entirely within a single function call.
- **32-bit Data Packing**: Efficiency-first approach storing the filtered payload in one standard register.
- **Register-Level Access**: Uses direct GPIO register manipulation (`PDDR`, `PCOR`, `PDIR`) for maximum timing precision.
- **Minimal Overhead**: Direct bit-banging without heavy SDK abstraction layers.
- **Checksum Validation**: Real-time 8-bit integrity verification integrated into the return logic.

## Hardware Connection
The driver uses a single GPIO pin with a pull-up resistor. 

| DHT11 Pin | MCXA153 Pin | Note |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Ensure stable power supply |
| **DATA** | P3_6 (GPIO3, Pin 6) | Requires 4.7k - 10k Pull-up |
| **GND** | GND | Common ground |

*Note: If you are using a DHT11 module (on a small PCB), the pull-up resistor is likely already included.*



## Configuration & MCUXpresso Setup

Configure your **Pins Tool** as follows to generate the required macros:

1.  **Functional Group**: `DHT11_InitPins`.
2.  **Pin Identifier**: Label P3_6 as `DHT11`.
3.  **Electrical Settings**: 
    - Direction: **Output** (Initial state)
    - Input Buffer: **Enabled** (Required for reading data!)

## Data Structure (32-bit Layout)
If the checksum is valid, the function returns a `uint32_t` packed as follows:
- **Bits 31-24**: Humidity (Integral)
- **Bits 23-16**: Temperature (Integral)
- **Bits 15-8**: Temperature (Decimal)
- **Bits 7-0**: Checksum

## Usage Example
```c
#include "dht11_driver.h"
#include "fsl_debug_console.h"

int main(void) {
    // Standard Board Initialization...
    
    while(1) {
        // Function returns the packed data or 200 on error
        uint32_t sensor_data = DHT11_Get_Temperature_And_RH();

        if(sensor_data != 200) {
            uint8_t humidity    = (uint8_t)(sensor_data >> 24);
            uint8_t temp_int    = (uint8_t)(sensor_data >> 16);
            uint8_t temp_dec    = (uint8_t)(sensor_data >> 8);

            PRINTF("Humidity: %d %%\r\n", humidity);
            PRINTF("Temperature: %d.%d C\r\n", temp_int, temp_dec);
        } else {
            PRINTF("Sensor Error: Checksum mismatch or Timeout\r\n");
        }

        // Delay 2 seconds (DHT11 sampling rate limit)
        for(uint8_t i = 0; i < 20; i++) {
            delay_us(100000);
        }
    }
}
```
*Developed by: silnymaciej*
*Year: 2026*
