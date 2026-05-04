# Efficient DHT11 Driver for NXP MCX Series (32-bit Optimized)

A lightweight, high-performance C driver for the DHT11 temperature and humidity sensor, specifically designed for NXP MCX microcontrollers (tested on FRDM-MCXA153). 

**Optimization Note:** This driver uses a smart 32-bit packing technique to store humidity, temperature, and checksum in a single `uint32_t` variable. It omits the unused RH decimal byte (which is always 0 in DHT11) to fit the entire relevant payload into a single standard register.

## Features
- **32-bit Data Packing**: Efficiency-first approach storing the entire payload in one variable.
- **Register-Level Access**: Uses direct GPIO register manipulation (`PDDR`, `PCOR`, `PDIR`) for maximum timing precision.
- **Minimal Overhead**: No reliance on heavy SDK functions for bit-banging.
- **Float-free**: Handles integer and decimal parts separately to save memory and processing power.
- **Checksum Validation**: Full 8-bit data integrity verification according to DHT11 protocol.

## Hardware Connection
The driver uses a single GPIO pin with a pull-up resistor. 

| DHT11 Pin | MCXA153 Pin | Note |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Ensure stable power supply |
| **DATA** | P3_6 (GPIO3, Pin 6) | Requires 4.7k - 10k Pull-up |
| **GND** | GND | Common ground |

*Note: If you are using a DHT11 module (on a small PCB), the pull-up resistor is likely already included.*

## Configuration & MCUXpresso Setup

To ensure the driver works with your `pin_mux.c` configuration, set up the **Pins Tool** as follows:

1.  **Functional Group**: Create or use a group named `DHT11_InitPins`.
2.  **Pin Identifier**: Set the label/identifier for P3_6 to `DHT11`.
3.  **Electrical Settings**: 
    - Direction: **Output**
    - Input Buffer: **Enabled** (Crucial for reading data!)

## Data Structure (32-bit Layout)
The `uint32_t` returned by the driver is packed as follows to optimize memory:
- **Bits 31-24**: Humidity (Integral)
- **Bits 23-16**: Temperature (Integral)
- **Bits 15-8**: Temperature (Decimal)
- **Bits 7-0**: Checksum

## Usage Example

```c
while(1) {
    // Get packed 32-bit data
    uint32_t data = DHT11_Get_Temperature_And_RH();

    // 200 is our custom error code for Checksum/Timeout
    if(data != 200) {
        uint8_t rh   = (uint8_t)(data >> 24);
        uint8_t t_i  = (uint8_t)(data >> 16);
        uint8_t t_d  = (uint8_t)(data >> 8);

        PRINTF("Humidity: %d %%\r\n", rh);
        PRINTF("Temperature: %d.%d C\r\n", t_i, t_d);
    } else {
        PRINTF("Checksum or Timeout Error!\r\n");
    }

    // Wait 2 seconds between readings (DHT11 protocol requirement)
    for(uint8_t i = 0; i < 20; i++) {
        delay_us(100000);
    }
}
```
Developed by: Maciej Mosio
Year: 2026
