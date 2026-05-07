# DHT11 Robust Driver for NXP MCX Series (32-bit Optimized)

A professional, high-performance C driver for the DHT11 temperature and humidity sensor, specifically optimized for NXP MCX microcontrollers (tested on **FRDM-MCXA153**).

## Key Features
- **Fail-Safe Mechanism**: Every `while` loop is protected by `DHT11_TIMEOUT_CYCLES`. This ensures the CPU never hangs if the sensor is physically disconnected or malfunctions.
- **32-bit Data Packing**: Data is packed into a single `uint32_t` variable, making it highly efficient for transport via UART, CAN, or BLE.
- **Optimization Proof**: Uses `volatile` qualifiers and `__asm("nop")` instructions to ensure consistent timing and timeout behavior regardless of compiler optimization levels (-O0, -O2, -Os).
- **Advanced Diagnostics**: Includes built-in error codes to distinguish between wiring shorts, power issues, and data corruption.

## Technical Specifications

### 32-bit Data Layout
Upon a successful checksum validation, the function returns a `uint32_t` formatted as follows:
- **Bits 31-24**: Humidity (Integral part)
- **Bits 23-16**: Humidity (Decimal part) — *Note: Usually 0 on DHT11*
- **Bits 15-8**: Temperature (Integral part)
- **Bits 7-0**: Temperature (Decimal part)

### Error Codes (`g_Sensor_Check_t`)
In case of failure, the function returns one of the following constants:
- `112` (**GND_ERROR**): Data line stuck LOW (possible short to ground).
- `445` (**VCC_ERROR**): No response from sensor (line pulled HIGH, sensor not detected).
- `554` (**CHECK_SUM_ERROR**): Data received but failed the 8-bit integrity check.

## Hardware Configuration (Pinout)

| DHT11 Pin | MCXA153 Pin | Description |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Power Supply |
| **DATA** | P3_6 (GPIO3-6) | Requires 4.7kΩ - 10kΩ Pull-up resistor |
| **GND** | GND | Common Ground |

**MCUXpresso Pins Tool Requirement:**
The Data Pin must have the **Input Buffer Enabled** in the electrical configuration. Without this, the CPU cannot read the logic levels sent by the sensor.

## File Structure

### `dht11_driver.h` / `.c`
Contains the core driver logic. The `DHT11_TIMEOUT_CYCLES` is set to 200,000 iterations, providing a safe 1-2ms margin at a 96MHz clock speed.

### `delay_us.h` / `.c`
Utilizes the **SysTick** timer to provide microsecond precision. The SysTick interrupt must be configured in `peripherals.c` with a 1 µs period for accurate timing.

## Usage Example

```c
#include "dht11_driver.h"
#include "fsl_debug_console.h"

int main(void) {
    // Hardware Initialization
    BOARD_InitBootPins();
    DHT11_InitPins(); // Ensure the sensor pin is initialized
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    while(1) {
        uint32_t raw_data = DHT11_Get_Temperature_And_RH();

        // Error checking and data display
        switch(raw_data) {
            case GND_ERROR:
                PRINTF("Error: Data line shorted to GND!\r\n");
                break;
            case VCC_ERROR:
                PRINTF("Error: Sensor not responding (Check VCC/GND)!\r\n");
                break;
            case CHECK_SUM_ERROR:
                PRINTF("Error: Checksum mismatch!\r\n");
                break;
            default:
                // Parse the 32-bit packed data
                uint8_t humidity = (uint8_t)(raw_data >> 24);
                uint8_t temp_int = (uint8_t)(raw_data >> 8);
                uint8_t temp_dec = (uint8_t)(raw_data);

                PRINTF("Humidity: %d%% | Temperature: %d.%d C\r\n", humidity, temp_int, temp_dec);
                break;
        }

        // DHT11 requires at least 2s between measurements
        delay_us(2000000);
    }
}
```
