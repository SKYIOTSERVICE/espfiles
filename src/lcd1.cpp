#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <LoRa.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Wire.begin(D4,D3);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world....!");
}


void loop()
{
    lcd.print("Hello, world ttt!");
    delay(1000);
    lcd.clear();
}