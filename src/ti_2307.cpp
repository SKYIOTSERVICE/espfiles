#include <LoRa.h>
#include <SPI.h>
//#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#define ss D8
#define rst D9
#define dio0 D10
#define networkid "1023"
#define rx_pin D4
 
String ds6 = "10230622";
String ds7 = "10230722";
int counter = 1;
const int hsen = D2;
const int lsen = D9;
const int hpin1 = D1;

int temp_count1=0;
int temp_count2=0;
int temp_count3=0;
int rnum = 2;
// Create a SoftwareSerial object for the receiver
SoftwareSerial receiverSerial1(rx_pin,D8);
SoftwareSerial receiverSerial2(rx_pin,D8);
 
void setup() 
{
  Serial.begin(115200); 
  receiverSerial1.begin(4800);
  //WiFiMode(WIFI_STA);
  //WiFi.disconnect(); 
  //WiFi.mode(WIFI_OFF);
  pinMode(hsen, INPUT_PULLUP); 
  pinMode(lsen, INPUT_PULLUP);
  pinMode(hpin1, OUTPUT);
  digitalWrite(hpin1,LOW);
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  LoRa.setSyncWord(0xA2);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(62.5E3);
  /*
  while (!LoRa.begin(433920000))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }*/
  Serial.println("LoRa Initializing OK!");
}

void send_data1(int rn){
  int i;
  for(i=0;i<=(7);i++){
    LoRa.beginPacket();   //Send LoRa packet to receiver
    LoRa.print(ds6);
    LoRa.endPacket(); 
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
}

void send_data2(int rn){
  int i;
  for(i=0;i<=(7);i++){
    LoRa.beginPacket();   //Send LoRa packet to receiver
    LoRa.print(ds7);
    LoRa.endPacket(); 
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
}
 
void loop() 
{
  Serial.print("Sending packet: ");
  Serial.println(counter);
  rnum=random(2,6);
  if (receiverSerial1.available() > 0) {
    Serial.println("Serial data recerived.....");
    int data = receiverSerial1.read(); // Read data from sender
    int data1 = receiverSerial1.read();
    receiverSerial1.
    Serial.println(data);
    Serial.println(data1);
    receiverSerial1.flush();

    if (data == 65) {
      ds6 = "10230600";
    } 
    if (data == 66 ) {
      ds6 = "10230611";
    }

    if (data1 == 67) {
      ds7 = "10230700";
    } 
    if (data1 == 68) {
      ds7 = "10230711";
    }
  }

  counter++;
  
  if(counter>=300){
    counter=10;
  }
 
  //send_data1(rnum);
  //LoRa.flush();
  delay(500);
  //send_data2(rnum);
  //LoRa.flush();
  delay(2000);
}


