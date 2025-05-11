#include <Wire.h>
#include <Arduino.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
  lcd.print(" Hello Makers "); // Start Printing
}

void loop(){
  delay(500);
  lcd.print(" Hello Makers ");
  // Nothing Absolutely Nothing!
}