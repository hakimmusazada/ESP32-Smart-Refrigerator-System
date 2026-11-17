# Firmware

Two versions are included:

| Sketch | Description |
|---|---|
| [`refrigerator_controller`](../firmware/refrigerator_controller/refrigerator_controller.ino) | Original code, as run on the prototype |
| [`refrigerator_controller_v2`](../firmware/refrigerator_controller_v2/refrigerator_controller_v2.ino) | Same behaviour and wiring, with the fixes below |

## Behaviour

- **Start-up:** shows `System Ready` for 2 s.
- **Every 2 s:** reads the DHT11 and shows temperature (°C) and relative humidity on the LCD. If the read fails, it shows `Sensor Error / Check DHT pin`.
- **Door:** the reed switch reads HIGH when the door is closed (magnet near) and LOW when it is open. When the door opens, a timer starts. After 20 s (`alarmDelay`) the buzzer turns on, and it stays on until the door closes.

Both tasks use `millis()`, so the door check is never blocked by the sensor read. The firmware does not control the Peltier or fans.

## Changes in v2

| Issue in the original | Fix in v2 |
|---|---|
| Door pin set as plain `INPUT`: with a 2-wire reed switch the pin floats while the door is open | `INPUT_PULLDOWN` |
| No debounce on the door input | 50 ms debounce |
| Timing constants declared as `long` | `unsigned long`, matching `millis()` |
| LCD lines longer than 16 characters | Lines padded to 16 characters |

## Building

1. Install the [ESP32 board package](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) in the Arduino IDE and select an ESP32 Dev Module.
2. Install the libraries:
   - *DHT sensor library* (Adafruit), together with *Adafruit Unified Sensor*
   - *LiquidCrystal I2C* (Frank de Brabander)
3. Open a sketch folder and upload it.

> GPIO 2 and 15 are ESP32 strapping pins. If uploading fails, disconnect the door sensor during flashing.
