//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1. 

#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display - 0x3F
float act=0.0;
int vct=0;

int fno1=0;
int sno1=0;
int res1=0;

void setup()
{
  Serial.begin(115200); // Starts the serial communication
  lcd.init();                      // initialize the lcd 
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("FNO");
  lcd.setCursor(6,0);
  lcd.print("SNO");
  lcd.setCursor(12,0);
  lcd.print("ANS");

  
}

/*
void change_amp(){
  int tc=1;
  act=act+0.1;
  Serial.println(act);
  lcd.print(act);
}

void change_volt(){
  int tc=1;
  vct=vct+1;
  Serial.println(vct);
  lcd.print(vct);
}*/

void change_fno(){
  fno1=fno1+1;
  //Serial.println(vct);
  lcd.print(fno1);
}
void change_sno(){
 sno1=sno1+1;
 Serial.println(vct);
 lcd.print(sno1);
}

void change_res1(){
  res1=fno1*sno1;
  //Serial.println(vct);
  lcd.print(res1);
}
void loop()
{
  if(digitalRead(D5)==0){
    lcd.setCursor(0,1);
    change_fno();
    delay(100);
  }

  if(digitalRead(D6)==0){
    lcd.setCursor(6,1);
    change_sno();
    delay(100);
  }
  
  if(digitalRead(D7)==0){
    int t1=0;
    int count=0;
    lcd.setCursor(12,1);
    change_res1();
    while(digitalRead(D7)==0){
      count=count+1;
      t1=t1+1;
      Serial.print("click.....   : ");
      Serial.println(count);
      if(t1>=10){
        fno1=0;
        sno1=0;
        lcd.print(fno1);
        lcd.print(sno1);
        change_res1();
        t1=0;
        delay(2000);
      }
      delay(300);
    }
    delay(100);
  }


  delay(100);
}

