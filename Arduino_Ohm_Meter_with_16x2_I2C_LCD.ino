/*
========================================================
  Project : Simple Arduino Ohm Meter
  Display : 16x2 I2C LCD
  Target  : Beginner Electronics Learning Project
========================================================

DESCRIPTION:
This project is a simple Arduino Ohm Meter designed
for beginners to learn:

- Voltage divider circuits
- Analog input reading
- Resistance measurement
- I2C LCD interfacing

The project works best for measuring resistors
between 100 Ohms and 1K Ohms using a 1K reference
resistor.

FEATURES:
- Real-time resistance measurement
- 16x2 I2C LCD output
- Stable reading using averaging
- Beginner friendly
- Simple and clean circuit

COMPONENTS:
- Arduino UNO or Nano
- 16x2 I2C LCD
- 1K Ohm reference resistor
- Breadboard
- Jumper wires
- Resistor to measure

LCD I2C WIRING:
--------------------------------
LCD VCC  -> Arduino 5V
LCD GND  -> Arduino GND
LCD SDA  -> Arduino A4
LCD SCL  -> Arduino A5

OHM METER WIRING:
--------------------------------
5V ----[1K REF]----+----[Rx]---- GND
                   |
                  A0

[1K REF] = 1K reference resistor
[Rx]     = resistor being measured

NOTES:
- Best measurement range:
  100 Ohms to 1K Ohms
- Default LCD I2C address:
  0x27
- If LCD does not appear,
  try changing to 0x3F

LIBRARY:
- LiquidCrystal_I2C

========================================================
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ----------------------------------------------------
// LCD Configuration
// ----------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------------------------------------------------
// Pin Configuration
// ----------------------------------------------------
const byte ANALOG_PIN = A0;

// ----------------------------------------------------
// Ohm Meter Configuration
// ----------------------------------------------------
const float R_REFERENCE = 1000.0; // 1K Ohm
const byte SAMPLE_COUNT = 10;

// ----------------------------------------------------
// Read Analog Average
// Improves reading stability
// ----------------------------------------------------
int readAverageADC()
{
  long total = 0;

  for (byte i = 0; i < SAMPLE_COUNT; i++)
  {
    total += analogRead(ANALOG_PIN);
    delay(5);
  }

  return total / SAMPLE_COUNT;
}

// ----------------------------------------------------
// Calculate Unknown Resistance
// Formula:
// Rx = Rref × ADC / (1023 - ADC)
// ----------------------------------------------------
float calculateResistance(int adcValue)
{
  if (adcValue <= 0 || adcValue >= 1023)
    return 0;

  return R_REFERENCE *
         ((float)adcValue / (1023.0 - adcValue));
}

// ----------------------------------------------------
// Display Resistance on LCD
// ----------------------------------------------------
void displayResistance(float resistance)
{
  lcd.setCursor(0, 0);
  lcd.print("Resistance:   ");

  lcd.setCursor(0, 1);

  if (resistance >= 1000.0)
  {
    lcd.print(resistance / 1000.0, 2);
    lcd.print(" KOhm   ");
  }
  else
  {
    lcd.print(resistance, 0);
    lcd.print(" Ohm    ");
  }
}

// ----------------------------------------------------
// Setup
// ----------------------------------------------------
void setup()
{
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Arduino");
  lcd.setCursor(0, 1);
  lcd.print("Ohm Meter");

  delay(2000);

  lcd.clear();
}

// ----------------------------------------------------
// Main Loop
// ----------------------------------------------------
void loop()
{
  int adcValue = readAverageADC();

  // No resistor connected
  // or direct wire connection
  if (adcValue > 1000 || adcValue < 10)
  {
    lcd.setCursor(0, 0);
    lcd.print("Resistance:   ");

    lcd.setCursor(0, 1);
    lcd.print("No Resistor   ");

    delay(500);
    return;
  }

  float resistance =
    calculateResistance(adcValue);

  displayResistance(resistance);

  delay(300);
}