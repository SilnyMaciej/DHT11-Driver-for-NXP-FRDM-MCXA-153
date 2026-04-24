# Efficient DHT11 Driver for NXP MCX Series

A lightweight, high-performance C driver for the DHT11 temperature and humidity sensor, specifically designed for NXP MCX microcontrollers (tested on FRDM-MCXA153).

##  Features
- **Register-Level Access**: Uses direct GPIO register manipulation (`PDDR`, `PSOR`, `PCOR`, `PDIR`) for maximum timing precision.
- **Minimal Overhead**: No reliance on heavy SDK functions for bit-banging.
- **Float-free**: Handles integer and decimal parts separately to save memory and processing power.
- **Checksum Validation**: Full data integrity verification for reliable readings.

## 🛠 Hardware Connection
The driver uses a single GPIO pin with a pull-up resistor. 

| DHT11 Pin | MCXA153 Pin | Note |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Ensure stable power supply |
| **DATA** | P3_6 (GPIO3, Pin 6) | Requires 4.7k - 10k Pull-up |
| **GND** | GND | Common ground |

*Note: If you are using a DHT11 module (on a small PCB), the pull-up resistor is likely already included.*

## 💻 Usage

1. **Pin Configuration**: Configure your pin in MCUXpresso Config Tools as a GPIO output/input.
2. **Include**: Add `dht11_driver.c` and `dht11_driver.h` to your project.
3. **Read Data**:
```c
uint8_t t_int, t_dec, rh_int, rh_dec;

if (DHT11_Get_Temperature_And_RH(&t_int, &t_dec, &rh_int, &rh_dec) == 1) {
    PRINTF("Temp: %d.%d C, Humidity: %d.%d %%\r\n", t_int, t_dec, rh_int, rh_dec);
} else {
    // Handle error (checksum or timeout)
}
