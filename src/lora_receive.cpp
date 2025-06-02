//https://www.youtube.com/watch?v=wH483V8fnN8

#include <LoRa.h>
#include <SPI.h>
 
#define ss D8
#define rst D0
#define dio0 D4
unsigned long myTime;
bool  bt_status = false; 
const int outpin=D1;
int temp_count=0;

void setup() 
{
  pinMode(outpin, OUTPUT);
  pinMode(D2, INPUT_PULLUP);
  digitalWrite(outpin,LOW);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
 
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  LoRa.setSyncWord(0xA2);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);

  //LoRa.begin(433E6)
  int temp_count=30;
  while (!LoRa.begin(433000000))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    temp_count++;
    if(temp_count>=30){
      break;
    }
    delay(500);
  }
  digitalWrite(outpin,LOW);
  Serial.println("LoRa Initializing OK!");
  
}
 
void loop() 
{
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
      String SubData1=LoRaData.substring(0,4);
      String SubData2=LoRaData.substring(6,8);
      Serial.println(SubData1);
      if(SubData1.equals("1009") && SubData2.equals("11")){
        temp_count++;
        Serial.println("Running..................");
        if(temp_count>=10){
          digitalWrite(outpin,HIGH);
        }
        
      }else{
        Serial.println("Off.......................");
        digitalWrite(outpin,LOW);
      }
    }
    Serial.print("' with RSSI ");         // print RSSI of packet
    Serial.println(LoRa.packetRssi());
  }
  delay(500);

}
