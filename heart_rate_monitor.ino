//define libraries
#include <Wire.h>                     // I2C bus
#include "MAX30100_PulseOximeter.h"   // pulse oximeter
#include <LiquidCrystal_I2C.h>        // LCD 

// initialize LCD display object with address 0x27, 16 characters, 2 lines
LiquidCrystal_I2C lcd(0x27, 16, 2);

// set the send interval of the sensor
#define REPORTING_PERIOD_MS 1000

// create new object of type pulsoximeter
PulseOximeter pox;

// define variables
int internalLED = 13;
uint32_t tsLastReport = 0;


// ***************************************************************************
void onBeatDetected()
{
  digitalWrite(internalLED, !digitalRead(internalLED));   // let the internal LED blink to indicate successful reading
}

// ***************************************************************************
void setup()
{
  Serial.begin(9600);

  pinMode(internalLED, OUTPUT);   // activate digital output 13 to control integrated LED

  lcd.init();                    // initialize the LCD
  lcd.backlight();               // turn on the backlight
  lcd.clear();

  if (!pox.begin())               // check if the sensor started successfully 
  {
    lcd.setCursor(0, 0);
    lcd.print("Start failed!");
    lcd.setCursor(1, 0);
    lcd.print("Please restart");

    Serial.println("Start failed! Please restart");

    while (1);
  }

  //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);

}



// ***************************************************************************
void loop()
{
  pox.update();                     // update the sensor values

  // check if the reading has been more than 1000 ms ago to start the loop
  if ( (millis() - tsLastReport) > REPORTING_PERIOD_MS)
  {
    lcd.clear();

    lcd.setCursor(0, 0);          // jump to the beginning of the first row to display the heart rate
    lcd.print("Puls: ");
    lcd.print((int) pox.getHeartRate());
    lcd.print(" bpm");

    lcd.setCursor(0, 1);          // jump to the beginning of the second row to display the oxygen level
    lcd.print("Sauerstoff: ");
    lcd.print(pox.getSpO2());
    lcd.print("%");

    Serial.print("Puls: ");
    Serial.print((int) pox.getHeartRate());
    Serial.println(" bpm");

    Serial.print("Sauerstoff: ");
    Serial.print(pox.getSpO2());
    Serial.println("%");

    tsLastReport = millis();    // save the time of this loop for comparison in the next one
  }
}
