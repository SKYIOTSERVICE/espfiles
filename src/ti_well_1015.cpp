#include <LoRa.h>
#include <SPI.h>

#define ss D8
#define rst D0
#define dio0 D4
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

  Serial.print("Sending packet: ");
  Serial.println(counter);
  Serial.print("Analog Value:");
  Serial.println(analogRead(A0));

  LoRa.beginPacket();   //Send LoRa packet to receiver
  LoRa.print(networkid);
  LoRa.print(deviceid);
  LoRa.print(counter);
  LoRa.print(value);
  LoRa.endPacket();
 
  counter++;
  if(counter>=90){
    counter=10;
    digitalWrite(spin,LOW);
  }
 
  delay(100);
  digitalWrite(spin, HIGH);
}

