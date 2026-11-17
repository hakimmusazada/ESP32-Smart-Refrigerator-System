// ESP32 Smart Portable Refrigerator - controller firmware, v2
//
// Same behaviour and wiring as the original firmware
// (../refrigerator_controller), with these fixes:
//   1. Door input uses the internal pull-down, so the pin no longer floats
//      while the reed switch is open (door open).
//   2. Door input is debounced (50 ms).
//   3. Timing constants are unsigned long, matching millis().
//   4. LCD lines are padded to exactly 16 characters.
//
// Door sensor wiring: 2-wire reed switch between 3.3 V and GPIO 2.
// Magnet near (door closed) -> switch closed -> GPIO 2 reads HIGH.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pins
const uint8_t DHT_PIN    = 4;
const uint8_t DOOR_PIN   = 2;
const uint8_t BUZZER_PIN = 15;
const uint8_t SDA_PIN    = 21;
const uint8_t SCL_PIN    = 22;

// Timing (ms)
const unsigned long SENSOR_INTERVAL = 2000;   // DHT11 read + LCD update
const unsigned long ALARM_DELAY     = 20000;  // door-open time before buzzer
const unsigned long DEBOUNCE_TIME   = 50;

DHT dht(DHT_PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long lastSensorRead = 0;
unsigned long doorOpenedAt   = 0;
unsigned long lastDoorChange = 0;
bool lastRawClosed  = true;
bool doorClosed     = true;

void printLine(uint8_t row, const String &text) {
  String line = text;
  while (line.length() < 16) line += ' ';
  lcd.setCursor(0, row);
  lcd.print(line.substring(0, 16));
}

void setup() {
  pinMode(DOOR_PIN, INPUT_PULLDOWN);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  dht.begin();

  printLine(0, "  System Ready");
  printLine(1, "");
  delay(2000);
  lcd.clear();
}

void updateDoor(unsigned long now) {
  bool rawClosed = (digitalRead(DOOR_PIN) == HIGH);
  if (rawClosed != lastRawClosed) {
    lastRawClosed = rawClosed;
    lastDoorChange = now;
  }
  if (now - lastDoorChange >= DEBOUNCE_TIME && rawClosed != doorClosed) {
    doorClosed = rawClosed;
    if (!doorClosed) doorOpenedAt = now;   // door has just been opened
  }

  bool alarm = !doorClosed && (now - doorOpenedAt >= ALARM_DELAY);
  digitalWrite(BUZZER_PIN, alarm ? HIGH : LOW);
}

void updateDisplay() {
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    printLine(0, "Sensor Error");
    printLine(1, "Check DHT pin");
    return;
  }
  printLine(0, "Temp: " + String(temperature, 1) + (char)223 + "C");
  printLine(1, "Humid: " + String(humidity, 1) + "%");
}

void loop() {
  unsigned long now = millis();
  updateDoor(now);

  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = now;
    updateDisplay();
  }
}
