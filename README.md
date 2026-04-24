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

## ⚙️ Configuration & Pin Modification

To make the driver work with your specific hardware setup, you must ensure that the following macros are defined (typically in `pin_mux.h` or at the top of your driver file). 

**If you change the pin, you must update the Port, Pin number, and Bit Mask accordingly:**

```c
/* Symbols to be used with GPIO driver */
#define DHT11_INITPINS_DHT11_GPIO GPIO3               /* GPIO Peripheral (e.g., GPIO1, GPIO3) */
#define DHT11_INITPINS_DHT11_INIT_GPIO_VALUE 0U       
#define DHT11_INITPINS_DHT11_GPIO_PIN 6U              /* Pin number on that GPIO */
#define DHT11_INITPINS_DHT11_GPIO_PIN_MASK (1U << 6U) /* Bit mask for the pin */

/* Symbols to be used with PORT driver */
#define DHT11_INITPINS_DHT11_PORT PORT3               /* Port Peripheral (e.g., PORT1, PORT3) */
#define DHT11_INITPINS_DHT11_PIN 6U                   
#define DHT11_INITPINS_DHT11_PIN_MASK (1U << 6U)
```
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
```
