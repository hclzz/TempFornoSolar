# Temperature Monitoring with SD Card Storage
## Requirements

- **Hardware:**
  - Arduino (Uno, Mega, or any other compatible board)
  - Temperature sensors (e.g., thermistors or LM35)
  - SD card module
  - Connecting wires
  - 10kΩ resistor (for thermistor configuration, if necessary)

- **Libraries:**
  - [SPI](https://www.arduino.cc/en/Reference/SPI): For communication with the SD card.
  - [SD](https://www.arduino.cc/en/Reference/SD): For reading and writing to the SD card.

## Setup

1. **Connections:**
   - Connect the temperature sensors to analog pins `A0`, `A1`, `A2`, and `A3`.
   - Connect the SD card module to the Arduino as per the standard SPI connections:
     - **VCC** → 5V or 3.3V (depending on the SD card module).
     - **GND** → GND.
     - **MISO** → Pin 12 on the Arduino.
     - **MOSI** → Pin 11 on the Arduino.
     - **SCK** → Pin 13 on the Arduino.
     - **CS (Chip Select)** → Pin 4 (can be changed in the code if needed).

2. **Initializing the SD Card:**
   - The code begins by checking if the SD card is initialized correctly. If there's an error, it will display a failure message.

## How It Works

1. **Reading Temperature Sensors:**
   - The code reads temperature values from 4 sensors connected to the analog pins `A0`, `A1`, `A2`, and `A3`. Each sensor is processed to convert its analog value to Celsius temperature using the Steinhart-Hart equation for thermistors.

2. **Storing Data on SD Card:**
   - Every 5 minutes (300,000 ms), the code writes the temperature values to a text file (`temperatures.txt`) on the SD card. The file format is:
     ```
     Elapsed Time (seconds), Ambient Temperature, Internal Temperature, Reference Temperature, Sample Temperature
     ```
     Example:
     ```
     12345,23.45 °C,22.50 °C,24.00 °C,25.10 °C
     ```

3. **File Header:**
   - The file generated on the SD card starts with a header that describes each temperature field.

4. **Storage Interval:**
   - Readings are taken and stored every 5 minutes. The time is calculated using the `millis()` function to simulate elapsed time since the program started.

## Main Functions

- `readTemperature(int pin)`: Reads the temperature sensor value from the specified pin and converts it to Celsius.
- `storeTemperature(String tempAmbient, String tempInternal, String tempReference, String tempSample)`: Stores the temperature values on the SD card in the `temperatures.txt` file.

## How to Use

1. Upload the code to your Arduino using the Arduino IDE.
2. Connect the temperature sensors and the SD card module to the Arduino as described.
3. Open the serial monitor to view real-time temperature readings.
4. Check the SD card to access the `temperatures.txt` file, where the temperature data will be stored.

## Example File Content of `temperatures.txt`

```
Timestamp, Ambient Temperature, Internal Temperature, Reference Temperature, Sample Temperature
12345,23.45 °C,22.50 °C,24.00 °C,25.10 °C
```

## Possible Improvements

- **RTC Module:** To include real-time clock (exact time), you could integrate an RTC module like the DS3231, replacing the time based on `millis()`.
- **Alerts:** Add an alert feature if temperatures exceed certain thresholds, using LEDs or a buzzer.
