# Hardware

## Components

| Component | Role |
|---|---|
| ESP32 development board | Controller (its onboard OLED is not used) |
| DHT11 | Internal temperature and humidity |
| 16x2 LCD with I2C backpack (0x27) | Display, mounted in the top panel |
| Reed switch (2-wire) + magnet | Door open/closed detection |
| Buzzer | Door-open alarm |
| Peltier module | Thermoelectric cooling |
| Large radiator + fan | Hot side, outside the enclosure |
| Small radiator + blower fan | Cold side, inside the enclosure |
| 12 V SMPS (6 A or 10 A model; exact rating not recorded) | 220 V AC to 12 V DC |
| 12 V battery | Alternative supply |
| XL4015 buck converter | Supply for the ESP32 |
| LM2596 buck converter | Supply for LCD, DHT11, buzzer and door sensor |
| Perfboard | Mounting for the ESP32 and wiring |

## Power

```text
220 V AC ─► SMPS (12 V) ─┐
                         ├─ manual selection ─► 12 V bus
12 V battery ────────────┘                        │
        ┌─────────────────────────────────────────┼──────────────────────────┐
        ▼                                         ▼                          ▼
 Peltier module + fans                    XL4015 buck ─► ESP32      LM2596 buck ─► LCD, DHT11,
                                                                                   buzzer, door sensor
```

- The Peltier and both fans run directly on the 12 V bus and are always on while powered.
- The ESP32 has a dedicated XL4015 regulator so that load changes on the other modules do not affect it.
- The source (SMPS or battery) is selected manually; there is no automatic switching.

## Cooling

- **Hot side:** thermal paste couples the Peltier to the large external radiator, and a fan removes the heat. Without enough airflow here the module overheats and stops cooling.
- **Cold side:** a small radiator inside the enclosure, with a blower fan circulating the cooled air.

## ESP32 connections

| GPIO | Connected to |
|---|---|
| 4 | DHT11 data |
| 2 | Reed switch |
| 15 | Buzzer |
| 21 / 22 | LCD SDA / SCL |

> **Note:** ESP32 inputs are 3.3 V. If the reed switch is supplied from a 5 V rail, connect it to the ESP32's 3.3 V pin instead.

## Enclosure

3D-printed body and door with a hinge and magnetic closure; see [`hardware/3d-models`](../hardware/3d-models). The body has a window in the top panel for the LCD and an opening in the side wall for the Peltier assembly.
