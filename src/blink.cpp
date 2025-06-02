#include <Arduino.h>

const int pin=LED_BUILTIN;
const int apin=A0;


void setup()

{
  Serial.begin(115200); 
  pinMode(pin, OUTPUT);
}


void loop(){
     
  digitalWrite(pin, HIGH);
  delay(3000);
   Serial.println("Blink");
  digitalWrite(pin, LOW);
 delay(3000);
 Serial.println("Stop");


}
