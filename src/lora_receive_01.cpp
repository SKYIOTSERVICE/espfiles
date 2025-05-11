//https://www.youtube.com/watch?v=wH483V8fnN8

#include <LoRa.h>
#include <SPI.h>
 
#define ss D8
#define rst D0
#define dio0 D4
unsigned long myTime;
bool  bt_status = false; 
const int outpin=LED_BUILTIN;
int temp_count=0;

void setup() 
{
  pinMode(outpin, OUTPUT);
  pinMode(D2, INPUT_PULLUP);
  digitalWrite(outpin,HIGH);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
 
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  LoRa.setSyncWord(0xA2);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  //LoRa.begin(433E6)
  while (!LoRa.begin(433920000))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  digitalWrite(outpin,HIGH);
  Serial.println("LoRa Initializing OK!");
  
}
 
void loop() 
{
    Serial.println("sending packets...");
    LoRa.beginPacket();   //Send LoRa packet to receiver
    LoRa.print(9999);
    LoRa.endPacket();
    
  
  int packetSize = LoRa.parsePacket();    // try to parse packet
  if (packetSize) 
  {
    Serial.print("Packet Size:");
    Serial.println(packetSize);
    Serial.print("Received packet '");
    while (LoRa.available())              // read packet
    {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData); 
      int n=LoRaData.length();
      String SubData=LoRaData.substring(0,2);
      Serial.println(SubData);
      if(SubData.equals("11")){
        temp_count++;
        Serial.println("Running..................");
        if(temp_count>=100){
          digitalWrite(outpin,LOW);
        }
        
      }else{
        Serial.println("Off.......................");
        digitalWrite(outpin,HIGH);
      }
    }
    Serial.print("' with RSSI ");         // print RSSI of packet
    Serial.println(LoRa.packetRssi());

    Serial.println("sending packets...");
    LoRa.beginPacket();   //Send LoRa packet to receiver
    LoRa.print(1111);
    LoRa.endPacket();

  }
  
  delay(300);

}