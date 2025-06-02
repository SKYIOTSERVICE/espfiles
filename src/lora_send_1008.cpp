#include <LoRa.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

#define ss 15
#define rst 16
#define dio0 2
#define networkid "1008"
#define deviceid "1008"
 
int counter = 10;
const int tonepin = D1;
int value = 11;
const int spin=LED_BUILTIN;
int state=0;
int volt_state=1;
 
void setup() 
{
  Serial.begin(115200); 
  pinMode(D0, WAKEUP_PULLUP);
  WiFiMode(WIFI_STA);
  WiFi.disconnect(); 
  WiFi.mode(WIFI_OFF);
  
  pinMode(tonepin, INPUT_PULLUP);
  pinMode(spin,OUTPUT);

  digitalWrite(spin,HIGH);
  
  
  while (!Serial);
  Serial.println("LoRa Sender");
 
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  LoRa.setSyncWord(0xA2);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  while (!LoRa.begin(433920000))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  Serial.println("LoRa Initializing OK!");
}
 
void loop() 
{

  //Serial.print("Sending packet: ");
  //Serial.println(counter);
  //Serial.print("Analog Value:");
  Serial.println(analogRead(A0));
  if(digitalRead(tonepin)==0){
      if(state==0){
        state=1;
        digitalWrite(spin,LOW);
      }else{
        state=0;
        digitalWrite(spin,HIGH);
      }   
  }
 
  LoRa.beginPacket();   //Send LoRa packet to receiver
  LoRa.print(networkid);
  LoRa.print(deviceid);
  LoRa.print(counter);
  LoRa.print(value);
  LoRa.endPacket();
  Serial.print(networkid);
  Serial.print(deviceid);
  Serial.print(counter);
  Serial.print(value);
 
  counter++;
  if(counter>=90){
    counter=10;
    //ESP.deepSleep(30e6);
    //esp_light_sleep_start();
  }
 
  delay(100);
}